#include "autorium.h"
//#include <EEPROM.h>
//#include <avr/sleep.h>
//#include <Wire.h>
//#include <NewPing.h>
//#include <NewTone.h>

  /*
   * TODO:
   *      Store operation mode (extraction or filling) in EEPROM to overcome any reboots due to power failure [DONE]
   *      Water level sensing may need to be averaged out more for stable values [DONE - Values increase to 15 iterations]
   *      Convert the flow counter from int to long [DONE - Test if its working]
   *      Set RTC Clock timing and change battery
   *      Connect Water Extractor Pump, Temperature Sensor, Heater, Light, Air Pump & Filter Pump to Relay
   *      Integrate ESP82xx chip for wifi connectivity
   *      Relay test does no longer need to switch on and off all the ports
   *      
  */

#if defined(TEMPERATURE_SENSOR_MODE)
  #include <OneWire.h>
  #include <DallasTemperature.h> 
  OneWire oneWire(TEMPERATURE_SENSOR_PIN);    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs) 
  DallasTemperature dtSensors(&oneWire);        // Pass our oneWire reference to Dallas Temperature.
#endif

#if defined(LCD_MODE)
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

NewPing sonar(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ultrasonicMaxDistance);

void setup(void) {

  // Set the default on-baord LCD to LOW
  digitalWrite(13, LOW);

  int arrayCount;   // variable to store array count

  // initialize light relay pins
  arrayCount = sizeof(arLightRelayPin) / sizeof(arLightRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arLightRelayPin[thisPin], OUTPUT);
    digitalWrite(arLightRelayPin[thisPin], LOW); // switch off all the lights
  }

  // initialize air pump relay pins
  int arrayCount = sizeof(arAirPumpRelayPin) / sizeof(arAirPumpRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arAirPumpRelayPin[thisPin], OUTPUT);
    digitalWrite(arAirPumpRelayPin[thisPin], LOW); // switch off air pumps
  }

  // initialize heater relay pins
  int arrayCount = sizeof(arHeaterRelayPin) / sizeof(arHeaterRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arHeaterRelayPin[thisPin], OUTPUT);
    digitalWrite(arHeaterRelayPin[thisPin], LOW); // switch off heaters
  }

  // initialize solenoid relay pins
  int arrayCount = sizeof(arSolenoidRelayPin) / sizeof(arSolenoidRelayPin[0]);
  for (int thisPin = 0; thisPin < arrayCount; thisPin++) {
    pinMode(arSolenoidRelayPin[thisPin], OUTPUT);
    digitalWrite(arSolenoidRelayPin[thisPin], LOW); // switch off solenoid valves
  }

  // initialize flow sensor pins
  int arrayCount = sizeof(arFlowSensorPin) / sizeof(arFlowSensorPin[0]);
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




  
  //initRelayBoard(31, 8);      // Led blink test on all active ports

  // Intialize all motors/relays to safe values
  digitalWrite(LIGHT_RELAY_PIN, LOW);             // Set the light relay to high state (Switch on)
  digitalWrite(EXTRACTOR_RELAY_PIN, LOW);          // Set the extractor motor relay to low state (Switch off)
  digitalWrite(FILTER_RELAY_PIN, LOW);             // Set the filter relay to low state (Switch off)
  digitalWrite(HEATER_RELAY_PIN, LOW);             // Set the heater relay to low state (Switch off)
  digitalWrite(AIR_PUMP_RELAY_PIN, LOW);           // Set the Air Pump relay to high state (Switch on)

  // Close all solenoid valves
  digitalWrite(INWARD_FLOW_SOLENOID_PIN, LOW);          // Set the in flow solenoid valve to closed (No water flow)
  digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);         // Set the out flow solenoid valve to closed (No water flow)

  // Initialize Ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Intialize LED Pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  // Initialize flow sensor pins
  pinMode(INWARD_FLOW_SENSOR_PIN, INPUT);
  pinMode(OUTWARD_FLOW_SENSOR_PIN, INPUT);

  // Attach interrupt with flow sensor pins
  attachInterrupt(digitalPinToInterrupt(INWARD_FLOW_SENSOR_PIN), inwardFlowSensor, RISING);
  attachInterrupt(digitalPinToInterrupt(OUTWARD_FLOW_SENSOR_PIN), outwardFlowSensor, RISING);

  Wire.begin();   // Initialize the I2C wire protocol

  NewTone(TONE_PIN, 262, 800); // Play a beep for startup.

  #if defined(LCD_MODE)
    initLCD(16, 2);             // LCD test
  #endif

  Serial.begin(9600);  // Initialise the serial port. This would be the same serial port used for programming the board.
  Serial.print("Starting Autorium version ");
  Serial.println(AUTORIUM_VERSION);
  Serial.println("Debug Mode is ON");
  
  i2cScanner();         // Run the I2C scanner to check for peripherals

  displayTime(); // Display current time on LCD and Serial port

  dtSensors.begin(); // Dallas temperature IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement


  waterLevel = getWaterLevel(50);    // Get the intial water level
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
/*
  // Max water level should be 5 cm less than Aquarium height
  if(maxWaterLevel > aquariumHeight - 5){
    
    #if defined(LCD_MODE)
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Max wtr level: ");
      lcd.setCursor(0,1);
      lcd.print("Aquarium height");
    #endif
    
    Serial.println("!!  Max water level is greater than the maximum aquarium depth. Please ensure that the maxWaterLevel is 5 cm less than aquariumHeight. !!");
    
    errorMode(0);
  }
*/
  //EEPROM.update(0, 2);
  // Check what was the last state of Autorium
  //autoriumCurrentState = EEPROM.read(0);
  autoriumCurrentState = 2;
  
  if(autoriumCurrentState==1 || autoriumCurrentState==3){
    
    // Start the extraction process
    Serial.println("Starting extraction process");
    digitalWrite(INWARD_FLOW_SOLENOID_PIN, LOW);
    digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, HIGH);
    digitalWrite(EXTRACTOR_RELAY_PIN, HIGH);
    autoriumState = 1;
    EEPROM.update(0, autoriumCurrentState);  // Store the current operation in memory for later use 
    digitalWrite(FILTER_RELAY_PIN, LOW); // Stop the filter  
     
  }
  
  if(autoriumCurrentState==2 || autoriumCurrentState==3){
    
    // Start the refill process
    Serial.println("Starting refill process");
    digitalWrite(INWARD_FLOW_SOLENOID_PIN, HIGH);
    digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);
    digitalWrite(EXTRACTOR_RELAY_PIN, LOW);
    autoriumState = 2;
    EEPROM.update(0, autoriumCurrentState);  // Store the current operation in memory for later use
       
  }


  Serial.println("Setup complete");
  
}

