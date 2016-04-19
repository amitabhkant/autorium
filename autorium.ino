#include "autorium.h"
#include <avr/sleep.h>
#include <Wire.h>
#include <NewPing.h>
#include <NewTone.h>

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

  // Initialze relay pins
  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(EXTRACTOR_RELAY_PIN, OUTPUT);
  pinMode(FILTER_RELAY_PIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  pinMode(AIR_PUMP_RELAY_PIN, OUTPUT);

  initRelayBoard(31, 5);      // Led blink test on all active ports

  // Intialize all motors/relays to safe values
  digitalWrite(LIGHT_RELAY_PIN, HIGH);             // Set the light relay to high state (Switch on)
  digitalWrite(EXTRACTOR_RELAY_PIN, LOW);          // Set the extractor motor relay to low state (Switch off)
  digitalWrite(FILTER_RELAY_PIN, LOW);             // Set the filter relay to low state (Switch off)
  digitalWrite(HEATER_RELAY_PIN, LOW);             // Set the heater relay to low state (Switch off)
  digitalWrite(AIR_PUMP_RELAY_PIN, HIGH);           // Set the Air Pump relay to high state (Switch on)

  // Initialize solenoid pins
  pinMode(INWARD_FLOW_SOLENOID_PIN, OUTPUT);
  pinMode(OUTWARD_FLOW_SOLENOID_PIN, OUTPUT);

  // Initialize Ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Intialize LED Pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  // Close all solenoid valves
  digitalWrite(INWARD_FLOW_SOLENOID_PIN, LOW);          // Set the in flow solenoid valve to closed (No water flow)
  digitalWrite(OUTWARD_FLOW_SOLENOID_PIN, LOW);         // Set the out flow solenoid valve to closed (No water flow)

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

  waterLevel = getWaterLevel();    // Get the intial water level

  // Max water level should be 5 cm less than Aquarium height
  if(maxWaterLevel > aquariumHeight - 5){
    
    #if defined(LCD_MODE)
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Max wtr level >");
      lcd.setCursor(0,1);
      lcd.print("Aquarium height");
    #endif
    
    Serial.println("!! => Max water level is greater than the maximum aquarium depth. Please ensure that the maxWaterLevel is 5 cm less than aquariumHeight");
    
    errorMode(1);
  }
}

void loop(void) {
  // STEPS:
  
  // c) Get the current temperature
  // d) Set alarm for next run

  // Get the current level of water  
  waterLevel = getWaterLevel();

  // Check for the temperature.
  temperatureCheck();

  if(autoriumOperationMode=0){
    // 
  }
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

int getWaterLevel(void){
  unsigned int echoTime;                            // Time difference returned by ultrasonic sensor in microseconds
  int waterDistance;                                // Distance of water from ultrasonic sensor
  int waterLevelFromBottom;                         // Water Level from bottom, calculated using aquarium height
  
  echoTime  = sonar.ping_median(5);                 // Ultrasonic sensor can return false values. Safer to use this to get an approximate value
  waterDistance = sonar.convert_cm(echoTime);       // Convert the microseconds to centimeters
  waterLevelFromBottom = aquariumHeight - waterDistance;

  Serial.print("Echo time from ultrsonic sensor -> ");
  Serial.println(echoTime);
  Serial.print("Distance of water from ultrasonic sensor -> ");
  Serial.println(waterDistance);
  Serial.print("Height of water from bottom of aquarium -> ");
  Serial.println(waterLevelFromBottom);     

  #if defined(LCD_MODE)
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Water level >");
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

// ** RTC Clock functions start
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

//Set the RTC clock time
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

// Get the RTC clock time
void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

// Format the time retrieved from RTC clock
void displayTime(void){
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  #if defined(LCD_MODE)
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(dayOfMonth);
    lcd.print("-");
    lcd.print(month);
    lcd.print("-");
    lcd.print(year);
    lcd.print(" ");
    lcd.print(hour);
    lcd.print(":");
    lcd.print(minute);
    lcd.setCursor(0,1);
    switch(dayOfWeek){
      case 1:
        lcd.print("Sunday");
        break;
      case 2:
        lcd.print("Monday");
        break;
      case 3:
        lcd.print("Tuesday");
        break;
      case 4:
        lcd.print("Wednesday");
        break;
      case 5:
        lcd.print("Thursday");
        break;
      case 6:
        lcd.print("Friday");
        break;
      case 7:
        lcd.print("Saturday");
        break;
    }
  #endif

  // send it to the serial monitor. convert the byte variable to a decimal number when displayed
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
    case 1:
      Serial.println("Sunday");
      break;
    case 2:
      Serial.println("Monday");
      break;
    case 3:
      Serial.println("Tuesday");
      break;
    case 4:
      Serial.println("Wednesday");
      break;
    case 5:
      Serial.println("Thursday");
      break;
    case 6:
      Serial.println("Friday");
      break;
    case 7:
      Serial.println("Saturday");
      break;
  }
}
// ** RTC Clock functions end


