#include <inttypes.h>
#include "stm32l4xx_hal.h"
#include "demux.h"

#ifndef BUTTON_H
#define BUTTON_H

class Button{
public:
	Button(GPIO_TypeDef* gpioPort, uint16_t gpioPin, uint8_t dmuxAddr){
		port = gpioPort;
		pin = gpioPin;
		address = dmuxAddr;
	}
	void refresh();
	void execute();
	void (*onChange)(bool) = nullptr;
private:
	bool currentState = false;
	bool previousState = true;
	GPIO_TypeDef* port;
	uint16_t pin;
	uint8_t address = 0;
};

#endif
