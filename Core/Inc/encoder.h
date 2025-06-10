#include <inttypes.h>
#include "stm32l4xx_hal.h"
#include "demux.h"

#ifndef ENKODER_SRC
#define ENKODER_SRC

enum EncoderDirection{
  Decrement = 0,
  Increment = 1
};

enum EncoderState{
	ClkRising = 0,
	DataRising = 2,
	ClkFalling = 4,
	DataFalling = 6
};

class Encoder{
public:
  Encoder(GPIO_TypeDef* gpioA, uint16_t aP, GPIO_TypeDef* gpioB, uint16_t bP, uint8_t dmuxAddr){
	  aGpioPort = gpioA;
	  aPin = aP;
	  bGpioPort = gpioB;
	  bPin = bP;
	  demuxAddress = dmuxAddr;
  }
  void setAddress();
  void refresh();
  void execute();
  uint8_t maximumVelocityMultiplier = 32;
  bool allowVelocityRecognition = true;
  void (*onChange)(EncoderDirection, uint8_t) = nullptr;

private:
  uint8_t calculateVelocity();
  bool a = false, b = false;
  bool aPrev = false, bPrev = false;
  uint32_t lastMillis = 0;
  uint32_t deltaT = 0;
  EncoderDirection lastDirection = Decrement;
  bool overspeed = false;
  GPIO_TypeDef* aGpioPort;
  uint16_t aPin;
  GPIO_TypeDef* bGpioPort;
  uint16_t bPin;
  uint8_t demuxAddress = 0;
};

#endif
