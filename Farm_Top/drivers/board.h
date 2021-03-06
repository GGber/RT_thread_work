/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-26     ChenYong     first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include <stm32f1xx.h>
#include "drv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------- UART CONFIG BEGIN --------------------------*/

/** After configuring corresponding uart or uart dma, you can use it.
  *
  * STEP 1, define macro define related to the serial port opening based on the serial port number
  *                 such as     #define BSP_USING_UATR1
  *
  * STEP 2, if you need to support serial port DMA, according to serial port number to define serial port tx/rx DMA function
  *                 such as     #define BSP_UART1_RX_USING_DMA
  *             Attention please, if you want using SERIAL DMA, make sure RT_SERIAL_USING_DMA is defined
  *
  * STEP 3, according to the corresponding pin of serial port, modify the related serial port information
  *                 such as     #define UART1_TX_PORT            GPIOX   ->   GPIOA
  *                             #define UART1_RX_PORT            GPIOX   ->   GPIOA
  *                             #define UART1_TX_PIN        GPIO_PIN_X   ->   GPIO_PIN_9
  *                             #define UART1_RX_PIN        GPIO_PIN_X   ->   GPIO_PIN_10
  *
  */
#define BSP_USING_UART1
#define BSP_USING_UART2
#define BSP_USING_UART3
#define BSP_USING_UART4
#define BSP_USING_UART5


#if defined(BSP_USING_UART1)
#define UART1_TX_PORT       GPIOA
#define UART1_RX_PORT       GPIOA
#define UART1_TX_PIN        GPIO_PIN_9
#define UART1_RX_PIN        GPIO_PIN_10
#endif
#if defined(BSP_USING_UART2)
#define UART2_TX_PORT       GPIOA
#define UART2_RX_PORT       GPIOA
#define UART2_TX_PIN        GPIO_PIN_2
#define UART2_RX_PIN        GPIO_PIN_3
#endif
#if defined(BSP_USING_UART3)
#define UART3_TX_PORT       GPIOB
#define UART3_RX_PORT       GPIOB
#define UART3_TX_PIN        GPIO_PIN_10
#define UART3_RX_PIN        GPIO_PIN_11
#endif
#if defined(BSP_USING_UART4)
#define UART4_TX_PORT       GPIOC
#define UART4_RX_PORT       GPIOC
#define UART4_TX_PIN        GPIO_PIN_10
#define UART4_RX_PIN        GPIO_PIN_11
#endif
#if defined(BSP_USING_UART5)
#define UART5_TX_PORT       GPIOC
#define UART5_RX_PORT       GPIOD
#define UART5_TX_PIN        GPIO_PIN_12
#define UART5_RX_PIN        GPIO_PIN_2
#endif


#if defined(BSP_USING_UART6)
#define UART6_TX_PORT       GPIOX
#define UART6_RX_PORT       GPIOX
#define UART6_TX_PIN        GPIO_PIN_X
#define UART6_RX_PIN        GPIO_PIN_X
#endif
#if defined(BSP_USING_UART7)
#define UART7_TX_PORT       GPIOX
#define UART7_RX_PORT       GPIOX
#define UART7_TX_PIN        GPIO_PIN_X
#define UART7_RX_PIN        GPIO_PIN_X
#endif
#if defined(BSP_USING_UART8)
#define UART8_TX_PORT       GPIOX
#define UART8_RX_PORT       GPIOX
#define UART8_TX_PIN        GPIO_PIN_X
#define UART8_RX_PIN        GPIO_PIN_X
#endif
#if defined(BSP_USING_LPUART1)
#define LPUART1_TX_PORT     GPIOX
#define LPUART1_RX_PORT     GPIOX
#define LPUART1_TX_PIN      GPIO_PIN_X
#define LPUART1_RX_PIN      GPIO_PIN_X
#endif
/*-------------------------- UART CONFIG END --------------------------*/

/*-------------------------- ROM/RAM CONFIG BEGIN --------------------------*/

#define STM32_FLASH_START_ADRESS       ((uint32_t)0x8000000)
#define STM32_FLASH_SIZE               (256)
#define STM32_FLASH_END_ADDRESS        ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE * 1024))

#define STM32_SRAM1_SIZE               (64)
#define STM32_SRAM1_START              (0x20000000)
#define STM32_SRAM1_END                (STM32_SRAM1_START + STM32_SRAM1_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$RW_IRAM1$ZI$Limit;
#define HEAP_BEGIN      ((void *)&Image$RW_IRAM1$ZI$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif

#define HEAP_END                       STM32_SRAM1_END

/*-------------------------- ROM/RAM CONFIG END --------------------------*/

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */

