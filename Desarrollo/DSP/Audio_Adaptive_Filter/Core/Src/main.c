/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//CMSIS Library
#define ARM_MATH_CM4	//Must be defined here
#include "arm_math.h"

//FLASH Library
#include "MY_FLASH.h"

//Other includes
#include "math.h"
#include <string.h>
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//Numeric constants
#define CONST_MAX_INT 2147483648.0f	//2^31

//Machine state flags
#define STOP 0
#define ADAPT 1
#define FILTER 2
#define LOOPBACK 3

//Number of filter coefficients
#define FILTER_TAP_NUM 128

//Buffer size
#define BLOCK_SIZE_FLOAT 512
#define BLOCK_SIZE_U16 4*BLOCK_SIZE_FLOAT  //Since we are working with 2 channels, and each 32 bit word is divided in 2 words of 16 bits

//Vumeter levels
#define MAX_VOLUME_LEVEL 0.33f
#define HIGH_THRESHOLD 0.95*MAX_VOLUME_LEVEL
#define MED_THRESHOLD 0.18*MAX_VOLUME_LEVEL
#define LOW_THRESHOLD 0.01*MAX_VOLUME_LEVEL

//FLASH
#define FLASH_SECTOR 11
#define FLASH_START_ADRESS  0x080E0000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_i2s2_ext_rx;
DMA_HandleTypeDef hdma_spi2_tx;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

//Machine state variables
uint8_t state=STOP;

//I2S VARIABLES

uint16_t rxBuf[BLOCK_SIZE_U16 * 2];	//Input buffer
uint16_t txBuf[BLOCK_SIZE_U16 * 2];	//Output buffer
float l_buf_in[BLOCK_SIZE_FLOAT * 2];	//Float left input buffer
float r_buf_in[BLOCK_SIZE_FLOAT * 2];	//Float right input buffer
float l_buf_out[BLOCK_SIZE_FLOAT * 2];	//Float left output buffer
float r_buf_out[BLOCK_SIZE_FLOAT * 2];	//Float right output buffer
uint8_t callback_state = 0;	//Allows circular buffer

//I2S data pointers
int offset_r_ptr;	//uint_16 buffer read pointer
int offset_w_ptr, w_ptr;	//float buffer write pointer

//GENERAL FILTER VARIABLES

static float filter_taps[FILTER_TAP_NUM] = { 0 };	//Filter coefficients array

//ADAPTIVE FILTER VARIABLES

arm_lms_norm_instance_f32 adaptive_filter;	//Adaptive filter instance
float mu = 0.001f;	//Mu step of adaptive filter
float mse=0;	//Mean Square Error
float filter_state[BLOCK_SIZE_FLOAT + FILTER_TAP_NUM - 1];	//Adaptive filter state (size is always number_of_samples + number_of_fir_tabs - 1)

//FIR FILTER VARIABLES

arm_fir_instance_f32 firsettings_l, firsettings_r;
float fir_l_state[BLOCK_SIZE_FLOAT + FILTER_TAP_NUM - 1];	//Fir state size is always number_of_samples + number_of_fir_tabs - 1
float fir_r_state[BLOCK_SIZE_FLOAT + FILTER_TAP_NUM - 1];	//Fir state size is always number_of_samples + number_of_fir_tabs - 1

//UART VARIABLES

uint8_t uart_buffer_tx[16];	//UART Tx buffer
uint8_t uart_buffer_rx[32] = {0};	//UART Rx buffer
uint8_t received_command[] = "";
uint8_t rxByte;
uint16_t rxIdx;
uint8_t rxFlag;

