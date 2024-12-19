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
  void setIncrementValue(int32_t val);
  void setConstrains(int32_t low, int32_t up);
  void setRollover(bool input);
  void setVelocityRecognition(bool input);
  void refresh(bool clk, bool data);
  int32_t getCounter();
  void setCounter(int32_t value);
  bool isChange();
  void resetChange();
  void setCallback(void(*funcpointer)(EncoderDirection dir, uint8_t velocity));

private:
  void (*OnChange)(EncoderDirection, uint8_t);
  uint8_t calculateVelocity();
  int pin1, pin2;
  bool a = false, b = false;
  bool pa = false, pb = false;
  bool change = false;
  bool allowRollover = false;
  bool allowVelocityRecognition = true;
  EncoderState currentState = ClkRising;
  EncoderState previousState = ClkRising;
  int32_t counter = 0;
  int32_t incValue = 1;
  int32_t lowerConstrain = 0;
  int32_t upperConstrain = 255;
  uint32_t lastMillis = 0;
  uint32_t deltaT = 0;
};

#endif
