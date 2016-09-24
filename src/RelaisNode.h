/*
 * RelaisNode.h
 *
 *  Created on: 14.08.2016
 *      Author: ian
 */

#ifndef SRC_RELAISNODE_H_
#define SRC_RELAISNODE_H_

#include <HomieNode.hpp>

#include <Atm_encoder.hpp>

#include <PCF8575.h>

class RelaisNode : public HomieNode {

public:
	RelaisNode();
protected:
//  virtual void setup() override;
//
//  virtual void loop() override;

  virtual void onReadyToOperate() override;

  virtual bool handleInput(String const &property, HomieRange range, String const &value) override;

private:
  void updateRelais(uint16_t updateMask=0xFFFF);

  uint16_t relais_bitset;

  PCF8575 io;

};

#endif /* SRC_RELAISNODE_H_ */
