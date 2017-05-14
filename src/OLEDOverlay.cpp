/*
 * OLEDIndicator.cpp
 *
 *  Created on: 15.08.2016
 *      Author: ian
 */

#include <OLEDOverlay.h>

/*static*/ std::vector<OLEDOverlay*> OLEDOverlay::overlays;

OLEDOverlay::OLEDOverlay() {
	overlays.push_back(this);
}



