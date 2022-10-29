// Change these settings to alter the two available initial_delay times, selectable via the Startup solder pads...
const unsigned long initial_delay_1 = (1000 * 10);            // give the Wemos 10 seconds to boot (Startup solder pads not bridged)
const unsigned long initial_delay_2 = (1000 * 20);            // give the Wemos 20 seconds to boot (Solder bridge in place between Startup solder pads)

// Change these settings to alter the two available heartbeat_timeout_delay times, selectable via the H-Beat solder pads...
const unsigned long heartbeat_timeout_delay_1 = (1000 * 10);  // 10 second heartbeat delay before reset (H-Beat solder pads not bridged)
const unsigned long heartbeat_timeout_delay_2 = (1000 * 15);  // 15 second heartbeat delay before reset (Solder bridge in place between H-Beat solder pads)


/* ------------- NO CHANGES REQUIRED BELOW THIS POINT - BUT PLEASE READ THE COMMENTS BELOW ------------- */


// This sketch is for the ATtiny85  
//
// Original code from here: https://www.instructables.com/id/External-Hardware-Watchdog/
// Modified by Pete Knight
//
// Download the ATtiny core from here: https://github.com/damellis/attiny/ (Other ATtiny cores may also work, this is the one that I have tested)
//
// Arduino IDE Settings...
// Board:       ATtiny/25/45/85                                                                           
// Processor:   ATtiny85                                                                                                                                                                                                                                                                                                                                                      
// Clock:       Internal 8 MHz
// Programmer:  Arduino as ISP (change this to suit your AVR Programmer, beware that many of the menu options in the IDE have similar names)
//
// Begin by burning the bootloader (Tools > "Burn Bootloader" in the Arduino IDE) then
// Upload the sketch using Sketch > "Upload Using Programmer CTRL+Shift+U" in the Arduino IDE

// This Hardware Watchdog timer circuit is intended to be used with the ATtiny Hardware Watchdog Timer PCB (v1.5 or later),
// full details available here: https://github.com/Peterkn2001/Wemos-D1-Mini-Hardware-Watchdog
// and used alongside a Wemos D1 mini or D1 Mini Pro.

// The Watchdog LED will be lit during the initial_delay priod then will flash when
// a heartbeat pulse is recieved from the Wemos. If the initial_delay has passed
// and no heartbeat pulse is recieved from the Wemos for more than the period of the
// heartbeat_timeout then the Wemos D1 Mini/Pro (along with the Watchdog circuit) will be rebooted.
//
// On startup and after a reset, initial_delay is invoked to insure the Wemos has time to
// properly boot up and begin emitting the heartbeat signal on either D1, D2, D5, D6 or D7 (the solder bridge on the Watchdog timer must match this)
// If no hearbeat pulse is recieved from the Wemos after startup then the Wemos will be rebooted after the
// initial_delay and the heartbeat_timeout combined have passed

// NOTE - The Wemos and ATtiny Reset pins are tied together, so resetting the Wemos via the biult-in
// reset button also reboots the Watchdog Timer.


/* -------------------------------------- END OF COMMENTS -------------------------------------- */


// ATting85 GPIO's (PB's) For Info Only...
// 0  initial_delay selector      Pull to GND via solder pad to enable initial_delay_2
// 1  heartbeat_timeout selector  Pull to GND via solder pad to enable heartbeat_timeout_delay_2
// 2  heartbeat_pin               Conencted to D1, D2, D5, D6 or D7 via solder pads. Only one set of solder pads should be bridged
// 3  reset_signal                Pin that is pulled LOW to reset the Wemos and the Watchdog Timer
// 4  led_pin                     Pin that has the LED attached to indicate initial_delay period (on) and heartbeat recieved (flashes on)


unsigned long initial_delay;
unsigned long heartbeat_timeout_delay;

// constants that won't change -- used to set pin numbers and define delays
const int initial_delay_select_pin =  0;      // Used to choose between initial_delay_1 and 2 using the White jumper
const int heartbeat_freq_select_pin = 1;      // Used to hoose between heartbeat_timeout_delay_1 and 2 using the Blue jumper
const int heartbeat_pin =             2;      // Use the signal from the host to set the heartbeat status (Connected to either D1, D2, D5, D6 or D7 on Wemos)
const int reset_signal =              3;      // Use this pin to reset the host (Connected to RST on Wemos)
const int led_pin =                   4;      // Onboard LED (connected to ground via a 1k resistor, active HIGH)


