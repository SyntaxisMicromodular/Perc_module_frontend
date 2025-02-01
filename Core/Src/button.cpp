#include <button.h>
void Button::refresh(bool in){
	currentState = in;
}
void Button::execute(){
	if (currentState != previousState) {
		OnChange(currentState);
		previousState = currentState;
	}
}
