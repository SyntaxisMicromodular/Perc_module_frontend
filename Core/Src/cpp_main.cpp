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
#include <string>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

struct OLEDdefinition oled[4];
Encoder enc[4];

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

void drawScreen(){
	char tmp[20] = "";
	if(hspi1.hdmatx->State == HAL_DMA_STATE_READY)
	{
		SSD1306_SetOLED(&oled[0]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		sprintf(tmp, "A = %d", enc[0].getCounter());
		GFX_DrawString(0,3, tmp, WHITE, BLACK);
		SSD1306_Display();

		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

		SSD1306_SetOLED(&oled[1]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		sprintf(tmp, "D = %d", enc[1].getCounter());
		GFX_DrawString(0,3, tmp, WHITE, BLACK);
		SSD1306_Display();

		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

		SSD1306_SetOLED(&oled[2]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		sprintf(tmp, "S = %d", enc[2].getCounter());
		GFX_DrawString(0,3, tmp, WHITE, BLACK);
		SSD1306_Display();

		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

		SSD1306_SetOLED(&oled[3]);
		SSD1306_Clear(BLACK);
		GFX_SetFontSize(2);
		sprintf(tmp, "R = %d", enc[3].getCounter());
		GFX_DrawString(0,3, tmp, WHITE, BLACK);
		SSD1306_Display();

		while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
	}
}

void writeAddress(uint8_t channel){
	HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (GPIO_PinState)bitRead(channel, 0));
	HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (GPIO_PinState)bitRead(channel, 1));
	HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (GPIO_PinState)bitRead(channel, 2));
	HAL_GPIO_WritePin(MUX_D_GPIO_Port, MUX_D_Pin, (GPIO_PinState)bitRead(channel, 3));
}

void enc0Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc[0]--\n", 10);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	}
	if (dir == Increment){
		strncpy(msg, "enc[0]++\n", 10);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc1Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc[1]--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc[1]++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc2Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc[2]--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc[2]++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc3Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc[3]--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc[3]++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void emptyCallback(EncoderDirection dir, uint8_t velocity){

}

void initializeEncoders(){
	enc[0].setCallback(enc0Callback);
	enc[1].setCallback(enc1Callback);
	enc[2].setCallback(enc2Callback);
	enc[3].setCallback(enc3Callback);
	enc[0].setConstrains(0, 1023);
	enc[1].setConstrains(0, 1023);
	enc[2].setConstrains(0, 1023);
	enc[3].setConstrains(0, 1023);
	for (uint8_t i = 0; i < 4; i++){	//first scan to avoid "ghost rotations" before setting initial values
		writeAddress(i);
		HAL_Delay(1);
		enc[i].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET);
	}
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
	initializeOLEDs();
	initializeEncoders();
}

void Timer6Interrupt(){
	if (!readState){
		writeAddress(currentReadChannel);
		readState = true;
	}
	if(readState){
		enc[currentReadChannel].refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET);
		++currentReadChannel;
		if (currentReadChannel == 4){
			currentReadChannel = 0;
		}
		readState = false;
	}
}

void UART_received(char* buf, uint16_t size){
	char emptymsg[20] = "";
	strncpy(msg, emptymsg, 10);
	strncpy(msg, buf, size > 10 ? 10 : size - 2);
}

void loop(){
	drawScreen();
}
