 #define AUTORIUM_VERSION 0.2

// Development mode; comment out for disabling debug messages on serial monitor
#define DEV_MODE

// LCD is available, comment out for disabling LCD code
#define LCD_MODE

// Relay ports
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
#define INWARD_FLOW_SENSOR_PIN 18
#define OUTWARD_FLOW_SENSOR_PIN 19

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

// ** Any definitions below this point might be changed into varaibles later, so they are following camel case notation **//

#define ultrasonicMaxDistance 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Define your aquarium dimensions in centimeters
#define aquariumHeight 70      // Z Axis
#define aquariumLength 130      // Y Axis
#define aquariumWidth 45        // X Axis
#define aquariumShape 0         // 0 - Rectangle/Cube; 1 - Circular/Cylindrical 

// Define water level in centimeters
#define maxWaterLevel 60
#define minWaterLevel 10


