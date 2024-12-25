/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "lcd.h"
#include "arm_math.h"
#include "ADS1292.h"
#include "IIRFilter.h"
#include "FIRFilter.h"
#include "stdio.h"
#include "arm_math.h"
#include "bsp_dwt.h"
#include "transfer_function.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint8_t ads1292_recive_flag;

extern int16_t IIR_Result;
extern int16_t FIRResult;
int heart_beat=0;
double heart_rate;

#define ECG_COUNT 1024
int16_t ECG_Signal[ECG_COUNT];
uint16_t ecg_num;

#define FFT_LENGTH		1024
float maxfft;
uint32_t maxindex = 0;
float FFT_InputBufmy[FFT_LENGTH*2];	
float FFT_OutputBufmy[FFT_LENGTH];	
float FFT_OutputBufmy1[FFT_LENGTH];	
arm_cfft_radix4_instance_f32 scfft;




void fftcal()
{
		for(int i=0; i < FFT_LENGTH; i++)
{
    FFT_InputBufmy[2*i]=(uint16_t)(ECG_Signal[i]);
    FFT_InputBufmy[2*i+1]=0;				 
}
arm_cfft_radix4_f32(&scfft,FFT_InputBufmy);					
arm_cmplx_mag_f32(FFT_InputBufmy,FFT_OutputBufmy,FFT_LENGTH);	

for(int i = 0;i<FFT_LENGTH;i++)
{
	FFT_OutputBufmy[i] =FFT_OutputBufmy[i]*0.36;
	if(FFT_OutputBufmy[i]<1)
	{
	FFT_OutputBufmy[i] = 0;
	
	}
		if(FFT_OutputBufmy[i]>500)
	{
	FFT_OutputBufmy[i] = 500;
	
	}
	

}
}
Second_Order_TF_t tf;

    // ?????????,?? c[0] = 2, c[1] = 1, c[2] = 1
    float coefficients[3] = {2.0f, 1.0f, 1.0f};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
	ADS1292R_Init();
	ADS1292R_Work();
	
	FIRInit();
	DWT_Init(168);
	lcd_init();
	lcd_clear(BLACK);
	lcd_display_dir(1);
			arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);  
Second_Order_TF_Init(&tf, coefficients);			
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		drawCurve(lcd_data, 460);
//		HAL_Delay(500);
		

//22000000/(56(cycle)+12.5)/1024/40(抽样) = 7.48
arm_max_f32(&FFT_OutputBufmy[5],1000,&maxfft,&maxindex);


		
		if(ads1292_recive_flag == 1)
		{	
	  	ads1292_recive_flag = 0;
			//心电与心率信号采集
			{
				data_trans();
				IIRFilter(ECGRawData[0]);
				FIRFilter(ECGRawData[0]);
				heart_beat = heartbeat_check(FIRResult);
				heart_rate = calc_heartbeat_rate(heart_beat);
				
				if(ecg_num < ECG_COUNT)
				{
					ECG_Signal[ecg_num] = ECGRawData[0]; //记录波形
					IIR_Result =  Second_Order_TF_Calculate(&tf, ECGRawData[0]);
					ecg_num++;
				}
				else
				{
					ecg_num = 0;
					drawCurve(ECG_Signal, ECG_COUNT); //画波形
					fftcal();
				}
				
				lcd_show_string(10, 10, 240, 16, 16, "HR: ", WHITE);
				lcd_show_num(32, 10, (uint32_t)heart_rate, 3, 16, WHITE);
				
				printf("A = %d,B = %d,C = %d\n", ECGRawData[0], IIR_Result, FIRResult);
				
			}
			
			//内部方波采集
//			{
//				data_trans();
//				
//				if(ecg_num < ECG_COUNT)
//				{
//					ECG_Signal[ecg_num] = ECGRawData[0]; //记录波形
//					ecg_num++;
//				}
//				else
//				{
//					ecg_num = 0;
//					drawCurve(ECG_Signal, ECG_COUNT); //画波形
//				}
//				
//				//printf("%d,%d\n", ECGRawData[0], ECGRawData[1]);
//			}
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
