/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SSD1306_DC_Pin GPIO_PIN_4
#define SSD1306_DC_GPIO_Port GPIOA
#define SSD1306_RESET_Pin GPIO_PIN_5
#define SSD1306_RESET_GPIO_Port GPIOA
#define SSD1306_CS_Pin GPIO_PIN_6
#define SSD1306_CS_GPIO_Port GPIOA
#define SSD1306_CS2_Pin GPIO_PIN_0
#define SSD1306_CS2_GPIO_Port GPIOB
#define SSD1306_CS3_Pin GPIO_PIN_1
#define SSD1306_CS3_GPIO_Port GPIOB
#define SSD1306_CS4_Pin GPIO_PIN_8
#define SSD1306_CS4_GPIO_Port GPIOA
#define MUX_A_Pin GPIO_PIN_11
#define MUX_A_GPIO_Port GPIOA
#define MUX_B_Pin GPIO_PIN_12
#define MUX_B_GPIO_Port GPIOA
#define MUX_Common2_Pin GPIO_PIN_4
#define MUX_Common2_GPIO_Port GPIOB
#define MUX_C_Pin GPIO_PIN_5
#define MUX_C_GPIO_Port GPIOB
#define MUX_D_Pin GPIO_PIN_6
#define MUX_D_GPIO_Port GPIOB
#define MUX_Common_Pin GPIO_PIN_7
#define MUX_Common_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
