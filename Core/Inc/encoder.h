#include <inttypes.h>
#include "stm32l4xx_hal.h"

//klasa dla uproszczonej obsługi enkodera

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
  void refresh(bool clk, bool data);
  void execute();
  uint8_t maximumVelocityMultiplier = 32;
  bool allowVelocityRecognition = true;
  void (*OnChange)(EncoderDirection, uint8_t) = NULL;

private:
  uint8_t calculateVelocity();
  bool a = false, b = false;
  bool pa = false, pb = false;
  uint32_t lastMillis = 0;
  uint32_t deltaT = 0;
};

#endif
