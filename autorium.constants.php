<?php
/*
Purpose: This file contains the various constants defined within the Autorium project.
*/

// Autorium Version
define('AUTORIUM_VERSION', 0.4);          

// LCD availability, set to 0 for disabling LCD display code
define('LCD_MODE',1);

// Temperature sensor availability, set to 0 for disabling Temperature Sensor code
define('TEMPERATURE_SENSOR_MODE',1);

//Set Relay port pins
define('LIGHT_RELAY_PIN',31);
define('EXTRACTOR_RELAY_PIN',32);
define('FILTER_RELAY_PIN',33);
define('HEATER_RELAY_PIN',34);
define('AIR_PUMP_RELAY_PIN',35);


// Flow sensor pins. These should attached to interrupt pin
define('INWARD_FLOW_SENSOR_PIN',2);
define('OUTWARD_FLOW_SENSOR_PIN',3);

// Solenoid valve pins 
define('INWARD_FLOW_SOLENOID_PIN',4);
define('OUTWARD_FLOW_SOLENOID_PIN',5);

// I2C addresses
define('LCD_I2C_ADDRESS',0x27);
define('RTC_I2C_ADDRESS',0x68);
define('EEPROM_I2C_ADDRESS',0x57);

// Pin speaker/piezo is connected to (Include a 100 ohm resistor).
define('TONE_PIN',8); 

// Ultrasonic sensor pins
define('ULTRASONIC_TRIGGER_PIN',24);    # Pin tied to trigger pin on the ultrasonic sensor.
define('ULTRASONIC_ECHO_PIN',25);       # Pin tied to echo pin on the ultrasonic sensor.

// Temperature sensor Pin
define('TEMPERATURE_SENSOR_PIN',23);  

// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
define('ultrasonicMaxDistance',400);

// Variable defintions which will be over-ridden from local db values post setup/inialization

$autoriumSetup = 0;         # If there is any problem in the setup, or some essential DB files are missing, 
                            # this variable will be set to 1, which will lead to setup/intialisation script
                            # being called.

// Aquarium dimensions in centimeters
$aquariumShape = 0;         # 0 - Rectangle/Cube; 1 - Circular/Cylindrical. For calculating water volume
$aquariumWidth = 20;        # X Axis [45]
$aquariumLength = 15;       # Y Axis [130]
$aquariumHeight = 30;       # Z Axis [70]
$aquariumDiameter = 0;      # Diameter in case of Circular/Cylindrical shape

$maxWaterLevel = 25;         # Max Water level in centimeters
$minWaterLevel = 10;         # Min Water level in centimeters

$autoriumCurrentState = 0;   # Current action being performed by Autorium: 0 - None; 1 - Water Extract; 2 - Water Refill
$autoriumOperationMode = 0;  # Operation mode for Autrium: 0 - Simple extraction and refill;1 - Time based; 2 - Based on sensors input (Not implemented); 3 - Combination of 1 & 2 

$waterLevel  = 0;            # Required Water level of aquarium in centimeters. This stores the water level from the bottom

$minTemperature = 20;        # Temperature in degree celcius at which the relay for heater will switch on
$maxTemperature = 26;        # Temperature in degree celcius at which the relay for heater will switch off

$inwardFlowCount = 0;        # Count from the inward flow sensor
$outwardFlowCount = 0;       # Count from the outward flow sensor

?>