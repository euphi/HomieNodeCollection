/*
 * SensorNode.h
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#ifndef SRC_SENSORNODE_H_
#define SRC_SENSORNODE_H_


#include <Wire.h>
#define SENSORS_BMP180_ATTACHED
#include <Sensors.h>
#include "SparkFunHTU21D.h"


#include <HomieNode.hpp>

class SensorNode: public HomieNode {
private:
	unsigned long lastLoop8000ms;

	HTU21D htu;

public:
	SensorNode();
	void setup();
	void loop();

};

#endif /* SRC_SENSORNODE_H_ */
