/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define VK36N4D_INT_Pin 				GPIO_PIN_13 //version >v02
#define VK36N4D_INT_GPIO_Port 			GPIOB

#define VK36N4D_INT_EXTI_IRQn 			EXTI4_15_IRQn //EXTI2_3_IRQn



//TM1723 CONTROL GPIO
#define TM1723_STB_Pin 						GPIO_PIN_4
#define TM1723_STB_GPIO_Port 				GPIOA

#define TM1723_SCLK_Pin 					GPIO_PIN_5
#define TM1723_SCLK_GPIO_Port 				GPIOA

#define TM1723_DIO_Pin 						GPIO_PIN_6
#define TM1723_DIO_GPIO_Port 				GPIOA

//KEY
	
#define KEY_POWER_Pin 						GPIO_PIN_10// "power" key
#define KEY_POWER_GPIO_Port 				GPIOB

#define KEY_MODE_Pin 						GPIO_PIN_11//"model"key
#define KEY_MODE_GPIO_Port   				GPIOB

#define KEY_ADD_Pin 						GPIO_PIN_2// "+"key
#define KEY_ADD_GPIO_Port 					GPIOB

#define KEY_DEC_Pin 						GPIO_PIN_12 //"-" key
#define KEY_DEC_GPIO_Port 					GPIOB


//lcd back light
#define BACK_LIGHT_Pin 						GPIO_PIN_9  //version.02
#define BACK_LIGHT_GPIO_Port 				GPIOB
//KEY of indication LED
#define LED_INFO_POWER_Pin 						GPIO_PIN_3
#define LED_INFO_POWER_GPIO_Port 				GPIOA

#define LED_INFO_MODE_Pin 						GPIO_PIN_2
#define LED_INFO_MODE_GPIO_Port 				GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
