// Simple example of how to use SimpleTimer to trigger heartbeat pulses for the Hardware Watchdog 

#include <ESP8266WiFi.h>
#include <SimpleTimer.h>

#define heartbeat_pin 14 // Pin D5 - should match the solder bridge on the hardware watchdog
#define wemos_led_pin  2 // Built-in LED on Wemos attached to GPIO2 (Pin D4)

SimpleTimer timer;

void setup()
{
  Serial.begin(74880);
  
  pinMode(heartbeat_pin, OUTPUT);   // Set Hardware watchdog jumper to match this
  pinMode(wemos_led_pin, OUTPUT);   // Wemos builtin LED - active LOW
  digitalWrite(wemos_led_pin,HIGH); // Turn Wemos built-in LED off

  timer.setInterval(5000, send_heartbeat);
}


void loop()
{
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
