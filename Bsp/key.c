#include "key.h"

//uint8_t	KEY_Scan(void)
//{
//	if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0)	//检测按键是否被按下
//	{	
//		HAL_Delay(10);	//延时消抖
//		if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0)	//再次检测是否为低电平
//		{
//			while(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == 0);	//等待按键放开
//			return KEY_ON;	//返回按键按下标志
//		}
//	}
//	return KEY_OFF;	
//}