void loop(void) {
    
  Serial.println("");
  Serial.println("**Start of Loop**");
  
  // Get the current level of water  
  waterLevel = getWaterLevel(30);
  Serial.print("Current water level from bottom: ");
  Serial.println(waterLevel);
  
  // Check for the temperature. Not being used right now
  // temperatureCheck();

  // Check for the current state
  autoriumCurrentState = EEPROM.read(0);
  Serial.print("Autorium Current State: ");
  Serial.println(autoriumCurrentState);
/*
  if(autoriumCurrentState==1){
    
    // if water level has gone beyond min water level, stop the outlet & extraction motor, and start the inlet process
    if(waterLevel < minWaterLevel){
      Serial.println("Water has reached minimum permissiable level. Stopping extraction.");
      digitalWrite(INWARD_FLOW_SOLENOID_PIN, HIGH);
      digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);
      digitalWrite(EXTRACTOR_RELAY_PIN, LOW);
      autoriumState = 2;
      EEPROM.update(0, autoriumState);
      return 0;
    }   
  } else if(autoriumCurrentState==2){
    
    // if water level has gone beyond max water level, stop the inlet/outlet & extraction motor
    if(waterLevel > maxWaterLevel){
      Serial.println("Water has reached maximum permissiable level. Stopping all operations.");
      digitalWrite(INWARD_FLOW_SOLENOID_PIN, LOW);
      digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);
      digitalWrite(EXTRACTOR_RELAY_PIN, LOW);
      autoriumState = 0;
      EEPROM.update(0, autoriumState);
      digitalWrite(FILTER_RELAY_PIN, HIGH); // Enable the filter
      return 0;
    }
    
  }
  */

/*
  // At this level, extraction and refill should go together
  if(waterLevel < (minWaterLevel + 27)){
    digitalWrite(INWARD_FLOW_SOLENOID_PIN, HIGH);
    digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, HIGH);
    digitalWrite(EXTRACTOR_RELAY_PIN, HIGH);
    autoriumState = 2;
    return 0;
  }

  // At this level, switch off the extraction and keep on refilling
  if(waterLevel > (minWaterLevel + 27)){
    digitalWrite(INWARD_FLOW_SOLENOID_PIN, HIGH);
    digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);
    digitalWrite(EXTRACTOR_RELAY_PIN, LOW);
    autoriumState = 2;
    return 0;
  }
    
*/
  Serial.print("Inward Flow sensor value:");
  Serial.println(inwardFlowCount);

  Serial.print("Water inlet in Liters:");
  Serial.println(inwardFlowCount/480);

  Serial.print("Outward Flow sensor value:");
  Serial.println(outwardFlowCount);

  Serial.print("Water outlet in litres:");
  Serial.println(outwardFlowCount/480);

    
  Serial.println("**End of Loop**");
  Serial.println("");
}

