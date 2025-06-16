#include "cpp_main.h"
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "OLED_SSD1306.h"
#include "GFX_BW.h"
#include "fonts/fonts.h"
#include "encoder.h"
#include "button.h"
#include "bitop.h"
#include "demux.h"
#include <math.h>

OLEDdefinition oled[4];
Encoder encoders[4] = { Encoder(MUX_Common2_GPIO_Port, MUX_Common2_Pin, MUX_Common_GPIO_Port, MUX_Common_Pin, 0),
						Encoder(MUX_Common2_GPIO_Port, MUX_Common2_Pin, MUX_Common_GPIO_Port, MUX_Common_Pin, 1),
						Encoder(MUX_Common2_GPIO_Port, MUX_Common2_Pin, MUX_Common_GPIO_Port, MUX_Common_Pin, 2),
						Encoder(MUX_Common2_GPIO_Port, MUX_Common2_Pin, MUX_Common_GPIO_Port, MUX_Common_Pin, 3)
					  };
Button buttons[4] = { Button(MUX_Common_GPIO_Port, MUX_Common_Pin, 4),
					  Button(MUX_Common_GPIO_Port, MUX_Common_Pin, 5),
					  Button(MUX_Common2_GPIO_Port, MUX_Common2_Pin, 4),
					  Button(MUX_Common2_GPIO_Port, MUX_Common2_Pin, 5)
};

const int CAPTION_LENGTH = 20;
const int VALUE_LENGTH = 7;
const int DISPLAYS = 4;

char caption[DISPLAYS][CAPTION_LENGTH];

char value[DISPLAYS][VALUE_LENGTH];

bool readState = false;

void initializeOLEDs(){
	oled[0].PortCS = SSD1306_CS_GPIO_Port;
	oled[0].PinCS = SSD1306_CS_Pin;
	oled[0].rotation_90 = false;

	oled[1].PortCS = SSD1306_CS2_GPIO_Port;
	oled[1].PinCS = SSD1306_CS2_Pin;
	oled[1].rotation_90 = false;

	oled[2].PortCS = SSD1306_CS3_GPIO_Port;
	oled[2].PinCS = SSD1306_CS3_Pin;
	oled[2].rotation_90 = false;

	oled[3].PortCS = SSD1306_CS4_GPIO_Port;
	oled[3].PinCS = SSD1306_CS4_Pin;
	oled[3].rotation_90 = false;

	for (int i = 0; i < DISPLAYS; i++){
		sprintf(caption[i], "Caption %d", i);
		sprintf(value[i], "%d", i);
	}
	GFX_SetFont(font_8x5);
	GFX_SetFontSize(1);

	SSD1306_ResetOLEDS();

	for (int i = 0; i < DISPLAYS; i++){
		SSD1306_SetOLED(&oled[i]);
		SSD1306_SpiInit(&hspi1);
		SSD1306_RotateDisplay(1);
		SSD1306_Clear(BLACK);
		SSD1306_Display();

		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
	}
	SSD1306_SetOLED(&oled[0]);
}

