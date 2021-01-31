#define AUTORIUM_VERSION 3.0
#define ultrasonicMaxDistance 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Version 3.0 - First working prototype of the aquarium automation. 


//** Variable defintions **//

// TODO: Pin numbers are temporary. Need to set the right pin numbers

// Pin number for lights / 220 V AC / Solid State Relay
byte arLightRelayPin[] = {1, 2, 3};

// Pin number for air pumps  / 220 V AC / Solid State Relay
byte arAirPumpRelayPin[] = {4, 5};


//Pin number for heaters  / 220 V AC / Solid State Relay
byte arHeaterRelayPin[] = {6, 7, 8};

// Pin number for solenoid valves (NO - Normally Open; NC - Normally Closed)  / 12 V DC / Mechanical Relay
byte arNCSolenoidRelayPin[] = {9, 10, 11, 12};

// Pin assignments for the solenoid pins in the array should be fixed
// arNCSolenoidRelayPin[0] -> NC - From Aquarium to Drain
// arNCSolenoidRelayPin[1] -> NC - From Filter to Drain
// arNCSolenoidRelayPin[2] -> NC - Freshwater to Aquarium
// arNCSolenoidRelayPin[3] -> NC - Freshwater to Filter 

byte arNOSolenoidRelayPin[] = {13, 14};
// arNCSolenoidRelayPin[0] -> NO - From Aquarium to Filter
// arNCSolenoidRelayPin[1] -> NO - From Filter to Aquarium 


// Pin number for flow sensors. Must be interrupt pins. Array index follows solenoid valves array index
byte arFlowSensorPin[] = {15, 16, 17, 18, 19, 20};

// Pin number for aquarium ultrasonic sensor
byte aqUltrasonicSensorTriggerPin = 20;
byte aqUltrasonicSensorEchoPin = 21;

// Pin number for filter float sensor
byte flFloatSensorHighPin = 22;
byte flFloatSensorLowPin = 22;

// Pin number for filter overflow float sensor
byte flOverFlowFloatSensorHighPin = 22;
byte flOverFlowFloatSensorLowPin = 22;

// Pin number for water pump in aquarium/ 220 V/ Solid State Relay
byte aqPumpRelayPin = 23;

// Pin number for water pump in filter/ 220 V/ Solid State Relay
byte flPumpRelayPin = 24;

// Pin number for water pump in overflow section of filter/ 220 V/ Solid State Relay
byte flOverFlowPumpRelayPin = 24;

//Pin number for temperature sensor
byte arTemperatureSensorPin[] = {6,7,8};


// Aquarium dimensions in centimeters
// Calculations are based on this page: https://www.calculatorsoup.com/calculators/construction/tank.php
int aqHeight = 91; 
int aqLength = 183; 
int aqWidth = 61;
int aqFilledDepth = 85;
byte aqShape = 0;           // 0 - Cuboid/Cube; 1 - Cylindrical. Can be used to calculate water volume

byte autoriumState = 0;     // Stores the current action being performed in the Autorium:
                            // 0 -> No action
                            // 1 -> Standard filter operation
                            // 2 -> Aquarium water drain
                            // 3 -> Aquarium fresh water filling
                            // 4 -> Filter water drain
                            // 5 -> Filter fresh water filling




int arWaterLevel[10];       // Store last 10 valaue from the water level. Discard out of range values


byte autoriumCurrentState = 0;                 // Current action being performed by Autorium: 0 - None; 1 - Water Extract; 2 - Water Refill; 3 - Both
byte autoriumOperationMode = 0;                // Operation mode for Autrium: 0 - Simple extraction and refill;1 - Time based; 2 - Based on sensors input (Not implemented); 3 - Combination of 1 & 2 
//byte autoriumState = 0;                        // This value is being stored in EEPROM. Values same as autoriumCurrentState



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


// Read all the temperature sensors

// Initialize all flow sensor pins as interrupt pins. This is to be done individually for every flow sensor pin as this would require individual call back functions. Best to move it to setup directly
// attachInterrupt(digitalPinToInterrupt(2), pin_ISR, CHANGE);


// All soelnoid valve to their normal state (wihtout power being passed on )
// If closeNoValves is set to 1, Normally open valves will also be powered on (will be closed)
void resetAllPins(int closeNOValves = 0);

// Drain water from the aquarium. Close all solenoid valves apart from Aquarium->Drain one
void drainAquarium();

// Drain water from the filter. Close all solenoid valves apart from Aquarium->Drain one
void drainFilter();

// Fill fresh water in the aquarium
void fillAquarium();

// Fill fresh waster in the filter
void fillFilter();

// Recycle the aquarium water through the filter. This is the main routine that needs to be running most of the time
void operateFilter();

// *********************




// I2C scanner for debugging
void i2cScanner(void);
