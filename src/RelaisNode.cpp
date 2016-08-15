/*
 * RelaisNode.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <RelaisNode.h>

#include <LoggerNode.h>

RelaisNode::RelaisNode() :
		HomieNode("Relais", "switch8"), relais_bitset(0x55), io(0x20) {
}

void RelaisNode::setup() {
}

void RelaisNode::loop() {
}

void RelaisNode::onReadyToOperate() {
	LN.log("RelaisNode", LoggerNode::DEBUG, "Ready");
	updateRelais();
}

bool RelaisNode::handleInput(const String& property, const String& value) {
	int16_t id = property.toInt();
	if (id < 0 || id >= 8) {
		LN.logf("RelaisNode::handleInput()", LoggerNode::ERROR,
				"Receive unknown property %s with value %s.", property.c_str(),
				value.c_str());
		return false;
	}
	bool on = value.equalsIgnoreCase("ON");
	LN.logf("RelaisNode::handleInput()", LoggerNode::INFO,
			"Receive command to switch %x to %s.", id, on ? "On" : "Off");
	updateRelais();
	return true;
}


void RelaisNode::drawFrame(OLEDDisplay* display, OLEDDisplayUiState* state,	int16_t x, int16_t y) {
	display->setFont(ArialMT_Plain_16);
	display->drawString(0+x,16,"Relais");
	for (uint_fast8_t i=0; i<8;i++) {
		int16_t xpos = (i*8)+4;
		int16_t ypos = 40;
		bool on = (relais_bitset & (1 << i)) != 0;
		//bool on = (0x55 & (1 << i)) != 0;
		display->drawRect(xpos,ypos,on?6:5,on?6:5);
		if (on) {
			display->drawRect(xpos+1,ypos+1,4,4);
			display->drawRect(xpos+2,ypos+2,2,2);
		}
	}
	display->drawHorizontalLine(0,60,128);
}



void RelaisNode::updateRelais() const {
	LN.logf("RelaisNode::updateValves()", LoggerNode::INFO, "Value: %x",
			relais_bitset);
	for (uint_fast8_t i = 0; i < 8; i++) {
		bool on = (relais_bitset & (1 << i) != 0);
		Homie.setNodeProperty(*this, String(i), on ? "On" : "Off", true);
	}
}
