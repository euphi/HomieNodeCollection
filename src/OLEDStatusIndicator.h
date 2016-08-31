/*
 * OLEDStatusIndicator.h
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#ifndef SRC_OLEDSTATUSINDICATOR_H_
#define SRC_OLEDSTATUSINDICATOR_H_

#include <Homie.h>
#include <SSD1306.h>

#include <OLEDDisplayUi.h>
#include <OLEDOverlay.h>

class OLEDStatusIndicator : public OLEDOverlay {
public:
	//OLEDStatusIndicator(SSD1306& display); // TODO: Allow to set reference to Display (or abstraction layer)
	OLEDStatusIndicator();
	void Event(HomieEvent event);

	virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx);


private:


	bool mqtt;
	bool wifi;
	bool cfgmode;
	int wifi_quality;

	//SSD1306Wire display;
	//OLEDDisplayUi ui;

	bool updateNecessary;
	String last_status;
	uint16_t cycle_time;

	Bounce but_fwd;
	Bounce but_bwd;
	Bounce but_clk;





};

#endif /* SRC_OLEDSTATUSINDICATOR_H_ */