void drawSingleScreen(uint8_t screenNumber){
	while (hspi1.hdmatx->State != HAL_DMA_STATE_READY) {}
	if(hspi1.hdmatx->State == HAL_DMA_STATE_READY)
	{
		char caption_fn[CAPTION_LENGTH];
		char value_fn[VALUE_LENGTH];
		for (int i = 0; i < CAPTION_LENGTH; i++) caption_fn[i] = caption[screenNumber][i];
		for (int i = 0; i < VALUE_LENGTH; i++) value_fn[i] = value[screenNumber][i];

		SSD1306_SetOLED(&oled[screenNumber]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		GFX_DrawString(0,0, caption_fn, WHITE, BLACK);
		GFX_SetFontSize(3);
		GFX_DrawString(0, 20, value_fn, WHITE, BLACK);
		SSD1306_Display();
	}
}

void sendData(uint8_t* data, uint16_t size){
	HAL_UART_Transmit(&huart1, data, size, 1);
	HAL_UART_Transmit(&huart2, data, size, 1);
}

void sendEncoderData(uint8_t encoderNumber, EncoderDirection dir, uint8_t velocity){
	char tmp[10] = "";
	char sign = dir == Increment ? '+' : '-';
	sprintf(tmp, "E%d%c%d\r\n", encoderNumber, sign, velocity);
	sendData((uint8_t*)tmp, strlen(tmp));
	//sprintf(value[encoderNumber], "%c%d", sign, velocity);
}

void enc0Callback(EncoderDirection dir, uint8_t velocity){
	sendEncoderData(0, dir, velocity);
}

void enc1Callback(EncoderDirection dir, uint8_t velocity){
	sendEncoderData(1, dir, velocity);
}

void enc2Callback(EncoderDirection dir, uint8_t velocity){
	sendEncoderData(2, dir, velocity);
}

void enc3Callback(EncoderDirection dir, uint8_t velocity){
	sendEncoderData(3, dir, velocity);
}

void initializeEncoders(){
	for (uint8_t i = 0; i < 4; i++){	//first scan to avoid "ghost rotations" before setting initial values
		encoders[i].setAddress();
		HAL_Delay(1);
		encoders[i].refresh();
	}

	encoders[0].onChange = enc0Callback;
	encoders[1].onChange = enc1Callback;
	encoders[2].onChange = enc2Callback;
	encoders[3].onChange = enc3Callback;
}

void sendButtonData(uint8_t buttonNumber, bool state){
	char tmp[10] = "";
	char statechar = state ? '1' : '0';
	sprintf(tmp, "B%d%c\r\n", buttonNumber, statechar);
	sendData((uint8_t*)tmp, strlen(tmp));
}

void btn0Callback(bool state){
	sendButtonData(0, state);
}

void btn1Callback(bool state){
	sendButtonData(1, state);
}

void btn2Callback(bool state){
	sendButtonData(2, state);
}

void btn3Callback(bool state){
	sendButtonData(3, state);
}

void initializeButtons(){
	for (uint8_t i = 0; i < 4; i++) buttons[i].refresh();

	buttons[0].onChange = btn0Callback;
	buttons[1].onChange = btn1Callback;
	buttons[2].onChange = btn2Callback;
	buttons[3].onChange = btn3Callback;
}

void setup(){
	HAL_Delay(100);
	initializeEncoders();
	initializeButtons();
	initializeOLEDs();
	for (uint8_t i = 0; i < DISPLAYS; i++){
		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
		drawSingleScreen(i);
	}
}

void EncoderInterrupt(){
	static uint8_t currentReadChannel = 0;
	if (!readState){
		encoders[currentReadChannel].setAddress();
		readState = true;
	}
	if(readState){
		encoders[currentReadChannel].refresh();
		++currentReadChannel;
		if (currentReadChannel == 4){
			currentReadChannel = 0;
		}
		readState = false;
	}

	for (uint8_t i = 0; i < 4; i++) buttons[i].refresh();
}

void messageReceived(char* buf, uint16_t size){
	if (size < 3) return;
	uint8_t selectedDisplay = 0;
	bool valuenotcaption = (buf[0] == 'V');

	selectedDisplay = buf[1] - '0';

	if (valuenotcaption){
		char value_fn[VALUE_LENGTH];
		for (int i = 0; i < VALUE_LENGTH; i++) value_fn[i] = ' ';
		for (int i = 0; i < size - 2 && i < VALUE_LENGTH ; i++) value_fn[i] = buf[i+2];
		for (int i = 0; i < VALUE_LENGTH; i++) value[selectedDisplay][i] = value_fn[i];
		//strcpy(value[selectedDisplay], value_fn);
	}
	if (!valuenotcaption){
		char caption_fn[CAPTION_LENGTH];
		for (int i = 0; i < CAPTION_LENGTH; i++) caption_fn[i] = ' ';
		for (int i = 0; i < size - 2 && i < CAPTION_LENGTH ; i++) caption_fn[i] = buf[i+2];
		for (int i = 0; i < CAPTION_LENGTH; i++) caption[selectedDisplay][i] = caption_fn[i];
		//strcpy(caption[selectedDisplay], caption_fn);
	}
}

void refreshScreens(){
	for (int i = 0; i < DISPLAYS; i++) drawSingleScreen(i);
}
