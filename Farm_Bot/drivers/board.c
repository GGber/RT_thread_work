/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-26     ChenYong     first version
 */

#include <board.h>
#include <rtthread.h>

//void SystemClock_Config(void)  //内部时钟
//{
//    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//    {
//      Error_Handler();
//    }
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//
//#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0)
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                                 |RCC_CLOCKTYPE_PCLK1;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//#elif defined(SOC_SERIES_STM32H7)
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
//                                 |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
//    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
//    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
//#else
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//#endif
//
//    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//    {
//      Error_Handler();
//    }
//}

void SystemClock_Config(void)//外部时钟
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}
