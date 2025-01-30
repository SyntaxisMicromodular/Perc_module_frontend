#include <encoder.h>

uint8_t Encoder::calculateVelocity(){
  if (!allowVelocityRecognition) return 1;

  deltaT = HAL_GetTick() - lastMillis;
  lastMillis = HAL_GetTick();

  if (deltaT > 60) return 1;
  else if (deltaT < 5)  return maximumVelocityMultiplier;
  else return (-0.018 * deltaT + 1.091) * maximumVelocityMultiplier + 1;
}

void Encoder::refresh(bool in_a, bool in_b){
  a = in_a;
  b = in_b;
}

void Encoder::execute(){
  uint8_t velocity = 1;
  if (pa != a){
	  if (a) {
	  	  velocity = calculateVelocity();
	  	  b ? OnChange(Increment, velocity) : OnChange(Decrement, velocity);
	  }
	  else {
		  velocity = calculateVelocity();
		  !b ? OnChange(Increment, velocity) : OnChange(Decrement, velocity);
	  }
  }

  pa = a;
  pb = b;
}
