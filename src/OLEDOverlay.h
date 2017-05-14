/*
 * OLEDIndicator.h
 *
 *  Created on: 15.08.2016
 *      Author: ian
 */

#ifndef LIB_ESP8266_OLED_SSD1306_OLEDOVERLAY_H_
#define LIB_ESP8266_OLED_SSD1306_OLEDOVERLAY_H_

#include <vector>

#include <OLEDDisplayUi.h>


class OLEDOverlay {
public:
	OLEDOverlay();

	virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) = 0;

private:
    friend class OLEDDisplayUi;
	static std::vector<OLEDOverlay*> overlays;

public:
	static uint8_t count() {return overlays.size();};
	static OLEDOverlay* get(uint8_t idx) {return (idx < count()) ? overlays[idx] : nullptr;};


};

#endif /* LIB_ESP8266_OLED_SSD1306_OLEDOVERLAY_H_ */
