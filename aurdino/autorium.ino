#include "autorium.h"

  /*
   * TODO:
   *      Water level sensing may need to be averaged out more for stable values [DONE - Values increase to 15 iterations]
   *      Convert the flow counter from int to long [DONE - Test if its working]
   *      Set RTC Clock timing and change battery
   *      Connect Water Extractor Pump, Temperature Sensor, Heater, Light, Air Pump & Filter Pump to Relay
   *      Integrate ESP32 chip for wifi connectivity
   *      Relay test does no longer need to switch on and off all the ports
   *      Variables strating with 'aq' are connected to Main Aquarium, those starting with 'fl' are connected to the Filter/Sump
   * 
   *      
   *      Steps:
   *      a) Complete the aquarium filter combination first
  */


void setup(void) {

  Serial.println("** Start of Setup **");
  
  byte arrayCount;   // variable to store array count

  // initialize light relay pins
  arrayCount = sizeof(arLightRelayPin) / sizeof(arLightRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arLightRelayPin[thisPin], OUTPUT);
  }

  // initialize air pump relay pins
  arrayCount = sizeof(arAirPumpRelayPin) / sizeof(arAirPumpRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arAirPumpRelayPin[thisPin], OUTPUT);
  }

  // initialize heater relay pins
  arrayCount = sizeof(arHeaterRelayPin) / sizeof(arHeaterRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arHeaterRelayPin[thisPin], OUTPUT);
  }

  // initialize solenoid relay pins
  arrayCount = sizeof(arNOSolenoidRelayPin) / sizeof(arNOSolenoidRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arNOSolenoidRelayPin[thisPin], OUTPUT);
  }

  arrayCount = sizeof(arNCSolenoidRelayPin) / sizeof(arNCSolenoidRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arNCSolenoidRelayPin[thisPin], OUTPUT);
  }
  
  // initialize flow sensor pins
  arrayCount = sizeof(arFlowSensorPin) / sizeof(arFlowSensorPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arFlowSensorPin[thisPin], INPUT);
    // attachInterrupt requires callback function, which should be unique to each interrupt
    //attachInterrupt(digitalPinToInterrupt(arFlowSensorPin[thisPin], inwardFlowSensor, RISING);
  }
  
  // initialize motor relay pumps
  pinMode(aqPumpRelayPin, OUTPUT);
  pinMode(aqPumpRelayPin, OUTPUT);

  // initialize aquarium ultrasnoic sensor pins
  pinMode(aqUltrasonicSensorTriggerPin, OUTPUT);
  pinMode(aqUltrasonicSensorEchoPin, INPUT);

  // initialize filter float sensor pins
  pinMode(flFloatSensorHighPin, INPUT_PULLUP);
  pinMode(flFloatSensorLowPin, INPUT_PULLUP);
  pinMode(flOverFlowFloatSensorHighPin, INPUT_PULLUP);
  pinMode(flOverFlowFloatSensorLowPin, INPUT_PULLUP);

  // set all the pins to safe values
  resetAllPins();

  // TODO: Initialize the ESP32 board RS232 ports
  
  Serial.begin(9600);  // Initialise the serial port. This would be the same serial port used for programming the board.
  Serial.print("Starting Autorium version ");
  Serial.println(AUTORIUM_VERSION);
  Serial.println("Debug Mode is ON");
  Serial.print("Aquarium Height -> ");
  Serial.println(aqHeight);
  Serial.print("Aquarium Width -> ");
  Serial.println(aqWidth);
  Serial.print("Aquarium Length -> ");
  Serial.println(aqLength);
  Serial.print("Aquarium Filled Depth -> ");
  Serial.println(aqFilledDepth);
  
  //i2cScanner();         // Run the I2C scanner to check for peripherals

  Serial.println("** End of Setup **");
  
  
}

void loop(void) {

  Serial.println("** Start of Loop **");
  
  
    
  Serial.println("** End of Loop **");
}

void operateFilter(){
  if(autoriumState==1){
    // TODO: Filtering is in process, keep checking for water level in the aquarium, filter and overflow section. Also need to check for the temperature
    // TODO: At a later stage, we need to inetgerate the DO/PH etc sensors
    // TODO: Operation of motors (aquarium and filter should be done here. Water level needs to be checked before operating the motor) 
  }else{

    // Start the filtering process

    // Reset all to safe values
    resetAllPins(1);

    // Drain filter first
    drainFilter();

    // Open up the solenoid valve from aquarium to filter and vice versa. These are NO valves, so we need to set the pin to LOW
    digitalWrite(arNOSolenoidRelayPin[0], LOW);   //From Aquarium to Filter
    digitalWrite(arNOSolenoidRelayPin[1], LOW);   //From Filter to Aquarium


  }
}

