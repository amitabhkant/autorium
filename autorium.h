 #define AUTORIUM_VERSION 0.2

// Define development mode; comment out for disabling debug messages on serial monitor
#define DEV_MODE

// Define all the relay ports
#define LIGHT_RELAY_PIN 31
#define EXTRACTOR_RELAY_PIN 32
#define FILTER_RELAY_PIN 33
#define HEATER_RELAY_PIN 34
#define AIR_PUMP_RELAY_PIN 35

// Define the flow sensor pins. These are attached to interrupt pin
#define INWARD_FLOW_SENSOR_PIN 18
#define OUTWARD_FLOW_SENSOR_PIN 19

// Define Solenoid valve pins 
#define INWARD_FLOW_SOLENOID_PIN 4
#define OUTWARD_FLOW_SOLENOID_PIN 5

// Define I2C addresses
#define LCD_I2C_ADDRESS 0x27
#define RTC_I2C_ADDRESS 0x68
#define EEPROM_I2C_ADDRESS 0x57

#define TONE_PIN 8 // Pin speaker/piezo is connected (Include a 100 ohm resistor).

#define TRIGGER_PIN  12  // Pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Any definitions below this point might be changed into varaibles later, so they are following camel case notation

// Define your aquarium dimensions in centimeters
#define aquariumHeight 70      // Z Axis
#define aquariumLength 130      // Y Axis
#define aquariumWidth 45        // X Axis
#define aquariumShape 0         // 0 - Rectangle/Cube; 1 - Circular/Cylindrical 

// Define water level in centimeters
#define maxWaterLevel 60
#define minWaterLevel 10


