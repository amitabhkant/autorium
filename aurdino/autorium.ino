#include "autorium.h"

  /*
   * TODO:
   *      Water level sensing may need to be averaged out more for stable values [DONE - Values increase to 15 iterations]
   *      Convert the flow counter from int to long [DONE - Test if its working]
   *      Set RTC Clock timing and change battery
   *      Connect Water Extractor Pump, Temperature Sensor, Heater, Light, Air Pump & Filter Pump to Relay
   *      Integrate ESP32 chip for wifi connectivity
   *      Relay test does no longer need to switch on and off all the ports
   *      
   *      Steps:
   *      a) Complete the aquarium filter combination first
  */


void setup(void) {

  Serial.println("** Start of Setup **");
  
  int arrayCount;   // variable to store array count

  /*
  // initialize light relay pins
  arrayCount = sizeof(arLightRelayPin) / sizeof(arLightRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arLightRelayPin[thisPin], OUTPUT);
    digitalWrite(arLightRelayPin[thisPin], LOW); // switch off all the lights
  }
  */

  /*
  // initialize air pump relay pins
  arrayCount = sizeof(arAirPumpRelayPin) / sizeof(arAirPumpRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arAirPumpRelayPin[thisPin], OUTPUT);
    digitalWrite(arAirPumpRelayPin[thisPin], LOW); // switch off air pumps
  }
  */

  /*
  // initialize heater relay pins
  arrayCount = sizeof(arHeaterRelayPin) / sizeof(arHeaterRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arHeaterRelayPin[thisPin], OUTPUT);
    digitalWrite(arHeaterRelayPin[thisPin], LOW); // switch off heaters
  }
  */


  // initialize solenoid relay pins
  arrayCount = sizeof(arSolenoidRelayPin) / sizeof(arSolenoidRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arSolenoidRelayPin[thisPin], OUTPUT);
    digitalWrite(arSolenoidRelayPin[thisPin], LOW); // switch off solenoid valves
  }

  // initialize flow sensor pins
  arrayCount = sizeof(arFlowSensorPin) / sizeof(arFlowSensorPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arFlowSensorPin[thisPin], INPUT);
    // attachInterrupt requires callback function, which should be unique to each interrupt
    //attachInterrupt(digitalPinToInterrupt(arFlowSensorPin[thisPin], inwardFlowSensor, RISING);
  }
  
  // switch off both the motor pumps
  pinMode(aqPumpRelayPin, OUTPUT);
  digitalWrite(aqPumpRelayPin, LOW); // switch off aquarium pump
  pinMode(aqPumpRelayPin, OUTPUT);
  digitalWrite(flPumpRelayPin, LOW); // switch off filter pump

  // initialize aquarium ultrasnoic sensor pins
  pinMode(aqUltrasonicSensorTriggerPin, OUTPUT);
  pinMode(aqUltrasonicSensorEchoPin, INPUT);

  // initialize filter float sensor pin
  pinMode(flFloatSensorPin, INPUT_PULLUP);


  
  //initRelayBoard(31, 8);      // Led blink test on all active ports


  Serial.begin(9600);  // Initialise the serial port. This would be the same serial port used for programming the board.
  Serial.print("Starting Autorium version ");
  Serial.println(AUTORIUM_VERSION);
  Serial.println("Debug Mode is ON");
  
  //i2cScanner();         // Run the I2C scanner to check for peripherals

  Serial.println("** End of Setup **");
  
  
}

void loop(void) {

  Serial.println("** Start of Loop **");
  
  
    
  Serial.println("** End of Loop **");
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
