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
#include <math.h>
#include <string>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

struct OLEDdefinition oled1;
struct OLEDdefinition oled2;

char state[17] = "0000000000000000";

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
			GFX_DrawString(0,3, state, WHITE, BLACK);
			SSD1306_Display();


			while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}


			SSD1306_SetOLED(&oled2);
			SSD1306_Clear(BLACK);
			GFX_SetFontSize(1);
			GFX_DrawString(0,3, "drugi", WHITE, BLACK);
			SSD1306_Display();

			while(hspi1.hdmatx->State != HAL_DMA_STATE_READY){}
		}
}

void readInputStates(){
	for (uint8_t i = 0; i < 16; i++){
		HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, (GPIO_PinState)bitRead(i, 0));
		HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, (GPIO_PinState)bitRead(i, 1));
		HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, (GPIO_PinState)bitRead(i, 2));
		HAL_GPIO_WritePin(MUX_D_GPIO_Port, MUX_D_Pin, (GPIO_PinState)bitRead(i, 3));

		HAL_Delay(1);

		if(HAL_GPIO_ReadPin(MUX_Common_GPIO_Port, MUX_Common_Pin)){
			state[i] = '1';
		}
		else{
			state[i] = '0';
		}
	}
}

void setup(){
	initializeOLEDs();
}

void loop(){
	drawScreen();
	readInputStates();
	HAL_Delay(10);
}
