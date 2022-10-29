// Sketch to demonstrate how to configure OTA settings whne using the Hardware Watchdog
// to prevent the watchdog rebooting the Wemos part way through the OTA update process.

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <SimpleTimer.h>

#define heartbeat_pin 14 // Pin D5 - should match the solder bridge on the hardware watchdog
#define wemos_led_pin  2 // Built-in LED on Wemos attached to GPIO2 (Pin D4)

const char *ssid =      "REDACTED";      
const char *pass =      "REDACTED";

WiFiClient My_WiFi_Client;
SimpleTimer timer;

void WifiConnect()
{
  Serial.print(F("Connecting to Wi-Fi"));
  
  WiFi.begin(ssid, pass);
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(" CONNECTED"));
  Serial.println();
  
  Serial.print(F("MAC Address:           "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("IP Address:            "));
  Serial.println(WiFi.localIP()); 
  Serial.println();
}


void setup()
{
  Serial.begin(74880);
  
  WifiConnect();
  
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
    ESP.restart();
  });
  
  ArduinoOTA.setHostname("Heartbeat_OTA_Timer_Test");   

  ArduinoOTA.onStart([]() {
  send_heartbeat(); 
  }); //Send a heartneat pulse when the OTA update process starts, to avoid a timeout
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100))); timer.run();
  }); // Run timer during the OTA process to avoid heartbeat timeout
  
  ArduinoOTA.begin();  // Start the OTA server
  
  pinMode(heartbeat_pin, OUTPUT);   // Set Hardware watchdog jumper to match this
  pinMode(wemos_led_pin, OUTPUT);   // Wemos builtin LED - active LOW
  digitalWrite(wemos_led_pin,HIGH); // Turn Wemos built-in LED off

  timer.setInterval(5000, send_heartbeat);
}


void loop()
{
  ArduinoOTA.handle();
  timer.run();
}


void send_heartbeat()
{
    digitalWrite(heartbeat_pin,HIGH);
    digitalWrite(wemos_led_pin,LOW);
    delay(50); // 50ms, but can be as low can be as low as 50us if needed
    digitalWrite(heartbeat_pin,LOW);
    digitalWrite(wemos_led_pin,HIGH);
    Serial.println(F("Heartbeat"));
}
