 #define AUTORIUM_VERSION 0.1

// Define development mode; comment out for disabling debug messages on serial monitor
#define devMode

// Define all the relay ports
#define relayLight 31
#define relayExtractor 32
#define relayFilter 33
#define relayHeater 34
#define relayAirPump 35

// Define the flow sensor pins. These are attached to interrupt pin
#define inFlowSensor 2
#define outFlowSensor 3

// Define Solenoid valve pins 
#define inFlowSolenoid 4
#define outFlowSolenoid 5

// Define I2C addresses
#define i2cLCD 0x27

// Define your aquarium dimensions in centimeters
#define aquariumHeight 100      // Z Axis
#define aquariumLength 150      // Y Axis
#define aquariumWidth 50        // X Axis
#define aquariumShape 0         // 0 - Rectangle/Cube; 1 - Circular/Cylindrical 

// Define water level in centimeters
#define maxWaterLevel 90
#define minWaterLevel 15

#define TONE_PIN 8 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).
