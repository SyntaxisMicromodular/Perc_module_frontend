#include <encoder.h>

void Encoder::setIncrementValue(int32_t val){
  incValue = val;
}

void Encoder::setConstrains(int32_t low, int32_t up){
  lowerConstrain = low;
  upperConstrain = up;
}

void Encoder::setRollover(bool input){
  allowRollover = input;
}

uint8_t Encoder::calculateVelocity(){
  //deltaT = millis() - lastMillis;
  //lastMillis = millis();

  //if (deltaT <  40) return 8;
  //if (deltaT <  80) return 4;
  //if (deltaT < 120) return 2;
  return 1;
}

void Encoder::refresh(bool clk, bool data){
  a = clk;
  b = data;

  if (!pa && a) currentState = ClkRising;
  if (pa && !a) currentState = ClkFalling;
  if (!pb && b){
	  currentState = DataRising;
	  if (previousState == ClkRising){
		  OnChange(Increment, calculateVelocity());
	  }
	  else{
		  OnChange(Decrement, calculateVelocity());
	  }
  }
  if (pb && !b) {
	  currentState = DataFalling;
	  if (previousState == ClkFalling){
		  OnChange(Increment, calculateVelocity());
	  }
	  else{
		  OnChange(Decrement, calculateVelocity());
	  }
  }


  /*
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
  }*/

  pa = a;
  pb = b;

  previousState = currentState;
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
