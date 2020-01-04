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
#ifdef SENSORS_BMP180_ATTACHED
#include <Sensors.h>
#else
#include "SparkFunHTU21D.h"
#endif

#include <HomieNode.hpp>

class SensorNode: public HomieNode {
private:
	static HomieSetting<double> tempOffset;
	static HomieSetting<long> interval;

	unsigned long lastLoop8000ms;
	float temp = NAN;

#ifdef SENSORS_BMP180_ATTACHED
	float pressure;
#endif
#ifndef SENSORS_BMP180_ATTACHED
	float hum = NAN;
	HTU21D htu;
#endif

public:
	SensorNode();
	void setup();
	void loop();

#ifndef SENSORS_BMP180_ATTACHED
	float getHumidity() const {
		return hum;
	}
#endif

#ifdef SENSORS_BMP180_ATTACHED
	float getPressure(bool sealevel=true) const {
		return pressure;
	}
#endif

	float getTemperatur() const {
		return temp;
	}

};

#endif /* SRC_SENSORNODE_H_ */
