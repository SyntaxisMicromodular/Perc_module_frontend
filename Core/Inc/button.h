#include <inttypes.h>
#include "stm32l4xx_hal.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button{
public:
	void refresh(bool in);
	void execute();
	void (*OnChange)(bool) = NULL;
private:
	bool currentState = false;
	bool previousState = true;
};

#endif
