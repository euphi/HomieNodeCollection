/*
 * RelaisNode.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include <RelaisNode.h>

#include <LoggerNode.h>
#include <Homie.hpp>

/** Constructor. Use it to configure the inputs and outputs
 * @param defaults initial default value
 * @param invert If a bit is set, the corresponding IO is inverted (OUTPUT only, also inverts initial value)
 * @param inputmask: If a bit is set, the corresponding IO is configured as INPUT_PULLUP, if not, as OUTPUT
 */
RelaisNode::RelaisNode(uint16_t defaults, uint16_t invert, uint16_t inputmask) :
		HomieNode("Relais", "switch16"),
		relais_bitset((defaults^invert) | inputmask),
		invert_bitset(invert),
		input_mask(inputmask),
		input_data(0x0000),
		updateMaskLoop(0x0000),
		io(0x20, false) {
	for (uint_fast8_t i = 0 ; i < 16 ; i++) {
		bool in = ((input_mask & (1 << i)) != 0);
		io.pinMode(i, in ? INPUT_PULLUP : OUTPUT);
	}
	advertiseRange("in",1,16).settable();
}

/** only thing to do in setup(): Initial write of output values to PCF
 *
 */
void RelaisNode::setup() {
	updateRelais(0); // write to PCF only
}

/** only thing to do in onReadyToOperate(): Send initial values to Homie MQTT
 *
 */
void RelaisNode::onReadyToOperate() {
	LN.log("RelaisNode", LoggerNode::DEBUG, "Ready");
	RelaisNode::updateRelais(0xFFFF);
}

/** Read input from IO device
 * Checks for difference in the selected bits (every bit set in field input_mask).
 * In case of difference, the new property (ranged index) of the RelaisNode is send over MQTT */
void RelaisNode::readInputs() {
	uint16_t input = io.read16();
	uint16_t diff = ((input ^ input_data) & input_mask);
	if (diff == 0) return;
	for (uint_fast8_t i = 0; i < 16; i++) {
		if ((diff & (1 << i)) != 0) {
			bool on = (input_data & (1 << i)) != 0;
			bool inverted = (invert_bitset & (1 << i)) != 0;
			LN.logf("RN::loop", LoggerNode::INFO,
					"Input %d changed to %c%s, new: %x, old: %x, diff: %x", i,
					inverted ? '~' : ' ', on ? "On" : "Off", input, input_data,
					diff);
			setProperty("in").setRange(i + 1).send(
					on ^ inverted ? "ON" : "OFF");
		}
	}
	input_data = input;
}

/** loop() is called every cycle from Homie
 * Overrides the HomieNode::loop() method. Every 8th cycle it checks the inputs for changes.
 * Furthermore it updates the outputs if a change has occured.
 * To do the actual change of output within the loop() function is necessary
 * because the handleInput() method is running in the network task of the NON-OS SDK
 * due to the use of async IO. If a write would occur during the handleInput there
 * is a race condition between writing the output and reading the input that may disturb
 * the I2C communication or may give false readings and/or writings.
 *
 * See also https://euphi.github.io/2018/03/31/ArduinoESP8266-multipleTasks.html
 *
 */
void RelaisNode::loop() {
	static uint_fast8_t count = 0;
	if ((++count % 8)==0) readInputs(); // read I2C on every 8th cycle only
	if (updateMaskLoop) {
		updateRelais(updateMaskLoop);
		updateMaskLoop = 0x0000;
	}

}

/** handleInput() handles the received MQTT messages from Homie
 *
 * The bit to change is
 * The property is not checked (but this is done by homie when evaluating the range)
 *
 */
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
	updateMaskLoop |= selected_bit;
	return true;
}

/** helper method to update output state
 *  @param updateMask bits that needs to be updated on MQTT
 *
 *  This method write all data to the I2C device.
 *  Furthermore it checks which bits should be updated on Homie MQTT and sends their state
 */
void RelaisNode::updateRelais(uint16_t updateMask) {
	static uint16_t last = relais_bitset;
	LN.logf("RelaisNode::updateRelais()", LoggerNode::DEBUG, "Value: %x (Update: %x)", relais_bitset, updateMask);
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
