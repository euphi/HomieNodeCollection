/*
 * LoggerNode.cpp
 *
 *  Created on: 10.08.2016
 *      Author: ian
 */

#include "LoggerNode.h"
#include <Homie.hpp>

HomieSetting<const char*> LoggerNode::default_loglevel ("loglevel", "default loglevel");  // id, description


LoggerNode::LoggerNode() :
		m_loglevel(DEBUG), logSerial(true), HomieNode("Log", "Logger") {
	//advertise("DefaultLevel").settable();
	default_loglevel.setDefaultValue(levelstring[DEBUG].c_str()).setValidator([] (const char* candidate) {
		return convertToLevel(String(candidate)) != INVALID;
	});
	advertise("Level").settable();
	advertise("LogSerial").settable();
}

const String LoggerNode::levelstring[CRITICAL+1] = { "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };

void LoggerNode::setup() {
	E_Loglevel loglevel = convertToLevel(String(default_loglevel.get()));
	if (loglevel == INVALID)
	{
		logf("LoggerNode", ERROR, "Invalid Loglevel in config (%s)", default_loglevel.get());
	}
}

void LoggerNode::onReadyToOperate() {
	setProperty("Level").send(levelstring[m_loglevel]);
}


void LoggerNode::log(const String function, const E_Loglevel level,	const String text) const {
	if (!loglevel(level)) return;
	String mqtt_path(levelstring[level]);
	mqtt_path.concat('/');
	mqtt_path.concat(function);
	if (Homie.isConnected()) {
		setProperty(mqtt_path).setRetained(false).send(text);
	}
	if (logSerial || !Homie.isConnected()) Serial.printf("%d: %s:%s\n",millis(), mqtt_path.c_str(), text.c_str());
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
	LN.logf("LoggerNode::handleInput()", LoggerNode::DEBUG,
			"property %s set to %s", property.c_str(), property.c_str());
	if (property.equals("Level") /* || property.equals("DefaultLevel") */) {
		E_Loglevel newLevel = convertToLevel(value);
		if (newLevel == INVALID) {
			logf(__PRETTY_FUNCTION__, WARNING , "Receivd invalid level %s.", value.c_str());
			return false;
		}
		if (property.equals("Level")) {
			m_loglevel = newLevel;
			logf(__PRETTY_FUNCTION__, INFO, "New loglevel set to %d", m_loglevel);
			setProperty("Level").send(levelstring[m_loglevel]);
			return true;
//		} else {  // DefaultLevel
//			//default_loglevel.set(levelstring[newLevel]);
//			//logf(__PRETTY_FUNCTION__, INFO, "New default loglevel set to %d", newLevel);
//			//setProperty("DefaultLevel").send(levelstring[newLevel]);
//			logf(__PRETTY_FUNCTION__, WARNING, "Setting persistent values via MQTT not yet supported by Homie");
		}
	} else if (property.equals("LogSerial")) {
		bool on = value.equalsIgnoreCase("ON");
		logSerial = on;
		LN.logf("LoggerNode::handleInput()", LoggerNode::INFO,
				"Receive command to switch 'Log to serial' %s.", on ? "On" : "Off");
		setProperty("LogSerial").send(on ? "On" : "Off");
		return true;
	}
	logf(__PRETTY_FUNCTION__, ERROR,
			"Received invalid property %s with value %s", property.c_str(),	value.c_str());
	return false;
}

LoggerNode::E_Loglevel LoggerNode::convertToLevel(const String& level) {
	for (int_fast8_t iLevel = DEBUG; iLevel <= CRITICAL; iLevel++) {
		if (level.equalsIgnoreCase(levelstring[iLevel])) return static_cast<E_Loglevel>(iLevel);
	}
	return INVALID;

}

LoggerNode LN;
