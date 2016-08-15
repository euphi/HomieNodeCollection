/*
 * OLEDStatusIndicator.cpp
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#include "OLEDStatusIndicator.h"

#include <ESP8266WiFi.h> // Needed for WiFi Signal Strength


#include "gfx_resources.h"
#include "images.h"

OLEDStatusIndicator::OLEDStatusIndicator() :
		display(0x3c, SDA, SCL), ui(&display), wifi(false), wifi_quality(-1), mqtt(0),
		updateNecessary(true), cycle_time(500) {

}

void OLEDStatusIndicator::Event(HomieEvent event) {
	switch (event) {
	case HOMIE_CONFIGURATION_MODE:
		last_status = "Configuration mode started";
		break;
	case HOMIE_NORMAL_MODE:
		last_status = "Normal mode started";
		break;
	case HOMIE_OTA_STARTED:
		last_status = "OTA started";
		break;
	case HOMIE_OTA_FAILED:
		last_status = "OTA failed";
		break;
	case HOMIE_OTA_SUCCESSFUL:
		last_status = "OTA successful";
		break;
	case HOMIE_ABOUT_TO_RESET:
		last_status = "About to reset";
		break;
	case HOMIE_WIFI_CONNECTED:
		last_status = "Wi-Fi connected";
		wifi = true;
		break;
	case HOMIE_WIFI_DISCONNECTED:
		last_status = "Wi-Fi disconnected";
		wifi = false;
		break;
	case HOMIE_MQTT_CONNECTED:
		last_status = "MQTT connected";
		mqtt = true;
		break;
	case HOMIE_MQTT_DISCONNECTED:
		last_status = "MQTT disconnected";
		mqtt = false;
		break;
	}
	updateNecessary = true;

}

void OLEDStatusIndicator::setup() {
	ui.setTargetFPS(30);

	// Customize the active and inactive symbol
	ui.setActiveSymbol(activeSymbol);
	ui.setInactiveSymbol(inactiveSymbol);

	// You can change this to
	// TOP, LEFT, BOTTOM, RIGHT
	ui.setIndicatorPosition(BOTTOM);

	// Defines where the first frame is located in the bar.
	ui.setIndicatorDirection(LEFT_RIGHT);

	// You can change the transition that is used
	// SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
	ui.setFrameAnimation(SLIDE_LEFT);
	ui.disableAutoTransition();
	ui.disableAllIndicators();
	ui.init();

	display.setLogBuffer(4,200);
	display.flipScreenVertically();

	but_fwd.attach(13, INPUT_PULLUP);
	but_fwd.interval(1);
	but_bwd.attach(12, INPUT_PULLUP);
	but_bwd.interval(1);
	but_clk.attach(14, INPUT_PULLUP);
	but_clk.interval(8);

	Homie.setLoggingPrinter(&display);
}

void OLEDStatusIndicator::drawOverlay(OLEDDisplay& display, OLEDDisplayUiState& state, uint8_t idx) {
	static uint8_t count = 0;

	uint32_t now = millis();

	bool blink_state = (now >> 4) % 2;
	uint8_t cycle4 = ++count % 4;

	display.setFont(ArialMT_Plain_10);
	if (wifi) {
		String RSSIString;
		int32_t rssi = WiFi.RSSI();
		RSSIString += rssi;
		RSSIString += " dbm";
		display.drawXbm(0, 0, wireless_full_width, wireless_full_height,
				wireless_full_bits);
		display.drawString(wireless_full_width, 3, RSSIString);
	} else {
		if (blink_state) {
			display.drawXbm(0, 0, wireless_full_width, wireless_full_height,
					wireless_full_bits);
		}
	}
	// MQTT indicator
	if (mqtt || (wifi && blink_state)) {
		display.drawXbm(128 - conn_mqtt_width, 0, conn_mqtt_width,
				conn_mqtt_height, conn_mqtt_bits);
	}

	// Lifesign
	uint8_t quads = (1 << 3 - cycle4); // Draw all quads except 1
	display.drawCircleQuads(128 - conn_mqtt_width - 8, 8, 6, quads);
}


void OLEDStatusIndicator::loop() {
	static uint32_t last = 0;
	static uint32_t lastpoll = 0;
	static uint16_t cyclecount = 0;
	static int8_t fb_count;
	static int8_t err_count;

	unsigned long mil = millis();
	ui.update();

	// Debounce Inputs every loop()
	bool bw = but_bwd.update();
	if (but_fwd.update()) bw = true;
	if (but_clk.update()) bw = true;
	if (bw)
	{
		updateNecessary = true;
		if (but_fwd.rose() && but_bwd.rose()) err_count++;
		if (but_fwd.rose() && but_bwd.read()) {
			fb_count--;
		}
		if (but_bwd.rose() && but_fwd.read()) {
			fb_count++;
		}

	}

	// Check for full cycle
	if (mil - last > cycle_time) {
		cyclecount ++;
		updateNecessary = true;
		last = mil;
	}


	if (!updateNecessary) return;

	bool blink_state = (cyclecount/2) % 2;

//	display.clear();
//	display.setFont(ArialMT_Plain_10);

	// Wifi Indicator


//
//	// Button State
//	if (but_bwd.read()) {
//		display.drawCircle(64,5,2);
//	}
//	if (but_fwd.read()) {
//		display.drawCircle(72,8,2);
//	}
//	if (but_clk.read()) {
//		display.drawCircle(80,11,2);
//	}

	// Status Text
//	display.drawStringMaxWidth(0,16,127,last_status);

	// Logger
//	display.setFont(Dialog_plain_7);
//	display.drawHorizontalLine(0,27,128);
//	display.drawLogBuffer(0,28);
//	String fwd_string("Fwd: ");
//	fwd_string += but_fwd.read() ? "ON":"OFF";
//	String back_string("Back: ");
//	back_string += but_bwd.read()? "ON":"OFF";
//	String press_string("Press: ");
//	press_string += but_clk.read()? "ON":"OFF";
//	String fbc_string("Counter: ");
//	fbc_string+=fb_count;
//	fbc_string+="   Errors: ";
//	fbc_string+=err_count;
//
//	display.drawString(0,27,fwd_string);
//	display.drawString(0,36,back_string);
//	display.drawString(0,45,press_string);
//	display.drawString(0,54,fbc_string);


	//display.display();
	updateNecessary = false;


}
