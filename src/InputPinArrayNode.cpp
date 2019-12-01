/*
 * InputPinArrayNode.cpp
 *
 *  Created on: 09.04.2018
 *      Author: ian
 */

#include <InputPinArrayNode.h>

InputPinArrayNode::InputPinArrayNode(std::vector<pin_data> pins, InputPinChangeEventHandler& cb):
			HomieNode("Input", "Contact", "Contact", true, 0,  pins.size()-1),
			inputPins(pins),
			callback(cb) {
	advertise("pin").setDatatype("boolean");
}

void InputPinArrayNode::setup() {
	for (pin_data pin : inputPins) {
		pinMode(pin.pin, INPUT_PULLUP);
	}
	setRunLoopDisconnected(true);
}

void InputPinArrayNode::loop() {
	uint8_t idx = 0;
	for (pin_data pin : inputPins) {
		bool curState = digitalRead(pin.pin);
		if (curState != pin.stored_state) {
			setProperty("pin").setRange(idx).setRetained(true).send(curState == pin.closed_state ? "CLOSED":"OPEN");
			inputPins[idx].stored_state = curState;
			callback(idx, curState);
		}
		++idx;
	}
}

void InputPinArrayNode::onReadyToOperate() {
	uint8_t idx = 0;
	for (pin_data pin : inputPins) {
		bool curState = digitalRead(pin.pin);
		pin.stored_state = curState;
		setProperty("pin").setRange(idx).setRetained(true).send(curState == pin.closed_state ? "CLOSED":"OPEN");
		idx++;
	}
}