// global variables
int currState = HIGH;
int prevState = LOW;


/* --------------------------------------------------------------------------------- */


// Class "timer" -- monitors a time instance and returns 1 if it has passed or 0 otherwise
// It requires a time interval in milliseconds

class timer
{
  //Class variables initialized at startup
  unsigned long timerDuration; // How long is this timer for?
  
  // Runtime variables
  unsigned long startMillis; // When did it start?
  
  // Constructor
  public:
  timer(unsigned long timer) {
  timerDuration = timer;
  startMillis = millis();
  }
  
  int update(void)
  {
    if (millis() - startMillis >= timerDuration)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  
  void reset(void)
  {
    startMillis = millis();
  }
};


/* --------------------------------------------------------------------------------- */


// Create timer instances
timer initialWaitTimer = timer(initial_delay);              // initial delay timer
timer heartbeat_timeout = timer(heartbeat_timeout_delay);   // heartbeat timer


/* --------------------------------------------------------------------------------- */


void setup()
{
  // Set pin modes
  pinMode(reset_signal, INPUT_PULLUP); // This pin is connected to RST on the ATtiny85. Keep HIGH by declaring as INPUT_PULLUP at boot
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH); // Turn the LED pin ON (HIGH) initially (until the initial_delay has passed)

  delay(50);   
  pinMode(reset_signal, OUTPUT);    // Now set the pin to OUTPUT
  digitalWrite(reset_signal, HIGH); // And make it HIGH

  pinMode(heartbeat_pin, INPUT); 
  pinMode(initial_delay_select_pin, INPUT_PULLUP);
  pinMode(heartbeat_freq_select_pin, INPUT_PULLUP);

  readTimingSettings();             // Read which of the initial_delay and heartbeat_timeout_delay timing options is to be used

  // Update timers...
  initialWaitTimer = timer(initial_delay);            // reset initial delay timer
  heartbeat_timeout = timer(heartbeat_timeout_delay);       // heartbeat timer
    
  // Start with a zeroed-out timers...
  initialWaitTimer.reset();
  heartbeat_timeout.reset();
}


/* --------------------------------------------------------------------------- */


void loop()
{
  if(initialWaitTimer.update() == 1) // if the initial_delay has passed
  {
    ledHeartbeat();
    if (heartbeat_timeout.update() == 1) 
    {
      resetHost(reset_signal);
    }
  }
  else
  {
    heartbeat_timeout.reset(); // Keep resetting the heartbeat timeout timer until the initial_delay has passed    
  }
}


/* --------------------------------------------------------------------------------- */


void ledHeartbeat(void) 
{
  // Pulse the LED to show that a heartbeat has been recieved
  digitalWrite(led_pin, currState = digitalRead(heartbeat_pin));
  if ( currState != prevState) 
  { // State has changed
    prevState = currState; // Update
    heartbeat_timeout.reset(); // Reset the timer
  }
}


/* -------------------------------------------------------------------------------- */


void resetHost(int reset_signal)
{
  digitalWrite(reset_signal, LOW); // Pull the MCU's RST pin LOW (Reboot)
  delay(500); // wait half a second before we do anything else

  // The code after this point won't execute if the Reset pin on the
  // ATtiny85 is tied to the RST pin on the Wemos D1 Mini, as the ATTiny will reboot
  // and the code execution will begin from the top.
  // This code is included to give the option not to configure the circuit that way
  
  digitalWrite(reset_signal, HIGH); // Turn it back on so the host can boot!
  initialWaitTimer.reset(); // Zero out the timers
  heartbeat_timeout.reset();
  digitalWrite(led_pin, HIGH); // Turn the LED back on again to indicate taht we are in initial_delay
}


/* -------------------------------------------------------------------------------- */


void readTimingSettings(void)
{
  int result;
  
  result = digitalRead (initial_delay_select_pin);
  if(result == 1)
  {
    initial_delay = initial_delay_1;
  }
  else
  {
    initial_delay = initial_delay_2;   
  }
  
  
  result = digitalRead (heartbeat_freq_select_pin);
  if(result == 1)
  {
    heartbeat_timeout_delay = heartbeat_timeout_delay_1;
  }
  else
  {
    heartbeat_timeout_delay = heartbeat_timeout_delay_2;   
  }
}
