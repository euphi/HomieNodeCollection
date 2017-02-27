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
	void Event(const HomieEvent& event);

	virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx);


private:
	bool mqtt;
	bool wifi;
	bool cfgmode;
	int wifi_quality;
	String last_status;
};

#endif /* SRC_OLEDSTATUSINDICATOR_H_ */
