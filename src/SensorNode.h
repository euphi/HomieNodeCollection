/*
 * SensorNode.h
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#ifndef SRC_SENSORNODE_H_
#define SRC_SENSORNODE_H_


#include <Wire.h>

/* For other sensors than HTU21 have a look at
 *  
 * https://github.com/loopj/i2c-sensor-hal
 * 
 * That lib is also available at platformio:
 *   http://platformio.org/lib/show/578/Sensors
 */
//#define SENSORS_BMP180_ATTACHED
//#include <Sensors.h>


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

	float getHumidity() const {
		return hum;
	}

	float getTemperatur() const {
		return temp;
	}

private:
	float temp = NAN;
	float hum = NAN;

};

#endif /* SRC_SENSORNODE_H_ */
