/*
 * PCF8575.cpp
 *
 *  Created on: 21.09.2016
 *      Author: ian
 */


/* Dependencies */
#include <Wire.h>
#include <PCF8575.h>

PCF8575::PCF8575(uint8_t address, bool inv) :
		bs_output(inv?0xFFFF:0x0000), bs_input(0), bs_DDR(0xFFFF),
		i2c_addr(0), invert(inv)
        // DDR defaults to OUTPUT mode
		// Invert by default, Output set to all OFF, that means 1 in case of inverted.
        // PCF8575 outputs are all 1 after reset/power-on.
{
	i2c_addr = address;
	Wire.begin();
	sendI2C();
}

void PCF8575::pinMode(uint8_t pin, uint8_t mode) {
	switch (mode) {
	case OUTPUT:
		bs_DDR |= (1 << pin);
		bs_output &= ~(1 << pin);
		break;

	case INPUT:
		bs_DDR &= ~(1 << pin);
		bs_output &= ~(1 << pin);
		break;

	case INPUT_PULLUP:
		bs_DDR &= ~(1 << pin);
		bs_output |= (1 << pin);
		break;

	default:
		break;
	}
	sendI2C();
}

void PCF8575::digitalWrite(uint8_t pin, bool value) {
	if (value ^ invert)
		bs_output |= (1 << pin);
	else
		bs_output &= ~(1 << pin);
	sendI2C();
}

bool PCF8575::digitalRead(uint8_t pin) {
	readI2C();
	return (bs_input & (1 << pin));
}

void PCF8575::write16(uint16_t value) {
	if (invert) value ^= 0xFFFF;
	bs_output = value;
	sendI2C();
}

uint16_t PCF8575::read16() {
	readI2C();
	return bs_input;
}

void PCF8575::toggle(uint8_t pin) {
	bs_output ^= (1 << pin);
	sendI2C();
}

void PCF8575::readI2C() {
	size_t r = Wire.requestFrom(i2c_addr, (uint8_t) 0x02);
	while (Wire.available() < 2)
		;
	bs_input = Wire.read();
	bs_input |= Wire.read() << 8;
}

void PCF8575::sendI2C() {
	uint16_t value = (bs_input & ~bs_DDR) | bs_output;
	Serial.printf("sendI2C: Write value %04x.\n", value);
	Wire.beginTransmission(i2c_addr);
	size_t rc = Wire.write(value & 0x00FF);
	rc += Wire.write((value & 0xFF00) >> 8);
	if (rc != 2) LN.logf("PCF8575::sendI2C()", LoggerNode::ERROR, "Error writing to I2C.\n");
	rc = Wire.endTransmission();
	if (rc) LN.logf("PCF8575::sendI2C()", LoggerNode::ERROR, "Transmission Error (rc=%x)\n", rc);
}


