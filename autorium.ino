
// Define all the relay ports
#define relayLight 31
#define relayExtractorMotor 32
#define relayFilter 33
#define relayHeater 34
 

void setup() {
  // Initialze relay pins
  pinMode(relayLight, OUTPUT);
  pinMode(relayExtractorMotor, OUTPUT);
  pinMode(relayFilter, OUTPUT);
  pinMode(relayHeater, OUTPUT);

}

void loop() {
  
  // Test Code for relay

  for(int iCounter=31;iCounter < 39;iCounter++){
  
    digitalWrite(iCounter, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(iCounter, LOW);    // turn the LED off by making the voltage LOW
    delay(400);             // wait for a second
  }

}
