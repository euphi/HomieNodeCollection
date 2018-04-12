/*
 * InputPinArrayNode.h
 *
 *  Created on: 09.04.2018
 *      Author: ian
 */

#ifndef SRC_INPUTPINARRAYNODE_H_
#define SRC_INPUTPINARRAYNODE_H_

#include <HomieNode.hpp>



class InputPinArrayNode: public HomieNode {


public:
	typedef std::function<bool(uint8_t idx, bool state)> InputPinChangeEventHandler;
	InputPinArrayNode(std::vector<uint8_t> pins, InputPinChangeEventHandler& cb);

protected:
	  virtual void setup() override;
	  virtual void loop() override;
	  virtual void onReadyToOperate() override;

private:
	std::vector<uint8_t> inputPins;
	std::vector<bool> storedPins;
	InputPinChangeEventHandler & callback;
};

#endif /* SRC_INPUTPINARRAYNODE_H_ */
