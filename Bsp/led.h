#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_ON 	  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)			// 输出低电平，点亮LED	
#define LED_OFF 	  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)				// 输出高电平，关闭LED	
#define LED_Toggle	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)		// LED状态翻转

#endif /* __LED_H */

