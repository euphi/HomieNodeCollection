/*
 * ConfigurationNode.cpp
 *
 *  Created on: 11.06.2016
 *      Author: ian
 */

#include "ConfigurationNode.h"
#include "Homie.hpp"

//#include <EEPROM.h>

ConfigurationNode::ConfigurationNode() :
		HomieNode("cfg", "Config", "Config") {
	//subscribeToAll();
}


void ConfigurationNode::setup() {
	Serial.println("ConfigurationNode::setup()");
}

void ConfigurationNode::writeToEEPROM() {
}

void ConfigurationNode::readFromEEPROM() {
}

ConfigurationNode config;
