#include "initDevice.h"

#include "main.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void CAN1_Init(void);
void CAN2_Init(void);

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


void initDevice(void) {
	MX_GPIO_Init();
	CAN1_Init();
	CAN2_Init();
	SystemClock_Config();
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
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 15;
	  RCC_OscInitStruct.PLL.PLLN = 144;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 5;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	  {
	    Error_Handler();
	  }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;


  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
}


void CAN2_Init(void) {
	CAN_HandleTypeDef *handle = &hcan2;

	handle->Instance = CAN2;
	handle->Init.Prescaler = 6;
	handle->Init.Mode = CAN_MODE_NORMAL;
	handle->Init.SyncJumpWidth = CAN_SJW_2TQ;
	handle->Init.TimeSeg1 = CAN_BS1_2TQ;
	handle->Init.TimeSeg2 = CAN_BS2_2TQ;
	handle->Init.TimeTriggeredMode = DISABLE;
	handle->Init.AutoBusOff = DISABLE;
	handle->Init.AutoWakeUp = DISABLE;
	handle->Init.AutoRetransmission = ENABLE;
	handle->Init.ReceiveFifoLocked = DISABLE;
	handle->Init.TransmitFifoPriority = DISABLE;

	if (HAL_CAN_Init(handle) != HAL_OK)
		Error_Handler();

	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank 				= 0;
	canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
	canfilterconfig.FilterMaskIdHigh 		= 0x0000;
	canfilterconfig.FilterMaskIdLow 		= 0x0000;
	canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(handle, &canfilterconfig) != HAL_OK)
		Error_Handler();
}

void CAN1_Init(void) {
	CAN_HandleTypeDef *handle = &hcan1;

	handle->Instance = CAN1;
	handle->Init.Prescaler = 16;
	handle->Init.Mode = CAN_MODE_NORMAL;
	handle->Init.SyncJumpWidth = CAN_SJW_1TQ;
	handle->Init.TimeSeg1 = CAN_BS1_1TQ;
	handle->Init.TimeSeg2 = CAN_BS2_1TQ;
	handle->Init.TimeTriggeredMode = DISABLE;
	handle->Init.AutoBusOff = DISABLE;
	handle->Init.AutoWakeUp = DISABLE;
	handle->Init.AutoRetransmission = ENABLE;
	handle->Init.ReceiveFifoLocked = DISABLE;
	handle->Init.TransmitFifoPriority = DISABLE;

	if (HAL_CAN_Init(handle) != HAL_OK)
		Error_Handler();

	CAN_FilterTypeDef canfilterconfig;
	canfilterconfig.FilterActivation 		= CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank 				= 0;
	canfilterconfig.FilterFIFOAssignment 	= CAN_FILTER_FIFO1;
	canfilterconfig.FilterMaskIdHigh 		= 0x0000;
	canfilterconfig.FilterMaskIdLow 		= 0x0000;
	canfilterconfig.FilterMode 				= CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale 			= CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(handle, &canfilterconfig) != HAL_OK)
		Error_Handler();
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
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
