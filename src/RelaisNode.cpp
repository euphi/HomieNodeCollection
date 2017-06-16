/*
 * RelaisNode.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <RelaisNode.h>

#include <LoggerNode.h>
#include <Homie.hpp>

RelaisNode::RelaisNode() :
		HomieNode("Relais", "switch16"), relais_bitset(0x0000), io(0x20) {
	advertiseRange("in",1,16).settable();
}

//void RelaisNode::setup() {
//}
//
//void RelaisNode::loop() {
//}

void RelaisNode::onReadyToOperate() {
	LN.log("RelaisNode", LoggerNode::DEBUG, "Ready");
	updateRelais();
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
	LN.logf("RelaisNode::handleInput()", LoggerNode::INFO,
			"Receive command to switch %d to %s.", id, on ? "On" : "Off");

	uint16_t selected_bit = (1 << (id-1));
	if (on) {
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
	static uint16_t last = 0x00;
	LN.logf("RelaisNode::updateRelais()", LoggerNode::DEBUG, "Value: %x", relais_bitset);
	io.write16(relais_bitset);
	for (uint_fast8_t i = 0; i < 16; i++) {
		bool on = ((relais_bitset & (1 << i)) != 0);
		bool changed = on ^ (((last & (1 << i)) != 0));
		//LN.logf("UpdateRelais", LoggerNode::DEBUG, "\nRelais: %02x\n  Last: %02x\n1 << i: %02x (%d)\nOn:%s, changed:%s", relais_bitset, last, (1<<i), i, on?"true":"false", changed?"true":"false");
		if (changed || ((updateMask & (1 << i)) != 0)) {
			String value("in_");
			value.concat(i + 1);
			setProperty(value).send(on ? "ON" : "OFF");
		}
	}
	last = relais_bitset;
}
