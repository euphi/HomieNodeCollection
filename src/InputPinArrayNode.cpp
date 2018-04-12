/*
 * InputPinArrayNode.cpp
 *
 *  Created on: 09.04.2018
 *      Author: ian
 */

#include <InputPinArrayNode.h>

InputPinArrayNode::InputPinArrayNode(std::vector<uint8_t> pins,	InputPinChangeEventHandler& cb):
			HomieNode("Input", "Contact"),
			inputPins(pins),
			storedPins(pins.size()),
			callback(cb) {
	advertiseRange("pin", 0, inputPins.size()-1);
}

void InputPinArrayNode::setup() {
	for (uint8_t pin : inputPins) {
		pinMode(pin, INPUT_PULLUP);
	}
}

void InputPinArrayNode::loop() {
	uint8_t idx = 0;
	for (uint8_t pin : inputPins) {
		bool curState = digitalRead(pin);
		bool oldState = storedPins[idx];
		if (curState != oldState) {
			setProperty("pin").setRange(idx).setRetained(true).send(curState ? "OPEN":"CLOSED");
			storedPins[idx] = curState;
			callback(idx, curState);
		}
		++idx;
	}
}

void InputPinArrayNode::onReadyToOperate() {
	uint8_t idx = 0;
	for (uint8_t pin : inputPins) {
		bool curState = digitalRead(pin);
		storedPins[idx] = curState;
		setProperty("pin").setRange(idx).setRetained(true).send(curState ? "OPEN":"CLOSED");
		idx++;
	}
}
