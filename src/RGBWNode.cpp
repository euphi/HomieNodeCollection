/*
 * RGBWNode.cpp
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#include "RGBWNode.h"
#include "Homie.hpp"
#include "LoggerNode.h"
#include "Arduino.h"

const float RGBWNode::percent_to_pwm = PWMRANGE / 100;

RGBWNode::RGBWNode() : 	HomieNode("LED", "RGBW"), initialized(false) {
//	Serial.begin(115200);
//	for (uint_fast8_t i=R;i<=W;i++) {
//		char cb[2];
//		cb[0] = rgbw_id[i];
//		cb[1] = '\0';
//		this->advertise(cb)->settable();
//		Serial.printf("Registered RGWNode property no. %d to %s.\n", i, cb);
//	}
//	Serial.flush();
	advertise("r").settable();
	advertise("g").settable();
	advertise("b").settable();
	advertise("w").settable();


}


bool RGBWNode::handleInput(const String& property, const HomieRange& range, const String& value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received  property %s (value=%s).", property.c_str(), value.c_str());
	int value_int = value.toInt();
	uint_fast8_t id = R;
	for (id=R; id <= W; id++)
		if (property[0] == rgbw_id[id]) break;
	if (id>W) return false;

	rgbw_values[id] = round((float) value_int * percent_to_pwm);
	updateLED(id);
	return true;
}

void RGBWNode::updateLEDs() const {
	for (uint_fast8_t i=0;i<4;i++) updateLED(i);
}

void RGBWNode::updateLED(uint8_t id) const {
	if (id < R || id > W) return;
	uint16_t value = rgbw_values[id];
	uint8_t pin=rgbw_pins[id];
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Update LED %c on Pin %d, value %d.", rgbw_id[id], pin, value);
	analogWrite(pin, value);
	PublishState(id);
}

void RGBWNode::PublishState(uint8_t id) const {
	if (id < R || id > W) return;
	const String id_string(rgbw_id[id]);
	const String value_string((uint16_t) round((float)rgbw_values[id]/percent_to_pwm));
	setProperty(id_string).send(value_string);
}

void RGBWNode::setup() {
	for (uint_fast8_t i=0;i<4;i++)
	{
		pinMode(rgbw_pins[i], OUTPUT);
	}
}

void RGBWNode::onReadyToOperate() {
	initialized = true;
	LN.log("RGBWNode", LoggerNode::DEBUG, __PRETTY_FUNCTION__);
	updateLEDs();
}

void RGBWNode::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& state, int16_t x, int16_t y) {
	display.setFont(ArialMT_Plain_10);
	bool blink = (millis() >> 8) % 2;

	for (uint_fast8_t i=0;i<=W;i++) {
		if (blink || i != 0) {
			String color(rgbw_id[i]);
			color.concat(':');
			display.drawString(0 + x, 16 + y + (i * 12), color);
		}
	    display.drawProgressBar(25+x,16+y+(i*12),100,10,rgbw_values[i] / 10);
	}
}

void RGBWNode::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
	display.setFont(ArialMT_Plain_16);
	display.drawString(25,0,"RGBW Ctrl");
}
