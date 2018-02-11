/*
 * SensorNode.cpp
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#include "SensorNode.h"
#include <Homie.hpp>
#include "LoggerNode.h"


SensorNode::SensorNode() :
	HomieNode("Sensor", "sensor_t_h"),
	lastLoop8000ms(0),
	temp(NAN),
	temp_adjust(0),  // TODO: Use HomieSetting
#ifndef SENSORS_BMP180_ATTACHED
	hum (NAN),
	htu()
#else
	pressure(NAN)
#endif

{
	advertise("degrees");
	advertise("rel%");

	//subscribeToAll();
}

void SensorNode::setup() {
#ifdef SENSORS_BMP180_ATTACHED
	Sensors::initialize();
	char printbuf[50];
	snprintf(printbuf, sizeof(printbuf),
			"Initialized sensors at %x (Thermo), %x (Baro)",
	Sensors::getThermometer(), Sensors::getBarometer());
	Serial.println(printbuf);
	Sensors::initialize();
#else
	//htu.begin(); - this is just another call to Wire.begin(), so don't call it.
#endif
}

void SensorNode::loop() {
	if (millis() - lastLoop8000ms >= 30000UL || lastLoop8000ms == 0) {
		lastLoop8000ms = millis();
#ifdef SENSORS_BMP180_ATTACHED
		temp = Sensors::getThermometer()->getTemperature();
		pressure = Sensors::getBarometer()->getSealevelPressure(290);
		if (isnan(pressure) == 0 && (pressure > 800) && (pressure < 1200) ) {
			setProperty("pressure").send(String(pressure));
		}
#else
		temp = htu.readTemperature() +  temp_adjust;
		hum = htu.readHumidity();
		if (isnan(hum) == 0) {
			setProperty("rel%").send(String(hum));
		} else {
			LN.log("SensorNode", LoggerNode::ERROR, "Cannot read humidity on I2C");
		}
#endif
		if (isnan(temp) == 0 && (temp > -50) && (temp < 150)) {
			setProperty("degrees").send(String(temp));
		} else {
			LN.log("SensorNode", LoggerNode::ERROR, "Cannot read temperature on I2C");
		}
	}
}

