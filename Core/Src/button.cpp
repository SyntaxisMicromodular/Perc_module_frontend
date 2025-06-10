#include <button.h>
void Button::refresh(){
	writeEncoderDemuxAddress(address);
	currentState = HAL_GPIO_ReadPin(port, pin);
	execute();
}
void Button::execute(){
	if (currentState != previousState) {
		if (onChange != nullptr) onChange(currentState);
		previousState = currentState;
	}
}
