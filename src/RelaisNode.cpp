/*
 * RelaisNode.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <RelaisNode.h>

#include <LoggerNode.h>
#include <Homie.hpp>

RelaisNode::RelaisNode(uint16_t defaults, uint16_t invert, uint16_t inputmask) :
		HomieNode("Relais", "switch16"),
		relais_bitset((defaults^invert) | inputmask),
		invert_bitset(invert),
		input_mask(inputmask),
		input_data(0x0000),
		io(0x20, false) {
	for (uint_fast8_t i = 0 ; i < 16 ; i++) {
		bool in = ((input_mask & (1 << i)) != 0);
		io.pinMode(i, in ? INPUT_PULLUP : OUTPUT);
	}
	advertiseRange("in",1,16).settable();
}

void RelaisNode::setup() {
	updateRelais(0); // write to PCF only

}
//

void RelaisNode::onReadyToOperate() {
	//LN.log("RelaisNode", LoggerNode::DEBUG, "Ready");
	updateRelais();
}

void RelaisNode::loop() {
	static uint_fast8_t count = 0;
	if (!(++count % 8)==0) return; // read I2C on every 8th cycle only
	uint16_t input = io.read16();
	uint16_t diff = ((input ^ input_data) & input_mask);
	if (diff == 0) return;
	for (uint_fast8_t i = 0 ; i<16; i++) {
		if ((diff & (1 << i)) != 0)
		{
			bool on = (input_data & (1 << i)) != 0;
			bool inverted = (invert_bitset & (1 << i)) != 0;
			LN.logf("RN::loop", LoggerNode::INFO, "Input %d changed to %c%s, new: %x, old: %x, diff: %x",
					i, inverted ? '~':' ', on ? "On" : "Off", input, input_data, diff);
			setProperty("in").setRange(i+1).send(on ^ inverted ? "ON": "OFF");
		}
	}
	input_data = input;

}
bool RelaisNode::handleInput(const String  &property, const HomieRange& range, const String &value) {
	int16_t id = range.index;
	if (id <= 0 || id > 16) {
		LN.logf("RelaisNode::handleInput()", LoggerNode::ERROR,
				"Receive unknown property %s with value %s.", property.c_str(),
				value.c_str());
		return false;
	}
	bool on = value.equalsIgnoreCase("ON");
	uint16_t selected_bit = (1 << (id-1));
	bool inverted = (invert_bitset & selected_bit) != 0;
	LN.logf("RelaisNode::handleInput()", LoggerNode::INFO,
			"Receive command to switch %d to %c%s.", id, inverted ? '~':' ', on ? "On" : "Off");

	if (on ^ inverted ) {
		relais_bitset |= selected_bit;
	} else	{
		relais_bitset &= ~selected_bit;
	}
	updateRelais(selected_bit);
	return true;
}

//
//void RelaisNode::drawFrame(OLEDDisplay& display, OLEDDisplayUiState& state,	int16_t x, int16_t y) {
//	bool blink = ((millis() >> 7) % 2) != 0;
//	display.setFont(ArialMT_Plain_16);
//	display.drawString(0+x,16,"Relais");
//	for (uint_fast8_t i=0; i<8;i++) {
//		int16_t xpos = (i*8)+4;
//		int16_t ypos = 40;
//		if (((i + 1) == encoder.state()) && blink) continue;
//		bool on = (relais_bitset & (1 << i)) != 0;
//		display.drawRect(xpos,ypos,on?6:5,on?6:5);
//		if (on) {
//			display.drawRect(xpos+1,ypos+1,4,4);
//			display.drawRect(xpos+2,ypos+2,2,2);
//		}
//	}
//	display.drawHorizontalLine(0,60,128);
//}



void RelaisNode::updateRelais(uint16_t updateMask) {
	static uint16_t last = relais_bitset;
	LN.logf("RelaisNode::updateRelais()", LoggerNode::DEBUG, "Value: %x", relais_bitset);
	io.write16(relais_bitset);
	for (uint_fast8_t i = 0; i < 16; i++) {
		bool on = ((relais_bitset & (1 << i)) != 0);
		bool changed = on ^ (((last & (1 << i)) != 0));
		//LN.logf("UpdateRelais", LoggerNode::DEBUG, "\nRelais: %02x\n  Last: %02x\n1 << i: %02x (%d)\nOn:%s, changed:%s", relais_bitset, last, (1<<i), i, on?"true":"false", changed?"true":"false");
		if (changed || ((updateMask & (1 << i)) != 0)) {
			bool inverted = (invert_bitset & (1<< i)) != 0;
			String value("in_");
			value.concat(i + 1);
			setProperty(value).send(on ^ inverted ? "ON" : "OFF");
		}
	}
	last = relais_bitset;
}
