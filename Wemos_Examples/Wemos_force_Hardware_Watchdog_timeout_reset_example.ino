// This example is intended to test the Hardware Watchdog functionality by increasing the
// time at which heartneat pulses are sent to the watchdog until the watchdog times-out and reboots the Wemos.
// Not intended for use as a real-world sketch.
// Use of blocking delays in the void loop is not reccomended. See other examples which use SimpleTimer
// to trigger the hearbeat pulses.

#define heartbeat_pin 14 // Pin D5 - should match the solder bridge on the hardware watchdog
#define wemos_led_pin  2 // Built-in LED on Wemos attached to GPIO2 (Pin D4)

float delay_time_ms = 4000;

void setup()
{
  Serial.begin(74880);
  pinMode(heartbeat_pin, OUTPUT);   // Set Hardware watchdog jumper to match this
  pinMode(wemos_led_pin, OUTPUT);   // Wemos builtin LED - active LOW
  digitalWrite(wemos_led_pin,HIGH); // Turn Wemos built-in LED off
}


void loop()
{
  Serial.print(F("Delay between heartbeats = "));
  Serial.print(delay_time_ms/1000);
  Serial.println (F(" seconds")); 
  delay(delay_time_ms);               // introduce the delay
  digitalWrite(heartbeat_pin,HIGH);
  digitalWrite(wemos_led_pin,LOW);
  delay(50); // 50ms, but can be as low can be as low as 50us if needed
  digitalWrite(heartbeat_pin,LOW);
  digitalWrite(wemos_led_pin,HIGH);
  Serial.println(F("Heartbeat"));

  delay_time_ms = delay_time_ms+500;  // increase the delay by half a second each time
}
