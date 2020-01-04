#include <Homie.hpp>                                                                                                                                                                                                 

// Includes for HomieNodeCollection includes
#include <RGBWNode.h>                                                                                                                                                                                                
#include <SensorNode.h>                                                                                                                                                                                              
#include <LoggerNode.h>

#define FW_NAME "HNC_pio_Example"
#define FW_VERSION "1.0.0"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */


// RGB strip connected to Pin 2, 15, 0 and White LED strip connected to 16
RGBWNode rgb("LED", 2, 15, 0, 16);

// HTU21 Sensor
SensorNode sensor;

LoggerNode LN;

void setup() {
        Homie.disableResetTrigger();
        
	// Initialize Serial
	Serial.begin(74880);
        Serial.println("Starting..");
        
        // Initialize I2C
	Wire.begin(SDA, SCL);
        Wire.setClockStretchLimit(2000);

        // A LoggerNode instance named "LN" is automatically instantiated in HomieNodeCollection.
	LN.setLoglevel(LoggerNode::DEBUG);

        Homie_setFirmware(FW_NAME, FW_VERSION);

	// Homie automatically calls setup() of any object derived from HomieNode
        Homie.setup();
}

void loop() {
	// Homie automatically calls loop() of any object derived from HomieNode
        Homie.loop();
}