void fillFilter(){

  if(autoriumState==5){
    // TODO: Filling of filter is in process, keep checking for water level in the filter and overflow section
  }else{
    // Start the filling process for the filter 

    // Reset all to safe values
    resetAllPins(1);

    // Open up the solenoid valve for freshwater input to filter 
    digitalWrite(arNCSolenoidRelayPin[3], HIGH);

    // set the autorium state
    autoriumState = 5;
  }
}

void fillAquarium(){

  if(autoriumState==3){
    // TODO: Filling of Aquarium is in process, keep checking for water level in the aquarium
  }else{
    // Start the filling process for the aquarium 

    // Reset all to safe values
    resetAllPins(1);

    // Open up the solenoid valve for freshwater input to aquarium 
    digitalWrite(arNCSolenoidRelayPin[2], HIGH);

    // set the autorium state
    autoriumState = 3;
  }
}

void drainAquarium(){
  
  if(autoriumState==2){
    // TODO: Draining of Aquarium is in process, keep checking for water level in the aquarium
  }else{
    // Start the draining process for the aquarium

    // Reset all to safe values
    resetAllPins(1);

    // Open up the solenoid valve which leads from the aquarium to drain
    digitalWrite(arNCSolenoidRelayPin[0], HIGH);

    // Start the aquarium motor
    digitalWrite(aqPumpRelayPin, HIGH);

    // set the autorium state
    autoriumState = 2;
  }
}

void drainFilter(){
  
  if(autoriumState==4){
    // TODO: Draining of Filter is in process, keep checking for water level in the filter and the overflow section
  }else{
    // Start the draining process for the filter

    // Reset all to safe values
    resetAllPins(1);

    // Open up the solenoid valve which leads from the filter to drain
    digitalWrite(arNCSolenoidRelayPin[1], HIGH);

    // Start the filter motor
    digitalWrite(flPumpRelayPin, HIGH);

    // Start the overflow motor
    digitalWrite(flOverFlowPumpRelayPin, HIGH);

    // set the autorium state
    autoriumState = 4;
  }

}

void resetAllPins(int closeNOValves = 0){
  
  byte arrayCount;   // variable to store array count
  
  // switch off all the lights
  arrayCount = sizeof(arLightRelayPin) / sizeof(arLightRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    digitalWrite(arLightRelayPin[thisPin], LOW); 
  }

  // switch off air pumps
  arrayCount = sizeof(arAirPumpRelayPin) / sizeof(arAirPumpRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    digitalWrite(arAirPumpRelayPin[thisPin], LOW); 
  }

  // switch off heaters
  arrayCount = sizeof(arHeaterRelayPin) / sizeof(arHeaterRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    digitalWrite(arHeaterRelayPin[thisPin], LOW); 
  }

  // opens solenoid valves, =these are normally open ones
  if(closeNOValves == 1){
    arrayCount = sizeof(arNOSolenoidRelayPin) / sizeof(arNOSolenoidRelayPin[0]);
    for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
      digitalWrite(arNOSolenoidRelayPin[thisPin], HIGH); 
    }
  }else{
    arrayCount = sizeof(arNOSolenoidRelayPin) / sizeof(arNOSolenoidRelayPin[0]);
    for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
      digitalWrite(arNOSolenoidRelayPin[thisPin], LOW); 
    }
  }

  // close solenoid valves,  these are normally closed ones
  arrayCount = sizeof(arNCSolenoidRelayPin) / sizeof(arNCSolenoidRelayPin[0]);
  for (byte thisPin = 0; thisPin < arrayCount; thisPin++) {
    digitalWrite(arNCSolenoidRelayPin[thisPin], LOW); 
  }

  // switch off both the motor pumps
  digitalWrite(aqPumpRelayPin, LOW); // switch off aquarium pump
  digitalWrite(flPumpRelayPin, LOW); // switch off filter pump

  // set the autorium state
  autoriumState = 0;

}

/*
// I2C scanner for debugging
void i2cScanner(void){
  byte error, address;
  int nDevices;

  Serial.println("Scanning for I2C modules/controllers...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
   
}
*/
