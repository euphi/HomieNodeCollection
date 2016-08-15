/*
 * ConfigurationNode.h
 *
 *  Created on: 11.06.2016
 *      Author: ian
 */

#ifndef SRC_CONFIGURATIONNODE_H_
#define SRC_CONFIGURATIONNODE_H_

#include "HomieNode.hpp"

class ConfigurationNode: public HomieNode {
public:
	ConfigurationNode();
	virtual void setup();
	//void loop(); // loop() not necessary

	uint16_t getValveDuration(uint_fast8_t v) const {
		return valveONduration[v];
	};


private:
	uint16_t valveONduration[4];
	void writeToEEPROM();
	void readFromEEPROM();
};

extern ConfigurationNode config;

#endif /* SRC_CONFIGURATIONNODE_H_ */