float rms_test;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void receiveString(void);
void interpretCommand(void);
void AudioProcess(void);
void TransmitCoefs(void);
void AdaptInit(void);
void FilterInit(void);
void Stop(void);
void Adapt(void);
void Filter(void);
void Loopback(void);
void vumeterR(void);
void vumeterL(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_I2S2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  //Wait for other devices to initialize
  HAL_Delay(1000);

  //Define FLASH area
  MY_FLASH_SetSectorAddrs(FLASH_SECTOR, FLASH_START_ADRESS);

  //Start i2s with 2048 samples transmission => 4096*u16 words
  HAL_I2SEx_TransmitReceive_DMA(&hi2s2, txBuf, rxBuf, BLOCK_SIZE_U16);

  //Start UART Rx
  __HAL_UART_FLUSH_DRREGISTER(&huart2);
  HAL_UART_Receive_DMA(&huart2, &rxByte, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (callback_state != 0)
	  {
		  AudioProcess();
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, L_LOW_LED_Pin|L_MED_LED_Pin|L_HIGH_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, R_LOW_LED_Pin|R_MED_LED_Pin|R_HIGH_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : L_LOW_LED_Pin L_MED_LED_Pin L_HIGH_LED_Pin */
  GPIO_InitStruct.Pin = L_LOW_LED_Pin|L_MED_LED_Pin|L_HIGH_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : R_LOW_LED_Pin R_MED_LED_Pin R_HIGH_LED_Pin */
  GPIO_InitStruct.Pin = R_LOW_LED_Pin|R_MED_LED_Pin|R_HIGH_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {

	callback_state = 1;
}

void HAL_I2SEx_TxRxCpltCallback(I2S_HandleTypeDef *hi2s) {

	callback_state = 2;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (state==ADAPT)
	{
		sprintf((char *)uart_buffer_tx,"&%.5f#\n",mse);	//Copy new MSE in UART buffer
		HAL_UART_Transmit_IT(&huart2, uart_buffer_tx, strlen((char *) uart_buffer_tx));
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2) {
		__HAL_UART_FLUSH_DRREGISTER(&huart2);
		if (rxByte == '#' || rxIdx >= 31)
		{
			uart_buffer_rx[rxIdx] = rxByte;
			receiveString();
			interpretCommand();
			for (int i = 0; i < 24; i++)
				uart_buffer_rx[i] = 0;
			rxIdx = 0;
		}
		else
		{
			uart_buffer_rx[rxIdx] = rxByte;
			rxIdx++;
		}
	}
}

void receiveString()
{
	char *p1, *p2;
	p1 = strstr((char*)uart_buffer_rx, "%");
	if (p1) {
		p2 = strstr(p1, "#");
		if (p2)
			snprintf((char*)received_command, p2 - p1 + 1, "%.*s", (int) (p2 - p1 - 1), (p1 + 1));
	}
}

void interpretCommand()
{
    if (strcmp((char*)received_command,"ST")==0)
    {
    	state=STOP;
    }
    else if (strcmp((char*)received_command,"AF")==0)
    {
    	mu=0.001f;
    	AdaptInit();
    	state=ADAPT;
    }
    else if (strcmp((char*)received_command,"AS")==0)
    {
    	mu=0.0001f;
		AdaptInit();
		state=ADAPT;
    }
    else if (strcmp((char*)received_command,"SV")==0)
	{
    	MY_FLASH_WriteN(0, filter_taps, FILTER_TAP_NUM, DATA_TYPE_32);
	}
    else if (strcmp((char*)received_command,"FI")==0)
    {
    	state=FILTER;
    	FilterInit();
    }
    else if (strcmp((char*)received_command,"LB")==0)
	{
		state=LOOPBACK;
	}
}

void AdaptInit()
{
	mse=0;

	for (int i=0; i<FILTER_TAP_NUM; i++)
	{
		filter_taps[i]=0;
	}

	//Init adaptive filter structure
	arm_lms_norm_init_f32(&adaptive_filter, FILTER_TAP_NUM, &filter_taps[0], &filter_state[0], mu, BLOCK_SIZE_FLOAT);

	//Initiate MSE transmission
	sprintf((char *)uart_buffer_tx,"&%.5f#\n",mse);
	HAL_UART_Transmit_IT(&huart2, uart_buffer_tx, strlen((char *) uart_buffer_tx));
}

void FilterInit()
{
	//Read filter stored value
	MY_FLASH_ReadN(0, filter_taps, FILTER_TAP_NUM, DATA_TYPE_32);

	//Init FIR structure
	arm_fir_init_f32(&firsettings_l, FILTER_TAP_NUM, &filter_taps[0], &fir_l_state[0], BLOCK_SIZE_FLOAT);
	arm_fir_init_f32(&firsettings_r, FILTER_TAP_NUM, &filter_taps[0], &fir_r_state[0], BLOCK_SIZE_FLOAT);
}

void AudioProcess()
{
	//Decide if it was half or cplt callback
	if (callback_state == 1)	//First half of the buffer
	{
		offset_r_ptr = 0;
		offset_w_ptr = 0;
		w_ptr = 0;
	}
	else if (callback_state == 2) 	//Second half of the buffer
	{
		offset_r_ptr = BLOCK_SIZE_U16;
		offset_w_ptr = BLOCK_SIZE_FLOAT;
		w_ptr = BLOCK_SIZE_FLOAT;
	}

	//Restore input sample buffer to float array
	for (int i = offset_r_ptr; i < offset_r_ptr + BLOCK_SIZE_U16; i = i + 4)
	{
		l_buf_in[w_ptr] = ((float) ((int) (rxBuf[i] << 16) | rxBuf[i + 1]))/CONST_MAX_INT;
		r_buf_in[w_ptr] = ((float) ((int) (rxBuf[i + 2] << 16) | rxBuf[i + 3]))/CONST_MAX_INT;
		w_ptr++;
	}

	vumeterR();
	vumeterL();
	arm_rms_f32 (&l_buf_in[offset_w_ptr], BLOCK_SIZE_FLOAT, &rms_test);

	switch (state)
	{
	case STOP: Stop();
		break;
	case ADAPT:	Adapt();
		break;
	case FILTER: Filter();
		break;
	case LOOPBACK: Loopback();
		break;
	default:
		break;
	}

	//Restore processed float-array to output sample-buffer
	w_ptr = offset_w_ptr;

	//Write I2S output buffer
	for (int i = offset_r_ptr; i < offset_r_ptr + BLOCK_SIZE_U16; i = i + 4)
	{
		l_buf_out[w_ptr]=l_buf_out[w_ptr]*CONST_MAX_INT;
		r_buf_out[w_ptr]=r_buf_out[w_ptr]*CONST_MAX_INT;

		txBuf[i] = (((int) l_buf_out[w_ptr]) >> 16) & 0xFFFF;
		txBuf[i + 1] = ((int) l_buf_out[w_ptr]) & 0xFFFF;
		txBuf[i + 2] = (((int) r_buf_out[w_ptr]) >> 16) & 0xFFFF;
		txBuf[i + 3] = ((int) r_buf_out[w_ptr]) & 0xFFFF;
		w_ptr++;
	}

	callback_state = 0;
}

void Stop()
{
	for (int i=offset_w_ptr; i<offset_w_ptr+BLOCK_SIZE_FLOAT; i=i+1)
		{
			l_buf_out[i]=0;
			r_buf_out[i]=0;
		}
}

void Adapt()
{
	//Adaptive Filter

	//right In  --> input signal (x)
	//left In   --> desired signal (d)
	//right Out --> output signal (y)
	//left Out  --> error signal (e)

	//arm_lms_f32 (const arm_lms_instance_f32 *S, const float32_t *pSrc, float32_t *pRef, float32_t *pOut, float32_t *pErr, uint32_t blockSize)

	arm_lms_norm_f32(&adaptive_filter, &r_buf_in[offset_w_ptr], &l_buf_in[offset_w_ptr], &r_buf_out[offset_w_ptr], &l_buf_out[offset_w_ptr], BLOCK_SIZE_FLOAT);

	//MSE calculation

	mse=0;
	for (int i=offset_w_ptr; i<offset_w_ptr+BLOCK_SIZE_FLOAT; i=i+1)
	{
		mse=mse+((float)l_buf_out[i]*(float)l_buf_out[i]);
	}
	mse=mse/(float)BLOCK_SIZE_FLOAT;
}

void Filter()
{
	//FIR
	arm_fir_f32 (&firsettings_l, &l_buf_in[offset_w_ptr], &l_buf_out[offset_w_ptr],BLOCK_SIZE_FLOAT);
	arm_fir_f32 (&firsettings_r, &r_buf_in[offset_w_ptr], &r_buf_out[offset_w_ptr],BLOCK_SIZE_FLOAT);
}

void Loopback()
{
	for (int i=offset_w_ptr; i<offset_w_ptr+BLOCK_SIZE_FLOAT; i=i+1)
		{
			l_buf_out[i]=l_buf_in[i];
			r_buf_out[i]=r_buf_in[i];
		}
}

void vumeterR()
{
	float rms_value;
	arm_rms_f32 (&r_buf_in[offset_w_ptr], BLOCK_SIZE_FLOAT, &rms_value);

	if (rms_value>=LOW_THRESHOLD && rms_value<MED_THRESHOLD)
		{
			HAL_GPIO_WritePin(GPIOD, R_LOW_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, R_MED_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, R_HIGH_LED_Pin, GPIO_PIN_RESET);
		}
		else if (rms_value>=MED_THRESHOLD && rms_value<HIGH_THRESHOLD)
		{
			HAL_GPIO_WritePin(GPIOD, R_LOW_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, R_MED_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, R_HIGH_LED_Pin, GPIO_PIN_RESET);
		}
		else if (rms_value>=HIGH_THRESHOLD)
		{
			HAL_GPIO_WritePin(GPIOD, R_LOW_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, R_MED_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, R_HIGH_LED_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOD, R_LOW_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, R_MED_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, R_HIGH_LED_Pin, GPIO_PIN_RESET);
		}
}

void vumeterL()
{
	float rms_value;
	arm_rms_f32 (&l_buf_in[offset_w_ptr], BLOCK_SIZE_FLOAT, &rms_value);

	if (rms_value>=LOW_THRESHOLD && rms_value<MED_THRESHOLD)
	{
		HAL_GPIO_WritePin(GPIOC, L_LOW_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, L_MED_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, L_HIGH_LED_Pin, GPIO_PIN_RESET);
	}
	else if (rms_value>=MED_THRESHOLD && rms_value<HIGH_THRESHOLD)
	{
		HAL_GPIO_WritePin(GPIOC, L_LOW_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, L_MED_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, L_HIGH_LED_Pin, GPIO_PIN_RESET);
	}
	else if (rms_value>=HIGH_THRESHOLD)
	{
		HAL_GPIO_WritePin(GPIOC, L_LOW_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, L_MED_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, L_HIGH_LED_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, L_LOW_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, L_MED_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, L_HIGH_LED_Pin, GPIO_PIN_RESET);
	}
}

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
	while (1) {
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
