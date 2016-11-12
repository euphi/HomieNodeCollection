/*
 * SensorNode.cpp
 *
 *  Created on: 16.05.2016
 *      Author: ian
 */

#include "SensorNode.h"
#include <Homie.hpp>


SensorNode::SensorNode() :
	HomieNode("Sensor", "sensor_t_h",
		[](String property, HomieRange range, String value) { return false; }),
	lastLoop8000ms(0), htu() {
	advertise("degrees");
	advertise("rel%");

	//subscribeToAll();
}

void SensorNode::setup() {
	Wire.begin();
	Sensors::initialize();
	char printbuf[50];
	snprintf(printbuf, sizeof(printbuf),
			"Initialized sensors at %x (Thermo), %x (Baro)",
			Sensors::getThermometer(), Sensors::getBarometer());
	Serial.println(printbuf);
	htu.begin();
}

void SensorNode::loop() {
	if (millis() - lastLoop8000ms >= 30000UL || lastLoop8000ms == 0) {
//		float cur_temp = Sensors::getThermometer()->getTemperature();
//		float cur_press = Sensors::getBarometer()->getSealevelPressure(320.0);
		temp = htu.readTemperature();
		hum = htu.readHumidity();
		lastLoop8000ms = millis();
		setProperty("degrees").send(String(temp));
//		Homie.setNodeProperty(*this, "hPa", String(cur_press));
		setProperty("rel%").send(String(hum));
	}
}

