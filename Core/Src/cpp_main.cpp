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

struct OLEDdefinition oled1;
struct OLEDdefinition oled2;

Encoder enc1;
Encoder enc2;
Encoder enc3;
Encoder enc4;

char stringstate[17] = "0000000000000000";
bool state[16];
char msg[10] = "";
uint8_t currentReadChannel = 0;
bool readState = false;

void initializeOLEDs(){
	oled1.PortCS = SSD1306_CS_GPIO_Port;
	oled1.PinCS = SSD1306_CS_Pin;
	oled1.rotation_90 = false;

	oled2.PortCS = SSD1306_CS2_GPIO_Port;
	oled2.PinCS = SSD1306_CS2_Pin;
	oled2.rotation_90 = true;

	GFX_SetFont(font_8x5);
	GFX_SetFontSize(1);

	SSD1306_ResetOLEDS();

	SSD1306_SetOLED(&oled1);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(BLACK);
	GFX_DrawString(0,3, "pierwszy", WHITE, BLACK);
	SSD1306_Display();

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}

	SSD1306_SetOLED(&oled2);
	SSD1306_SpiInit(&hspi1);
	SSD1306_RotateDisplay(1);
	SSD1306_Clear(WHITE);
	GFX_DrawString(0,3, "drugi", WHITE, BLACK);
	SSD1306_Display();

	SSD1306_SetOLED(&oled1);

	while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
}

void drawScreen(){
	if(hspi1.hdmatx->State == HAL_DMA_STATE_READY)
		{
			SSD1306_SetOLED(&oled1);
			SSD1306_Clear(BLACK);
			GFX_SetFontSize(1);
			GFX_DrawString(0,3, msg, WHITE, BLACK);
			SSD1306_Display();


			while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}


			SSD1306_SetOLED(&oled2);
			SSD1306_Clear(BLACK);
			GFX_SetFontSize(1);
			char tmp[20] = "";
			sprintf(tmp, "Enc1 = %d", enc1.getCounter());
			GFX_DrawString(0,3, tmp, WHITE, BLACK);
			sprintf(tmp, "Enc2 = %d", enc2.getCounter());
			GFX_DrawString(0,11, tmp, WHITE, BLACK);
			sprintf(tmp, "Enc3 = %d", enc3.getCounter());
			GFX_DrawString(0,19, tmp, WHITE, BLACK);
			sprintf(tmp, "Enc4 = %d", enc4.getCounter());
			GFX_DrawString(0,27, tmp, WHITE, BLACK);
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

void enc1Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc1--\n", 10);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	}
	if (dir == Increment){
		strncpy(msg, "enc1++\n", 10);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc2Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc2--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc2++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc3Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc3--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc3++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void enc4Callback(EncoderDirection dir, uint8_t velocity){
	if (dir == Decrement){
		strncpy(msg, "enc4--\n", 10);
	}
	if (dir == Increment){
		strncpy(msg, "enc4++\n", 10);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)msg, sizeof(msg));
}

void emptyCallback(EncoderDirection dir, uint8_t velocity){

}

void initializeEncoders(){
	enc1.setCallback(enc1Callback);
	enc2.setCallback(enc2Callback);
	enc3.setCallback(enc3Callback);
	enc4.setCallback(enc4Callback);
	enc1.setConstrains(0, 1023);
	enc2.setConstrains(0, 1023);
	enc3.setConstrains(0, 1023);
	enc4.setConstrains(0, 1023);
	enc1.setCounter(1);
	enc2.setCounter(1);
	enc3.setCounter(1);
	enc4.setCounter(1);
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
		switch (currentReadChannel){
		case 0: enc1.refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET); break;
		case 1: enc2.refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET); break;
		case 2: enc3.refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET); break;
		case 3: enc4.refresh(HAL_GPIO_ReadPin(MUX_Common2_GPIO_Port, MUX_Common2_Pin) == GPIO_PIN_SET, HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin) == GPIO_PIN_SET); break;
		default: break;
		}
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
