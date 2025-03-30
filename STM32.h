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
#include "stm32f4xx_hal.h"

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
#define PW_LED_Pin GPIO_PIN_13
#define PW_LED_GPIO_Port GPIOC
#define DHT11_Pin GPIO_PIN_1
#define DHT11_GPIO_Port GPIOA
#define DS18B20_Pin GPIO_PIN_2
#define DS18B20_GPIO_Port GPIOA
#define WL_1_Pin GPIO_PIN_4
#define WL_1_GPIO_Port GPIOA
#define WL_2_Pin GPIO_PIN_5
#define WL_2_GPIO_Port GPIOA
#define WL_3_Pin GPIO_PIN_6
#define WL_3_GPIO_Port GPIOA
#define WL_PW_Pin GPIO_PIN_7
#define WL_PW_GPIO_Port GPIOA
#define MQ135_Pin GPIO_PIN_1
#define MQ135_GPIO_Port GPIOB
#define ESP32_Tx_Pin GPIO_PIN_15
#define ESP32_Tx_GPIO_Port GPIOA
#define ESP32_Rx_Pin GPIO_PIN_3
#define ESP32_Rx_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
