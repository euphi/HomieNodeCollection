/*
 * RelaisNode.h
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#ifndef SRC_RELAISNODE_H_
#define SRC_RELAISNODE_H_

#include <HomieNode.hpp>

#include <PCF8575.h>

class RelaisNode : public HomieNode {

public:
	RelaisNode(uint16_t defaults = 0x0000, uint16_t invert = 0x0000, uint16_t inputmask = 0x0000);

protected:
  virtual void setup() override;
  virtual void loop() override;

  virtual void onReadyToOperate() override;

  virtual bool handleInput(const String  &property, const HomieRange& range, const String &value) override;

private:
  void updateRelais(uint16_t updateMask=0xFFFF);

  uint16_t relais_bitset; // stores actual value
  uint16_t invert_bitset; // If bit is set, value written to PCF8575 will be inverted
  uint16_t input_mask;  // If bit is set, related pin will be configured as (quasi-)input
  uint16_t input_data;

  PCF8575 io;

};

#endif /* SRC_RELAISNODE_H_ */
