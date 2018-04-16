#include <Homie.hpp>
#include <Wire.h>

#include <RGBWNode.h>
#include <SensorNode.h>
#include <InputPinArrayNode.h>
#include <LoggerNode.h>

#define FW_NAME "HNC_InputPinArrayExample"

#define FW_VERSION "1.0.0"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

RGBWNode white1("LED_W1", RGBWNode::NOPIN, RGBWNode::NOPIN, RGBWNode::NOPIN, 0);
RGBWNode white2("LED_W2", RGBWNode::NOPIN, RGBWNode::NOPIN, RGBWNode::NOPIN, 2);

std::vector<InputPinArrayNode::pin_data> vecInputs = {{12,false,false},{13, false, false}};

InputPinArrayNode::InputPinChangeEventHandler handler = [](uint8_t idx, bool state)->bool {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Input %x changed to %s", idx, state?"true":"false");
	((idx==0)?white1:white2).switchLed("w", state?100:0);
};

SensorNode sensor;
InputPinArrayNode inputs(vecInputs, handler);


void setup() {
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Serial.begin(74880);
  Serial.println("Start");
  Serial.flush();
  Wire.begin(SDA, SCL);
  Homie.disableLedFeedback();
  Homie.disableResetTrigger();
  Homie.setLoggingPrinter(&Serial);
  Homie.setup();
}


void loop() {
    Homie.loop();
}
