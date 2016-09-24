/*
 * PCF8575.h
 *
 *  Created on: 21.09.2016
 *      Author: ian
 */

#ifndef SRC_PCF8575_H_
#define SRC_PCF8575_H_

#include <Arduino.h>

class PCF8575 {
public:
	PCF8575(uint8_t address = 0x20, bool inv=true);

	void pinMode(uint8_t pin, uint8_t mode);

	void digitalWrite(uint8_t pin, bool value);
	bool digitalRead(uint8_t pin);
	void toggle(uint8_t pin);

	void write16(uint16_t value);
	uint16_t read16();

private:
	/** Output Bitset */
	uint16_t bs_output;

	/** Input Bitset */
	uint16_t bs_input;

	/** Pin direction Bitset (1: OUT, 0: IN) */
	uint16_t bs_DDR;

	/** PCF8575 I2C address */
	uint8_t i2c_addr;

	/** Invert all OUTPUT bits */
	bool invert;

	void readI2C();
	void sendI2C();
};

#endif /* SRC_PCF8575_H_ */
