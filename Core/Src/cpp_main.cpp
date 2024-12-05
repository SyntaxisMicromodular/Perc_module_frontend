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


struct OLEDdefinition oled1;
struct OLEDdefinition oled2;

void initializeOLEDs(){
	oled1.PortCS = SSD1306_CS_GPIO_Port;
	oled1.PinCS = SSD1306_CS_Pin;

	oled2.PortCS = SSD1306_CS2_GPIO_Port;
	oled2.PinCS = SSD1306_CS2_Pin;

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
			GFX_DrawString(0,3, "pierwszy", WHITE, BLACK);
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

void setup(){
	initializeOLEDs();
}

void loop(){
	drawScreen();
	HAL_Delay(100);
}
