/*
 * LoggerNode.cpp
 *
 *  Created on: 10.08.2016
 *      Author: ian
 */

#include "LoggerNode.h"
#include <Homie.hpp>

LoggerNode::LoggerNode() :
		m_loglevel(DEBUG), HomieNode("Log", "Logger") {
}

String LoggerNode::levelstring[4] = { "DEBUG", "INFO", "ERROR", "CRITICAL" };

void LoggerNode::setup() {
	//empty
}

void LoggerNode::log(const String function, const E_Loglevel level,	const String text) const {
	if (!loglevel(level)) return;
	String mqtt_path(levelstring[level]);
	mqtt_path.concat('/');
	mqtt_path.concat(function);
	Homie.setNodeProperty(*this, mqtt_path).setRetained(false).send(text);
	Serial.printf("%d: %s:%s\n",millis(), mqtt_path.c_str(), text.c_str());
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

LoggerNode LN;

