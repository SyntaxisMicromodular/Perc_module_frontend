/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SSD1306_DC_Pin|SSD1306_RESET_Pin|SSD1306_CS_Pin|SSD1306_CS4_Pin
                          |MUX_A_Pin|MUX_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SSD1306_CS2_Pin|SSD1306_CS3_Pin|GPIO_PIN_3|MUX_C_Pin
                          |MUX_D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SSD1306_DC_Pin SSD1306_RESET_Pin SSD1306_CS_Pin SSD1306_CS4_Pin
                           MUX_A_Pin MUX_B_Pin */
  GPIO_InitStruct.Pin = SSD1306_DC_Pin|SSD1306_RESET_Pin|SSD1306_CS_Pin|SSD1306_CS4_Pin
                          |MUX_A_Pin|MUX_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SSD1306_CS2_Pin SSD1306_CS3_Pin PB3 MUX_C_Pin
                           MUX_D_Pin */
  GPIO_InitStruct.Pin = SSD1306_CS2_Pin|SSD1306_CS3_Pin|GPIO_PIN_3|MUX_C_Pin
                          |MUX_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MUX_Common2_Pin MUX_Common_Pin */
  GPIO_InitStruct.Pin = MUX_Common2_Pin|MUX_Common_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
