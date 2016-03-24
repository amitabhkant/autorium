 #define AUTORIUM_VERSION 0.1

// Define development mode; set to 1 for disabling debug messages on serial monitor
#define devMode 0

// Define all the relay ports
#define relayLight 31
#define relayExtractorMotor 32
#define relayFilter 33
#define relayHeater 34

// Define the flow sensor pins. These are attached to interrupt pin
#define inFlowSensor 2
#define outFlowSensor 3

// Define Solenoid valve pins 
#define inFlowSolenoid 4
#define outFlowSolenoid 5

// Define I2C addresses
#define i2cLCD 68
