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

// Gamma correction, for details see https://learn.adafruit.com/led-tricks-gamma-correction/
// This table maps [0%-100%] to [0-1024] (PWMRANGE of ESP8266's arduino.h)
// See tools directory for C++ program to create table for different ranges

const uint16_t /*PROGMEM*/ RGBWNode::gamma8[] = {
  0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,5,
  6,7,8,10,11,13,15,17,19,21,24,26,29,32,35,39,
  42,46,50,54,59,63,68,73,79,84,90,96,103,109,116,124,
  131,139,147,155,164,173,182,192,202,212,223,234,245,257,269,281,
  293,307,320,334,348,362,377,392,408,424,441,458,475,493,511,529,
  548,568,587,608,628,650,671,693,716,739,762,786,811,836,861,887,
  913,940,968,996,1024 };


HomieSetting<long> RGBWNode::fadeDelay ("RGBfadeDelay", "fade delay");  // id, description
bool RGBWNode::settingsInitialized(false);

RGBWNode::RGBWNode(const char* name, char redpin, char greenpin, char bluepin, char whitepin) :
		HomieNode(name, "RGBW-LED PWM Output", "RGBW"),
		rgbw_pins{redpin, greenpin, bluepin, whitepin},
	    initialized(false),
		fade_active(true)
{
	// fadeDelay is static, so we need to take care to initialize it only once
	if (!settingsInitialized) {
		settingsInitialized = true;
		fadeDelay.setDefaultValue(5).setValidator([] (long candidate) {
			return (candidate > 0 && candidate < 200);
		});
	}
	if (redpin   != NOPIN) advertise("r").settable();
	if (bluepin  != NOPIN) advertise("g").settable();
	if (greenpin != NOPIN) advertise("b").settable();
	if (whitepin != NOPIN) advertise("w").settable();
	if (redpin != NOPIN && bluepin != NOPIN && greenpin != NOPIN) advertise("rgb").settable();
}


bool RGBWNode::handleInput(const HomieRange& range, const String& property, const String& value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received  property %s (value=%s).", property.c_str(), value.c_str());
	if (property.equals("rgb")) {
		uint_fast8_t r_end = value.indexOf(',');
		uint_fast8_t g_end = value.lastIndexOf(',');
		int new_r = value.substring(0, r_end).toInt() * 100 / 255;
		int new_g = value.substring(r_end+1, g_end).toInt() * 100 / 255;
		int new_b = value.substring(g_end+1).toInt() * 100 / 255;
		if (new_r >= 0 && new_r <= 100) rgbw_values[R] = new_r;
		if (new_g >= 0 && new_g <= 100) rgbw_values[G] = new_g;
		if (new_b >= 0 && new_b <= 100) rgbw_values[B] = new_b;
		updateLEDs();
	} else {
		int value_int = value.toInt();
		uint_fast8_t id = R;
		for (id=R; id <= W; id++)
			if (property[0] == rgbw_id[id]) break;
		if ((id > W) || (value_int < 0) || (value_int > 100)) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR,
					"Received value %d or property %s out of range", value_int, property.c_str());
			return false;
		}
		rgbw_values[id] = value_int;
		updateLED(id);
		PublishState(id);
	}
	fade_active = true;
	return true;
}

void RGBWNode::switchLed(const String &property, uint8_t value) {
	uint_fast8_t id = R;
	for (id=R; id <= W; id++)
		if (property[0] == rgbw_id[id]) break;
	if ((id > W) || (value > 100)) {
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR,
				"Internal property %s out of range", property.c_str());
		return;
	}
	rgbw_values[id] = value;
	updateLED(id);
	PublishState(id);
	fade_active = true;

}

void RGBWNode::loop() {
	static unsigned long last = 0;
	
	if (fade_active && ((last + fadeDelay.get()) < millis()) ) {
		last = millis();
		fadeLEDs();
	}
}

void RGBWNode::fadeLEDs() {
	bool changed = false;
	for (uint_fast8_t id = R;id <= W; id++) {
		if (rgbw_values[id] != rgbw_cur_values[id]) {
			changed = true;
			if (rgbw_values[id] > rgbw_cur_values[id]) {
				rgbw_cur_values[id]++;
			} else {
				rgbw_cur_values[id]--;
			}
			updateLED(id);
		}
	}
	fade_active = changed;
}

void RGBWNode::updateLEDs() const {
	for (uint_fast8_t i=0;i<4;i++) {
		updateLED(i);
		PublishState(i);
	}
}

void RGBWNode::updateLED(uint8_t id) const {
	if (id < R || id > W) return;
	uint16_t value = rgbw_cur_values[id];
	if (value > 100) {
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Invalid color value %d for id %d", value, id);
		return;
	}
	uint16_t value_gamma = gamma8[value];
	uint8_t pin=rgbw_pins[id];
	if (pin == NOPIN) {
		//LN.logf(__PRETTY_FUNCTION__, LoggerNode::ERROR, "Tried to set invalid pin");
		return; // PIN "255" reserved for UNUSED
	}
	analogWrite(pin, value_gamma);
	if (rgbw_values[id] == value && LN.loglevel(LoggerNode::INFO)) {
		LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Updated LED %c on Pin %d, value %d%% (gamma-corrected %d).", rgbw_id[id], pin, value, value_gamma);
	}
}

void RGBWNode::PublishState(uint8_t id) const {
	if (id < R || id > W) return;
	if (rgbw_pins[id] == NOPIN) return;
	const String id_string(rgbw_id[id]);
	const String value_string((uint16_t) round((float)rgbw_values[id]));
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
	LN.logf("RGBWNode/Settings", LoggerNode::INFO, "RGBfadedelay: %d", fadeDelay.get());
	delay(200);
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
