/*
 * InputController.h
 *
 *  Created on: 19.08.2016
 *      Author: ian
 */

#ifndef SRC_INPUTCONTROLLER_H_
#define SRC_INPUTCONTROLLER_H_

#include <Bounce2.h>

#include <vector>

class InputController {

public:
	InputController();

	void loop();
	void setup();

	static std::vector<InputController*> ictrl_list;

private:
	Bounce but_fwd;
	Bounce but_bwd;
	Bounce but_clk;
};

#endif /* SRC_INPUTCONTROLLER_H_ */
