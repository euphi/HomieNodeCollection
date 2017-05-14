/*
 * OLEDFrame.h
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#ifndef LIB_ESP8266_OLED_SSD1306_OLEDFRAME_H_
#define LIB_ESP8266_OLED_SSD1306_OLEDFRAME_H_

#include <vector>

#include <OLEDDisplay.h>
#include <OLEDDisplayUi.h>

class OLEDFrame {

public:
	OLEDFrame();
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) = 0;
	virtual const char * getFrameTitle() const {return "";}

private:
    static std::vector<OLEDFrame*> frames;

public:
	static uint8_t count() {return frames.size();};
	static OLEDFrame* get(uint8_t idx) {return (idx < count()) ? frames[idx]: 0;};

};

#endif /* LIB_ESP8266_OLED_SSD1306_OLEDFRAME_H_ */
