#define AUTORIUM_VERSION 3.0

// 3.0 First working prototype of the aquarium automation. 


// LED pins
#define RED_LED_PIN 41
#define YELLOW_LED_PIN 40
#define GREEN_LED_PIN 39

// I2C addresses
#define LCD_I2C_ADDRESS 0x27
#define RTC_I2C_ADDRESS 0x68
#define EEPROM_I2C_ADDRESS 0x57

#define TONE_PIN 8 // Pin speaker/piezo is connected (Include a 100 ohm resistor).

//** Any definitions below this point might be changed into varaibles later, so they are following camel case notation **//

#define ultrasonicMaxDistance 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//** Variable defintions **//

// *********************
// Variable definitions for version 3.0 starts
// Pin numbers are temporary. Need to set the right pin numbers

// Pin number for lights / 220 V AC / Solid State Relay
int arLightRelayPin[] = {1, 2, 3};

// Pin number for air pumps  / 220 V AC / Solid State Relay
int arAirPumpRelayPin[] = {4, 5};


//Pin number for heaters  / 220 V AC / Solid State Relay
int arHeaterRelayPin[] = {6, 7, 8};

// Pin number for solenoid valves (NO - Normally Open; NC - Normally Closed)  / 12 V DC / Mechanical Relay
int arSolenoidRelayPin[] = {9, 10, 11, 12, 13, 14};
// 0 - NC - From Aquarium to Filter // NC - From Aquarium to Drain // NC - From Filter to Aquarium // NC - From Filter to Aquarium // NC - From Filter to Drain // NC - Freshwater to Aquarium // NC - Freshwater to Filter

// Pin number for flow sensors. Must be interrupt pins. Array index follows solenoid valves array index
int arFlowSensorPin[] = {15, 16, 17, 18, 19, 20};

// Pin number for aquarium ultrasonic sensor
int aqUltrasonicSensorTriggerPin = 20;
int aqUltrasonicSensorEchoPin = 21;

// Pin number for filter float sensor
int flFloatSensorPin = 22;

// Pin number for water pump in aquarium / 220 V/ Solid State Relay
int aqPumpRelayPin = 23;

// Pin number for water pump in filter / 220 V/ Solid State Relay
int flPumpRelayPin = 24;

//Pin number for temperature sensor
int arTemperatureSensorPin[] = {6,7,8};




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




// I2C scanner for debugging
void i2cScanner(void);
