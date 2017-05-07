/*
 * LoggerNode.cpp
 *
 *  Created on: 10.08.2016
 *      Author: ian
 */

#include "LoggerNode.h"
#include <Homie.hpp>

LoggerNode::LoggerNode() :
		m_loglevel(DEBUG), logSerial(true), HomieNode("Log", "Logger") {
	advertise("Level").settable();
	advertise("LogSerial").settable();
}

const String LoggerNode::levelstring[4] = { "DEBUG", "INFO", "ERROR", "CRITICAL" };

void LoggerNode::setup() {
	//empty
}

void LoggerNode::log(const String function, const E_Loglevel level,	const String text) const {
	if (!loglevel(level)) return;
	String mqtt_path(levelstring[level]);
	mqtt_path.concat('/');
	mqtt_path.concat(function);
	setProperty(mqtt_path).setRetained(false).send(text);
	if (logSerial) Serial.printf("%d: %s:%s\n",millis(), mqtt_path.c_str(), text.c_str());
}

void LoggerNode::logf(const String function, const E_Loglevel level, const char* format, ...) const {
	if (!loglevel(level)) return;
	va_list arg;
	va_start(arg, format);
	char temp[100];
	char* buffer = temp;
	size_t len = vsnprintf(temp, sizeof(temp), format, arg);
	va_end(arg);
	log(function, level, temp);
}

bool LoggerNode::handleInput(const String& property, const HomieRange& range,
		const String& value) {
	if (property.equals("Level")) {
		E_Loglevel newLevel = E_Loglevel::INVALID;
		switch (value.c_str()[0]) {
		case 'D':
		case 'd':
			newLevel = E_Loglevel::DEBUG;
			break;
		case 'I':
		case 'i':
			newLevel = E_Loglevel::INFO;
			break;
		case 'E':
		case 'e':
			newLevel = E_Loglevel::ERROR;
			break;
		case 'C':
		case 'c':
			newLevel = E_Loglevel::CRITICAL;
			break;
		}
		if (newLevel >= E_Loglevel::DEBUG && newLevel <= E_Loglevel::CRITICAL) {
			m_loglevel = newLevel;
			logf(__PRETTY_FUNCTION__, INFO, "New loglevel set to %d", m_loglevel);
			setProperty("Level").send(levelstring[m_loglevel]);
			return true;
		}
	} else if (property.equals("LogSerial")) {
		bool on = value.equalsIgnoreCase("ON");
		logSerial = on;
		LN.logf("RelaisNode::handleInput()", LoggerNode::INFO,
				"Receive command to switch 'Log to serial' %s.", on ? "On" : "Off");
		setProperty("LogSerial").send(on ? "On" : "Off");
		return true;
	}
	logf(__PRETTY_FUNCTION__, ERROR,
			"Received invalid property %s with value %s", property.c_str(),	value.c_str());
	return false;
}

LoggerNode LN;
