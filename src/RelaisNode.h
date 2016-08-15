/*
 * RelaisNode.h
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#ifndef SRC_RELAISNODE_H_
#define SRC_RELAISNODE_H_

#include <HomieNode.hpp>
#include <OLEDFrame.h>

#include <pcf8574.h>

class RelaisNode : public HomieNode, OLEDFrame {
public:
	RelaisNode();
protected:
  virtual void setup();

  virtual void loop();

  virtual void onReadyToOperate();

  virtual bool handleInput(String const &property, String const &value);

  virtual void drawFrame(OLEDDisplay *display,  OLEDDisplayUiState* state, int16_t x, int16_t y);


private:
  void updateRelais() const;

  uint8_t relais_bitset;
  PCF8574 io;

};

#endif /* SRC_RELAISNODE_H_ */
