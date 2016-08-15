/*
 * RGBWNode.h
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#ifndef SRC_RGBWNODE_H_
#define SRC_RGBWNODE_H_

#include <HomieNode.hpp>

class RGBWNode: public HomieNode {

	static const float percent_to_pwm;


	enum RGB_MAP {
		R, G, B, W
	};

	enum RGBW_PINMAP {
		REDPIN = 12, GREENPIN = 13, BLUEPIN = 14, WHITEPIN = 16
	};

	const uint8_t rgbw_pins[4] = { REDPIN, GREENPIN, BLUEPIN, WHITEPIN };
	const char rgbw_id[4] = {'r', 'g', 'b', 'w'};
	uint16_t rgbw_values[4] = { 0, 0, 0, 0 };

    virtual bool handleInput(String const &property, String const &value);

private:
    void updateLEDs() const;
    void updateLED(uint8_t id) const;
    void PublishState(uint8_t id) const;

    bool initialized;


public:
	RGBWNode();
	void setup();
    void onReadyToOperate();
};

#endif /* SRC_RGBWNODE_H_ */
