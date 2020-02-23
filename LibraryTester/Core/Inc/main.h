/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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

void MyInit();
int MyMain();


/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_USER_LED_Pin GPIO_PIN_13
#define GPIO_USER_LED_GPIO_Port GPIOC
#define GPIO_PUSH_BUTTON_3_Pin GPIO_PIN_4
#define GPIO_PUSH_BUTTON_3_GPIO_Port GPIOA
#define GPIO_PUSH_BUTTON_3_EXTI_IRQn EXTI4_IRQn
#define GPIO_LATCH_Pin GPIO_PIN_6
#define GPIO_LATCH_GPIO_Port GPIOA
#define GPIO_PUSH_BUTTON_1_Pin GPIO_PIN_0
#define GPIO_PUSH_BUTTON_1_GPIO_Port GPIOB
#define GPIO_PUSH_BUTTON_1_EXTI_IRQn EXTI0_IRQn
#define GPIO_PUSH_BUTTON_2_Pin GPIO_PIN_1
#define GPIO_PUSH_BUTTON_2_GPIO_Port GPIOB
#define GPIO_PUSH_BUTTON_2_EXTI_IRQn EXTI1_IRQn
#define GPIO_PUSH_BUTTON_4_Pin GPIO_PIN_11
#define GPIO_PUSH_BUTTON_4_GPIO_Port GPIOB
#define GPIO_PUSH_BUTTON_4_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_KEYPAD_COLUMN_4_Pin GPIO_PIN_14
#define GPIO_KEYPAD_COLUMN_4_GPIO_Port GPIOB
#define GPIO_KEYPAD_COLUMN_3_Pin GPIO_PIN_8
#define GPIO_KEYPAD_COLUMN_3_GPIO_Port GPIOA
#define GPIO_KEYPAD_COLUMN_2_Pin GPIO_PIN_9
#define GPIO_KEYPAD_COLUMN_2_GPIO_Port GPIOA
#define GPIO_KEYPAD_COLUMN_1_Pin GPIO_PIN_10
#define GPIO_KEYPAD_COLUMN_1_GPIO_Port GPIOA
#define GPIO_KEYPAD_ROW_4_Pin GPIO_PIN_15
#define GPIO_KEYPAD_ROW_4_GPIO_Port GPIOA
#define GPIO_KEYPAD_ROW_3_Pin GPIO_PIN_3
#define GPIO_KEYPAD_ROW_3_GPIO_Port GPIOB
#define GPIO_KEYPAD_ROW_2_Pin GPIO_PIN_4
#define GPIO_KEYPAD_ROW_2_GPIO_Port GPIOB
#define GPIO_KEYPAD_ROW_1_Pin GPIO_PIN_5
#define GPIO_KEYPAD_ROW_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
