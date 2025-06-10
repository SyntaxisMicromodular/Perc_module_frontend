#include <encoder.h>

uint8_t Encoder::calculateVelocity(){
  if (!allowVelocityRecognition) return 1;

  deltaT = HAL_GetTick() - lastMillis;
  lastMillis = HAL_GetTick();

  if (deltaT < 2)  {
	  overspeed = true;
	  return maximumVelocityMultiplier;
  }
  if (deltaT > 60) return 1;
  else if (deltaT < 5)  return maximumVelocityMultiplier;
  else return (-0.018 * deltaT + 1.091) * maximumVelocityMultiplier + 1;
}

void Encoder::setAddress(){
	writeEncoderDemuxAddress(demuxAddress);
}

void Encoder::refresh(){
  a = HAL_GPIO_ReadPin(aGpioPort, aPin);
  b = HAL_GPIO_ReadPin(bGpioPort, bPin);
  execute();
}

void Encoder::execute(){
  EncoderDirection direction;
  uint8_t velocity = 1;
  if (aPrev != a){
	  if (a) {
	  	  velocity = calculateVelocity();
	  	  b ? direction = Increment : direction = Decrement;
	  }
	  else {
		  velocity = calculateVelocity();
		  !b ? direction = Increment : direction = Decrement;
	  }

	  if (overspeed) {
		  direction = lastDirection;
		  overspeed = false;
	  }
	  lastDirection = direction;
	  if (onChange != nullptr) onChange(direction, velocity);
  }

  aPrev = a;
  bPrev = b;
}
