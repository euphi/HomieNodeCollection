#include <Homie.hpp>
#include <Wire.h>

#include <RGBWNode.h>
#include <SensorNode.h>
#include <InputPinArrayNode.h>
#include <LoggerNode.h>

#define FW_NAME "HNC_InputPinArrayExample"

#define FW_VERSION "1.0.0"

// Two white  only LED strips on GPIO 0 and 2
RGBWNode white1("LED_W1", RGBWNode::NOPIN, RGBWNode::NOPIN, RGBWNode::NOPIN, 0);
RGBWNode white2("LED_W2", RGBWNode::NOPIN, RGBWNode::NOPIN, RGBWNode::NOPIN, 2);

//Two inputs on GPIO 12 and 13 (with "false" ( = 0) as CLOSED state)
std::vector<InputPinArrayNode::pin_data> vecInputs = {{12,false,false},{13, false, false}};

// Callback-method for InputPinArrayNode
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
