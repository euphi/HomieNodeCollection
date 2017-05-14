/*
 * OLEDFrame.cpp
 *
 *  Created on: 15.08.2016
 *      Author: ian
 */

#include <OLEDFrame.h>

/*static*/ std::vector<OLEDFrame*> OLEDFrame::frames;

OLEDFrame::OLEDFrame() {
	frames.push_back(this);
}


