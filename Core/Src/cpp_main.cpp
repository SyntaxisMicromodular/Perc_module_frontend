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
#include <math.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

struct OLEDdefinition oled[4];
Encoder enc[4];

#define CAPTION_LENGTH 20
#define VALUE_LENGTH 5

char caption0[CAPTION_LENGTH];
char caption1[CAPTION_LENGTH];
char caption2[CAPTION_LENGTH];
char caption3[CAPTION_LENGTH];

char value0[VALUE_LENGTH];
char value1[VALUE_LENGTH];
char value2[VALUE_LENGTH];
char value3[VALUE_LENGTH];

char stringstate[17] = "0000000000000000";
bool state[16];
char msg[10] = "";
uint8_t currentReadChannel = 0;
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

	sprintf(caption0, "Test 0");
	sprintf(caption1, "Test 1");
	sprintf(caption2, "Test 2");
	sprintf(caption3, "Test 3");

	sprintf(value0, "123");
	sprintf(value1, "456");
	sprintf(value2, "789");
	sprintf(value3, "012");

	GFX_SetFont(font_8x5);
	GFX_SetFontSize(1);

	SSD1306_ResetOLEDS();

	SSD1306_SetOLED(&oled[0]);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(BLACK);
	SSD1306_Display();

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

	SSD1306_SetOLED(&oled[1]);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(BLACK);
	SSD1306_Display();

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

	SSD1306_SetOLED(&oled[2]);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(BLACK);
	SSD1306_Display();

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

	SSD1306_SetOLED(&oled[3]);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(BLACK);
	SSD1306_Display();

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

	SSD1306_SetOLED(&oled[0]);
}

void drawScreen(uint8_t screenNumber){
	while (hspi1.hdmatx->State != HAL_DMA_STATE_READY) {}
	if(hspi1.hdmatx->State == HAL_DMA_STATE_READY)
	{
		char caption[CAPTION_LENGTH];
		char value[VALUE_LENGTH];
		switch (screenNumber){
			case 0:
				strcpy(caption, caption0);
				strcpy(value, value0);
				break;
			case 1:
				strcpy(caption, caption1);
				strcpy(value, value1);
				break;
			case 2:
				strcpy(caption, caption2);
				strcpy(value, value2);
				break;
			case 3:
				strcpy(caption, caption3);
				strcpy(value, value3);
				break;
		}
		SSD1306_SetOLED(&oled[screenNumber]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		GFX_DrawString(0,0, caption, WHITE, BLACK);
		GFX_SetFontSize(3);
		GFX_DrawString(0, 20, value, WHITE, BLACK);
		SSD1306_Display();
	}
}

void writeAddress(uint8_t channel){
	HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (GPIO_PinState)bitRead(channel, 0));
	HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (GPIO_PinState)bitRead(channel, 1));
	HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (GPIO_PinState)bitRead(channel, 2));
	HAL_GPIO_WritePin(MUX_D_GPIO_Port, MUX_D_Pin, (GPIO_PinState)bitRead(channel, 3));
}

void sendEncoderData(uint8_t encoderNumber, EncoderDirection dir, uint8_t velocity){
	char tmp[10] = "";
	char sign = '+';
	if (dir == Decrement) sign = '-';
	sprintf(tmp, "E%d%c%d\r\n", encoderNumber, sign, velocity);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tmp, sizeof(tmp));
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)tmp, sizeof(tmp));
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
	enc[0].setConstrains(0, 1023);
	enc[1].setConstrains(0, 1023);
	enc[2].setConstrains(0, 1023);
	enc[3].setConstrains(0, 1023);
	for (uint8_t i = 0; i < 4; i++){	//first scan to avoid "ghost rotations" before setting initial values
		writeAddress(i);
		HAL_Delay(1);
		enc[i].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET);
	}

	enc[0].setCallback(enc0Callback);
	enc[1].setCallback(enc1Callback);
	enc[2].setCallback(enc2Callback);
	enc[3].setCallback(enc3Callback);
	enc[0].setCounter(1);
	enc[1].setCounter(12);
	enc[2].setCounter(123);
	enc[3].setCounter(256);
	enc[0].setMaximumVelocityIncrement(64);
	enc[1].setMaximumVelocityIncrement(32);
	enc[2].setMaximumVelocityIncrement(16);
	enc[3].setMaximumVelocityIncrement(8);
}

void setup(){
	initializeEncoders();
	initializeOLEDs();
	for (uint8_t i = 0; i < 4; i++){
		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
		drawScreen(i);
	}
}

void Timer6Interrupt(){
	if (!readState){
		writeAddress(currentReadChannel);
		readState = true;
	}
	if(readState){
		enc[currentReadChannel].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET);
		enc[currentReadChannel].execute();
		++currentReadChannel;
		if (currentReadChannel == 4){
			currentReadChannel = 0;
		}
		readState = false;
	}
}

void UART_received(char* buf, uint16_t size){
	uint8_t selectedDisplay = 0;
	bool valuenotcaption = (buf[0] == 'V');

	if(buf[1] == '0') selectedDisplay = 0;
	if(buf[1] == '1') selectedDisplay = 1;
	if(buf[1] == '2') selectedDisplay = 2;
	if(buf[1] == '3') selectedDisplay = 3;

	if (valuenotcaption){
		char value[VALUE_LENGTH];
		for (int i = 0; i < VALUE_LENGTH; i++) value[i] = ' ';
		for (int i = 0; i < size + 1 && i < VALUE_LENGTH ; i++) value[i] = buf[i+2];
		switch (selectedDisplay){
			case 0: strcpy(value0, value); break;
			case 1: strcpy(value1, value); break;
			case 2: strcpy(value2, value); break;
			case 3: strcpy(value3, value); break;
		}
	}
	if (!valuenotcaption){
		char caption[CAPTION_LENGTH];
		for (int i = 0; i < CAPTION_LENGTH; i++) caption[i] = ' ';
		for (int i = 0; i < size + 1 && i < CAPTION_LENGTH ; i++) caption[i] = buf[i+2];
		switch (selectedDisplay){
			case 0: strcpy(caption0, caption); break;
			case 1: strcpy(caption1, caption); break;
			case 2: strcpy(caption2, caption); break;
			case 3: strcpy(caption3, caption); break;
		}
	}
	drawScreen(selectedDisplay);
}

void loop(){

}
