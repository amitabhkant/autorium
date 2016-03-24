#include "autorium.h"

void setup() {
  // Initialze relay pins
  pinMode(relayLight, OUTPUT);
  pinMode(relayExtractorMotor, OUTPUT);
  pinMode(relayFilter, OUTPUT);
  pinMode(relayHeater, OUTPUT);

  // Intialize all motors/relays to safe values
  digitalWrite(relayExtractorMotor, LOW);     // Set the extractor motor relay to low state (Switch off)
  digitalWrite(relayHeater, LOW);             // Set the heater relay to low state (Switch off)
  digitalWrite(relayFilter, LOW);             // Set the heater relay to low state (Switch off)

  #if defined(devMode)
    Serial.begin(9600);  // Initialise the serial port. This would be the same serial port used in programming.
    Serial.print("Starting Autorium version ");
    Serial.println(AUTORIUM_VERSION);
  #endif

  initRelayBoard();       // 
    
  
  

}

void loop() {
  
  // test_8_board_relay();


}

// Blinks all ports of the relay board
void initRelayBoard(){

  for(int iCounter=31;iCounter < 39;iCounter++){
  
    digitalWrite(iCounter, HIGH);   // turn the Relay on (HIGH is the voltage level)
    delay(500);              // wait for 500 milliseconds
    digitalWrite(iCounter, LOW);    // turn the relay off by making the voltage LOW
    delay(400);             // wait for 400 millisecond
  }
  
}

