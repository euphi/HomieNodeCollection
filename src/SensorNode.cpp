/*
 * SensorNode.cpp
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#include "SensorNode.h"
#include <Homie.hpp>
#include "LoggerNode.h"


HomieSetting<double> SensorNode::tempOffset ("TempAdj", "offset to add to temperature");
HomieSetting<long> SensorNode::interval("sensorInterval", "interval how often to read sensors");


SensorNode::SensorNode() :
	HomieNode("Sensor", "Sensor (Temperatur & Luftfeuchte)", "sensor_t_h"),
	lastLoop8000ms(0),
	temp(NAN),
#ifndef SENSORS_BMP180_ATTACHED
	hum (NAN),
	htu()
#else
	pressure(NAN)
#endif

{
	tempOffset.setDefaultValue(0).setValidator([] (double candidate) {
		return ((!isnan(candidate) || candidate == 0.0) && candidate > -15.0 && candidate < 15.0);
	});
	interval.setDefaultValue(30000).setValidator([] (long candidate) {
		return (candidate > 1000 && candidate < 600000);
	});
	advertise("degrees").setName("Temperatur").setRetained(true).setUnit("°C").setDatatype("float").setFormat("-50:100");
	advertise("relH").setName("relative Luftfeuchte").setRetained(true).setUnit("%").setDatatype("float").setFormat("0:100");
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
	if (millis() - lastLoop8000ms >= (unsigned long) interval.get() || lastLoop8000ms == 0) {
		lastLoop8000ms = millis();
#ifdef SENSORS_BMP180_ATTACHED
		temp = Sensors::getThermometer()->getTemperature();
		pressure = Sensors::getBarometer()->getSealevelPressure(290);
		if (isnan(pressure) == 0 && (pressure > 800) && (pressure < 1200) ) {
			setProperty("pressure").send(String(pressure));
		}
#else
		temp = htu.readTemperature() +  tempOffset.get();
		hum = htu.readHumidity();
		if (isnan(hum) == 0) {
			setProperty("relH").send(String(hum));
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

