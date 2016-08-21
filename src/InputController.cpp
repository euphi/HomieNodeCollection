/*
 * InputController.cpp
 *
 *  Created on: 19.08.2016
 *      Author: ian
 */

#include "InputController.h"
#include <Arduino.h>
#include <LoggerNode.h>

std::vector<InputController*>  InputController::ictrl_list;

InputController::InputController() {
	but_fwd.attach(13, INPUT_PULLUP);
	but_fwd.interval(1);
	but_bwd.attach(12, INPUT_PULLUP);
	but_bwd.interval(1);
	but_clk.attach(14, INPUT_PULLUP);
	but_clk.interval(8);
	ictrl_list.push_back(this);
}

void InputController::setup() {

}

void InputController::loop() {
	static uint16_t err_count;
	static int8_t fb_count;

	// Debounce Inputs every loop()
	bool bw = but_bwd.update();
	if (but_fwd.update()) bw = true;
	if (but_clk.update()) bw = true;
	if (bw)
	{
		if (but_fwd.rose() && but_bwd.rose()) err_count++;
		if (but_fwd.rose() && but_bwd.read()) {
			fb_count--;
		}
		if (but_bwd.rose() && but_fwd.read()) {
			fb_count++;
		}
//		LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG,
//				"Input count: %d (error: %d)", fb_count, err_count);
	}
}

