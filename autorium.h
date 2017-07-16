#define AUTORIUM_VERSION 0.3

// LCD is available, comment out for disabling LCD code
#define LCD_MODE

// Temperature sensor is available, comment out for disbaling temperature sensor code
#define TEMPERATURE_SENSOR_MODE

// Relay port pins
#define LIGHT_RELAY_PIN 31
#define EXTRACTOR_RELAY_PIN 32
#define FILTER_RELAY_PIN 33
#define HEATER_RELAY_PIN 34
#define AIR_PUMP_RELAY_PIN 35

// LED pins
#define RED_LED_PIN 41
#define YELLOW_LED_PIN 40
#define GREEN_LED_PIN 39


// Flow sensor pins. These are attached to interrupt pin
#define INWARD_FLOW_SENSOR_PIN 2
#define OUTWARD_FLOW_SENSOR_PIN 3

// Solenoid valve pins 
#define INWARD_FLOW_SOLENOID_PIN 4
#define OUTWARD_FLOW_SOLENOID_PIN 5

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

// Define your aquarium dimensions in centimeters
int aquariumHeight = 30; //70      // Z Axis
int aquariumLength = 15; //130      // Y Axis
int aquariumWidth = 20;  //45        // X Axis
int aquariumShape = 0;         // 0 - Rectangle/Cube; 1 - Circular/Cylindrical. Can be used to calculate water volume

// Define water level in centimeters
int maxWaterLevel = 25;
int minWaterLevel = 10;

int autoriumCurrentState = 0;                 // Current action being performed by Autorium: 0 - None; 1 - Water Extract; 2 - Water Refill
int autoriumOperationMode = 0;                // Operation mode for Autrium: 0 - Simple extraction and refill;1 - Time based; 2 - Based on sensors input (Not implemented); 3 - Combination of 1 & 2 

int waterLevel  = 0;                          // Water level of aquarium in centimeters. This stores the water level from the bottom

float minTemperature;                         // Temperature at which the relay for heater will switch on
float maxTemperature;                         // Temperature at which the relay for heater will switch off

volatile int inwardFlowCount = 0;             // Count from the inward flow sensor
volatile int outwardFlowCount = 0;            // Count from the outward flow sensor

//** Function Definitions

// Sets out an audible/visible error using speaker and LED
// @Parameters    :
//  criticalError  : 0 -> Normal error just beep and blink red led || 1 -> Critical error halt code execution in addition to mode 0
void errorMode(int criticalError);

// Blinks LED's of all active relay ports on the relay board
// @Parameters    :
// relayPinStart  : First rellay pin used for the relay trigger
// relayPorts     : No of relay ports being used. The pin numbers will go in increasing order 
void initRelayBoard(int relayPinStart, int relayPorts);

// Blinks LCD thrice, and prints out the Autrium version
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
int getWaterLevel(void);

// Checks the temperature of the aquarium, and switches on or off based on min and max values
void temperatureCheck(void);



