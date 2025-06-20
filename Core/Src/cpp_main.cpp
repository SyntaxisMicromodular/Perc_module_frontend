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
#include <math.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

struct OLEDdefinition oled[4];
Encoder enc[4];
Button btn[4];

const int CAPTION_LENGTH = 20;
const int VALUE_LENGTH = 7;
const int DISPLAYS = 4;

char caption[DISPLAYS][CAPTION_LENGTH];

char value[DISPLAYS][VALUE_LENGTH];

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

void drawScreen(uint8_t screenNumber){
	while (hspi1.hdmatx->State != HAL_DMA_STATE_READY) {}
	if(hspi1.hdmatx->State == HAL_DMA_STATE_READY)
	{
		char caption_fn[CAPTION_LENGTH];
		char value_fn[VALUE_LENGTH];
		for (int i = 0; i < CAPTION_LENGTH; i++) caption_fn[i] = caption[screenNumber][i];
		for (int i = 0; i < VALUE_LENGTH; i++) value_fn[i] = value[screenNumber][i];
		//strcpy(caption_fn, caption[screenNumber]);
		//strcpy(value_fn, value[screenNumber]);

		SSD1306_SetOLED(&oled[screenNumber]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		GFX_DrawString(0,0, caption_fn, WHITE, BLACK);
		GFX_SetFontSize(3);
		GFX_DrawString(0, 20, value_fn, WHITE, BLACK);
		SSD1306_Display();
	}
}

void writeAddress(uint8_t channel){
	HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (GPIO_PinState)bitRead(channel, 0));
	HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (GPIO_PinState)bitRead(channel, 1));
	HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (GPIO_PinState)bitRead(channel, 2));
	HAL_GPIO_WritePin(MUX_D_GPIO_Port, MUX_D_Pin, (GPIO_PinState)bitRead(channel, 3));
}

void sendData(uint8_t* data, uint16_t size){
	//while (huart1.hdmatx->State != HAL_DMA_STATE_READY) {}
	//HAL_UART_Transmit_DMA(&huart1, data, size);
	//HAL_UART_Transmit_DMA(&huart2, data, size);
	HAL_UART_Transmit(&huart1, data, size, 1);
	HAL_UART_Transmit(&huart2, data, size, 1);
}

void sendEncoderData(uint8_t encoderNumber, EncoderDirection dir, uint8_t velocity){
	char tmp[10] = "";
	char sign = dir == Increment ? '+' : '-';
	sprintf(tmp, "E%d%c%d\r\n", encoderNumber, sign, velocity);
	sendData((uint8_t*)tmp, strlen(tmp));
	sprintf(value[encoderNumber], "%c%d", sign, velocity);
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
		writeAddress(i);
		HAL_Delay(1);
		enc[i].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET);
	}

	enc[0].OnChange = enc0Callback;
	enc[1].OnChange = enc1Callback;
	enc[2].OnChange = enc2Callback;
	enc[3].OnChange = enc3Callback;
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
	writeAddress(4);
	btn[0].refresh(HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_RESET);
	btn[2].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_RESET);
	writeAddress(5);
	btn[1].refresh(HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_RESET);
	btn[3].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_RESET);

	btn[0].OnChange = btn0Callback;
	btn[1].OnChange = btn1Callback;
	btn[2].OnChange = btn2Callback;
	btn[3].OnChange = btn3Callback;
}

void setup(){
	HAL_Delay(100);
	initializeEncoders();
	initializeButtons();
	initializeOLEDs();
	for (uint8_t i = 0; i < DISPLAYS; i++){
		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
		drawScreen(i);
	}
}

void EncoderInterrupt(){
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

	writeAddress(4);
	btn[0].refresh(HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_RESET);
	btn[2].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_RESET);
	writeAddress(5);
	btn[1].refresh(HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_RESET);
	btn[3].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_RESET);

	for (uint8_t i = 0; i < 4; i++) btn[i].execute();
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

void loop(){
	for (int i = 0; i < DISPLAYS; i++) drawScreen(i);
	HAL_Delay(20);
}
