#include <encoder.h>

void Encoder::setIncrementValue(int32_t val){
  incValue = val;
}

void Encoder::setMaximumVelocityIncrement(uint8_t val){
	maxinc = val;
}

void Encoder::setConstrains(int32_t low, int32_t up){
  lowerConstrain = low;
  upperConstrain = up;
}

void Encoder::setRollover(bool input){
  allowRollover = input;
}

void Encoder::setVelocityRecognition(bool input){
  allowVelocityRecognition = input;
}

uint8_t Encoder::calculateVelocity(){
  if (!allowVelocityRecognition) return 1;

  deltaT = HAL_GetTick() - lastMillis;
  lastMillis = HAL_GetTick();

  if (deltaT > 60) return 1;
  else if (deltaT < 5)  return maxinc;
  else return (-0.018 * deltaT + 1.091) * maxinc + 1;
}

void Encoder::refresh(bool clk, bool data){
  a = clk;
  b = data;
  uint8_t velocity = 1;

  if (!pa && a) {
	  velocity = calculateVelocity();
      data ? OnChange(Increment, 0) : OnChange(Decrement, 0);
      data ? counter+=velocity : counter-=velocity;
  }
  if (pa && !a) {
	  velocity = calculateVelocity();
		!data ? OnChange(Increment, 0) : OnChange(Decrement, 0);
		!data ? counter+=velocity : counter-=velocity;
  }

  if (counter < lowerConstrain) {
    if (allowRollover)
    {
      counter = upperConstrain;
    }
    else
    {
      counter = lowerConstrain;
    }
  }
  if (counter > upperConstrain) {
    if (allowRollover)
    {
      counter = lowerConstrain;
    }
    else
    {
      counter = upperConstrain;
    }
  }

  pa = a;
  pb = b;
}

int32_t Encoder::getCounter(){
  return counter;
}

void Encoder::setCounter(int32_t value){
    if (value > upperConstrain) {
        counter = upperConstrain;
        return;
    }
    if (value < lowerConstrain){
        counter = lowerConstrain;
        return;
    }
    counter = value;
}

bool Encoder::isChange(){
  if (change){
    change = false;
    return true;
  }
  return false;
}

void Encoder::setCallback(void(*funcpointer)(EncoderDirection, uint8_t)){
  OnChange = funcpointer;
}
