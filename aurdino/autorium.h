#define AUTORIUM_VERSION 3.0

// 3.0 First working prototype of the aquarium automation. This verion uses 6 solenoid valves and 6 flow sensors


// LCD is available, comment out for disabling LCD code
//#define LCD_MODE

// Temperature sensor is available, comment out for disbaling temperature sensor code
#define TEMPERATURE_SENSOR_MODE

// Relay port pins
#define LIGHT_RELAY_PIN 31
#define FILTER_RELAY_PIN 33
#define HEATER_RELAY_PIN 34
#define AIR_PUMP_RELAY_PIN 35
#define INWARD_FLOW_SOLENOID_PIN 37
#define OUTWARD_FLOW_SOLENOID_PIN 36
#define EXTRACTOR_RELAY_PIN 38

// LED pins
#define RED_LED_PIN 41
#define YELLOW_LED_PIN 40
#define GREEN_LED_PIN 39


// Flow sensor pins. These are attached to interrupt pin
#define INWARD_FLOW_SENSOR_PIN 3
#define OUTWARD_FLOW_SENSOR_PIN 2


// I2C addresses
#define LCD_I2C_ADDRESS 0x27
#define RTC_I2C_ADDRESS 0x68
#define EEPROM_I2C_ADDRESS 0x57

#define TONE_PIN 8 // Pin speaker/piezo is connected (Include a 100 ohm resistor).

#define ULTRASONIC_TRIGGER_PIN  24  // Pin tied to trigger pin on the ultrasonic sensor.
#define ULTRASONIC_ECHO_PIN     25  // Pin tied to echo pin on the ultrasonic sensor.

#define TEMPERATURE_SENSOR_PIN  23  // Pin for temperature sensor

//** Any definitions below this point might be changed into varaibles later, so they are following camel case notation **//

#define ultrasonicMaxDistance 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//** Variable defintions **//

// *********************
// Variable definitions for version 3.0 starts
// Pin numbers are temporary. Need to set the right pin numbers

// Pin number for lights / 220 V AC / Solid State Relay
int arLightRelayPin[0] = 1;
int arLightRelayPin[1] = 2;
int arLightRelayPin[2] = 3;

// Pin number for air pumps  / 220 V AC / Solid State Relay
int arAirPumpRelayPin[0] = 4;
int arAirPumpRelayPin[1] = 5;

//Pin number for heaters  / 220 V AC / Solid State Relay
int arHeaterRelayPin[0] = 6;
int arHeaterRelayPin[1] = 7;
int arHeaterRelayPin[2] = 7;

// Pin number for solenoid valves (NO - Normally Open; NC - Normally Closed)  / 12 V DC / Mechanical Relay
int arSolenoidRelayPin[0] = 8;    // NC - From Aquarium to Filter
int arSolenoidRelayPin[1] = 9;    // NC - From Aquarium to Drain
int arSolenoidRelayPin[2] = 10;   // NC - From Filter to Aquarium
int arSolenoidRelayPin[3] = 11;   // NC - From Filter to Drain
int arSolenoidRelayPin[4] = 12;   // NC - Freshwater to Aquarium
int arSolenoidRelayPin[5] = 13;   // NC - Freshwater to Filter

// Pin number for flow sensors. Must be interrupt pins. Array index follows solenoid valves array index
int arFlowSensorPin[0] = 14;
int arFlowSensorPin[1] = 15;
int arFlowSensorPin[2] = 16;
int arFlowSensorPin[3] = 17;
int arFlowSensorPin[4] = 18;
int arFlowSensorPin[5] = 19;

// Pin number for aquarium ultrasonic sensor
int aqUSTriggerPin = 20;
int aqUSEchoPin = 21;

// Pin number for filter float sensor
int flFloatSensorPin = 22;

// Pin number for water pump in aquarium / 220 V/ Solid State Relay
int aqPumpRelayPin = 23;

// Pin number for water pump in filter / 220 V/ Solid State Relay
int flPumpRelayPin = 24;

//Pin number for temperature sensor
int arTemperatureSensorPin[0] = 6;
int arTemperatureSensorPin[1] = 7;
int arTemperatureSensorPin[2] = 7;




// Variable definitions for version 3.0 ends
// *********************




// Define your aquarium dimensions in centimeters
int aquariumHeight = 63; //70      // Z Axis
int aquariumLength = 135; //130      // Y Axis
int aquariumWidth = 43;  //45        // X Axis
int aquariumShape = 0;         // 0 - Rectangle/Cube; 1 - Circular/Cylindrical. Can be used to calculate water volume

// Define water level in centimeters from bottom
int maxWaterLevel = 54;
int minWaterLevel = 10;


int arWaterLevel[10];       // Store last 10 valaue from the water level. Discard out of range values


byte autoriumCurrentState = 0;                 // Current action being performed by Autorium: 0 - None; 1 - Water Extract; 2 - Water Refill; 3 - Both
byte autoriumOperationMode = 0;                // Operation mode for Autrium: 0 - Simple extraction and refill;1 - Time based; 2 - Based on sensors input (Not implemented); 3 - Combination of 1 & 2 
byte autoriumState = 0;                        // This value is being stored in EEPROM. Values same as autoriumCurrentState



int waterLevel  = 0;                          // Water level of aquarium in centimeters. This stores the water level from the bottom

float minTemperature;                         // Temperature at which the relay for heater will switch on
float maxTemperature;                         // Temperature at which the relay for heater will switch off

//volatile int inwardFlowCount = 0;             // Count from the inward flow sensor
//volatile int outwardFlowCount = 0;            // Count from the outward flow sensor

//volatile unsigned int inwardFlowCount = 0;             // Count from the inward flow sensor
//volatile unsigned int outwardFlowCount = 0;            // Count from the outward flow sensor

volatile long inwardFlowCount = 0L;             // Count from the inward flow sensor
volatile long outwardFlowCount = 0L;            // Count from the outward flow sensor




//** Function Definitions


// *********************
// Function definitions for version 3.0 starts

// Read all the temperatrure sensors

// Initialize all flow sensor pins as interrupt pins
// attachInterrupt(digitalPinToInterrupt(2), pin_ISR, CHANGE);

// Function definitions for version 3.0 ends
// *********************


// Sets out an audible/visible error using speaker and LED
// @Parameters    :
//  criticalError  : 0 -> Normal error just beep and blink red led || 1 -> Critical error halt code execution in addition to mode 0
void errorMode(int criticalError);

// TODO: This function should not be used. Might create problems with waterflow
// Blinks LED's of all active relay ports on the relay board
// @Parameters    :
// relayPinStart  : First relay pin used for the relay triggers
// relayPorts     : No of relay ports being used. The pin numbers will go in increasing order 
void initRelayBoard(int relayPinStart, int relayPorts);

// Blinks LCD thrice, and prints out the Autorium version
// @Parameters    :
// columns        : No of columns in the lcd 
// rows           : No of rows in the lcd
void initLCD(int columns, int rows);

// Callback routine for Inward Flow sensor
void inwardFlowSensor(void);

// Callback routine for Outward Flow sensor
void outwardFlowSensor(void);

// I2C scanner for debugging
void i2cScanner(void);

// Gets the water level of the aquarium in centimeters. 
// @Return : Water level in centimeres. Uses the aquarium height parameter and the distance returned by ultrasonic sensor to calculate the water level
int getWaterLevel(byte repeatReadings = 15);

// Checks the temperature of the aquarium, and switches on or off based on min and max values
void temperatureCheck(void);