void temperatureCheck(void){
  
  float temperatureValue;         // Stores the temperature in celcius
  
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  dtSensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for Aquarium is: ");
  temperatureValue = dtSensors.getTempCByIndex(0);      // You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.println(temperatureValue); 

  if(temperatureValue < minTemperature){
    // switch on the heater
    digitalWrite(HEATER_RELAY_PIN, HIGH);
  }
  else{
    // switch off the heater
    digitalWrite(HEATER_RELAY_PIN, LOW);    
  }
    
}

int getWaterLevel(byte repeatReadings){
  unsigned int echoTime;                            // Time difference returned by ultrasonic sensor in microseconds
  int waterDistance;                                // Distance of water from ultrasonic sensor
  int waterLevelFromBottom;                         // Water Level from bottom, calculated using aquarium height
  
  echoTime  = sonar.ping_median(repeatReadings);                 // Ultrasonic sensor can return false values. Safer to use this to get an approximate value
  waterDistance = sonar.convert_cm(echoTime);       // Convert the microseconds to centimeters
  waterLevelFromBottom = aquariumHeight - waterDistance;

  // Check if this is the first run
  if(arWaterLevel[0] == 0){
    arWaterLevel[0] = waterLevelFromBottom;
    return waterLevelFromBottom;
  }

  // Check if the water level is out of bound, if yes return the previous value
  if(abs(waterLevelFromBottom - arWaterLevel[0]) > 10){
    return arWaterLevel[0];
  }

  // move all values one step ahead starting from index 1  
  int levelCounter;
  for(levelCounter=1;levelCounter < 10; levelCounter++){
    arWaterLevel[levelCounter] = arWaterLevel[levelCounter - 1];
  }

  // Store the current value in 0th position
  arWaterLevel[0] = waterLevelFromBottom;

  for(levelCounter=1;levelCounter < 10; levelCounter++){
    waterLevelFromBottom = (arWaterLevel[levelCounter] + waterLevelFromBottom ) / 2;
  }
  
  Serial.print("Echo time from ultrasonic sensor: ");
  Serial.println(echoTime);
  Serial.print("Distance of water from ultrasonic sensor: ");
  Serial.println(waterDistance);
  Serial.print("Height of water from bottom of aquarium: ");
  Serial.println(waterLevelFromBottom);     

  #if defined(LCD_MODE)
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Water level :");
    lcd.setCursor(0,1);
    lcd.print(waterLevelFromBottom);  
  #endif

  
  return waterLevelFromBottom;  
}

void errorMode(int criticalError){
  
  NewTone(TONE_PIN, 1321, 400); // Error tone
  delay(600);
  NewTone(TONE_PIN, 1321, 400); // Error tone
  delay(600);
  NewTone(TONE_PIN, 1321, 400); // Error tone

  //Blink Red LED thrice
  digitalWrite(RED_LED_PIN, HIGH);
  delay(200);
  digitalWrite(RED_LED_PIN, LOW);
  delay(200);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(200);
  digitalWrite(RED_LED_PIN, LOW);
  delay(200);
  digitalWrite(RED_LED_PIN, HIGH);

  if(criticalError == 1){
    // Halt the execution of the code. See http://playground.arduino.cc/Learning/ArduinoSleepCode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
    cli();
    sleep_enable();
    sleep_cpu();  
  }
}

void initRelayBoard(int relayPinStart, int relayPorts){
  
  for(int iCounter=relayPinStart;iCounter < relayPinStart + relayPorts;iCounter++){

    pinMode(iCounter, OUTPUT);
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

void initLCD(int columns, int rows){
  #if defined(LCD_MODE)
    lcd.begin(columns, rows);   // initialize the lcd for 16 chars 2 lines, turn on backlight
    lcd.clear();
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
  #endif
}

void inwardFlowSensor(void){
  inwardFlowCount++;
}

// Callback routine for Outward Flow sensor
void outwardFlowSensor(void){
  outwardFlowCount++;
}

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

void calculateWaterLevel(int CurrentWaterLevel){
  
}
