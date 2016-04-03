#include "autorium.h"
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <NewTone.h>

LiquidCrystal_I2C lcd(i2cLCD, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int currentAutoriumState = 0;                 // Stores if any current action is being performed by Autorium: 0 - None; 1 - Water Extract;; 2 - Water Refill  

void setup() {
  // Initialze relay pins
  pinMode(relayLight, OUTPUT);
  pinMode(relayExtractor, OUTPUT);
  pinMode(relayFilter, OUTPUT);
  pinMode(relayHeater, OUTPUT);
  pinMode(relayAirPump, OUTPUT);

  // Intialize all motors/relays to safe values
  digitalWrite(relayLight, HIGH);             // Set the light relay to high state (Switch on)
  digitalWrite(relayExtractor, LOW);          // Set the extractor motor relay to low state (Switch off)
  digitalWrite(relayFilter, LOW);             // Set the filter relay to low state (Switch off)
  digitalWrite(relayHeater, LOW);             // Set the heater relay to low state (Switch off)
  digitalWrite(relayAirPump, HIGH);           // Set the Air Pump relay to high state (Switch on)

  // Initialize solenoid pins
  pinMode(inFlowSolenoid, OUTPUT);
  pinMode(outFlowSolenoid, OUTPUT);

  // Intialize all motors/relays to safe values
  digitalWrite(inFlowSolenoid, LOW);          // Set the in flow solenoid valve to closed (No water flow)
  digitalWrite(outFlowSolenoid, LOW);         // Set the out flow solenoid valve to closed (No water flow)
  

  #if defined(devMode)
    Serial.begin(9600);  // Initialise the serial port. This would be the same serial port used for programming the board.
    Serial.print("Starting Autorium version ");
    Serial.println(AUTORIUM_VERSION);
    Serial.println("Debug Mode is ON");
  #endif

  initRelayBoard(31, 5);      // Led blink test on all active ports
      
  initLCD(16, 2);             // LCD test

  // Max water level should be 5 cm less than Aquarium height
  if(maxWaterLevel > aquariumHeight - 5){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Max wtr level >");
    lcd.setCursor(0,1);
    lcd.print("Aquarium height");
    #if defined(devMode)
      Serial.println("!! => Max water level is greater than the maximum aquarium depth. Please ensure that the maxWaterLevel is 5 cm less than aquariumHeight");
    #endif
  }

  NewTone(TONE_PIN, 262, 800); // Play a beep for startup.

}

void loop() {
  // STEPS:
  //
  // a) Get the depth for water using ultrasonic sensor (Use NewPing library)
  // b) 

}


// Blinks all active ports of the relay board
void initRelayBoard(int relayPinStart, int relayPorts){
  
  for(int iCounter=relayPinStart;iCounter < relayPinStart + relayPorts;iCounter++){
  
    digitalWrite(iCounter, HIGH);   // turn the Relay on (HIGH is the voltage level)
    delay(100);              // wait for 500 milliseconds
    digitalWrite(iCounter, LOW);    // turn the relay off by making the voltage LOW
    delay(100);             // wait for 400 millisecond
  }

  for(int iCounter=relayPinStart + relayPorts -1 ;iCounter >= relayPinStart;iCounter--){
  
    digitalWrite(iCounter, HIGH);   // turn the Relay on (HIGH is the voltage level)
    delay(100);              // wait for 500 milliseconds
    digitalWrite(iCounter, LOW);    // turn the relay off by making the voltage LOW
    delay(100);             // wait for 400 millisecond
  }
  
}

// Blinks LCD thrice
void initLCD(int columns, int rows){
  lcd.begin(columns, rows);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  // ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.setBacklight(LOW);
  lcd.backlight(); // finish with backlight on  
  //lcd.setContrast(HIGH);

  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Autorium version ");
  lcd.setCursor(0,1);
  lcd.print(AUTORIUM_VERSION);
}


