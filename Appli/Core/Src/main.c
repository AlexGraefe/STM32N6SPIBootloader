 /**
 ******************************************************************************
 * @file    main.c
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <string.h>
#include <unistd.h>

#include "console.h"
#include "stm32n6xx_hal_gpio.h"
#include "timer.h"
#include "iris.h"
#include "fuseprogramming.h"
#include "gpio.h"
#include "iac.h"
// #include "mylcd.h"
#include "mpu.h"
// #include "sd_card.h"
#include "security.h"
#include "stm32n6xx_hal.h"
#include "stm32n6xx_hal_spi.h"
#include "system_clock.h"
// #include "venc.h"

// #include "cmw_camera.h"
#include "stm32n6570_discovery_bus.h"
// #include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery_xspi.h"
// #include "stm32n6570_discovery_sd.h"
#include "stm32n6570_discovery.h"
// #include "stm32_lcd.h"
// #include "stm32_lcd_ex.h"

// #include "h264encapi.h"
// #include "stm32n6xx_ll_venc.h"

// #include "app_camerapipeline.h"
#include "main.h"
#include <stdio.h>
#include "app_config.h"
// #include "crop_img.h"
// #include "stlogo.h"

#ifndef APP_GIT_SHA1_STRING
#define APP_GIT_SHA1_STRING "dev"
#endif
#ifndef APP_VERSION_STRING
#define APP_VERSION_STRING "unversioned"
#endif

static void Hardware_init(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  Hardware_init();
  uint8_t data[256];
  for (uint16_t i = 0; i < 256; i++) {
    data[i] = 2;
  }

  uint8_t read_data[256] = {1.0};
  uint8_t k = 10;
  int ret;
  while (1) {
    for (uint16_t i = 0; i < 256; i++) {
      data[i] = (k+i)%256;
    }
    printf("W: %u\n", k);
    ret = BSP_XSPI_NOR_Erase_Block(0, 0x5A20000LU, MX66UW1G45G_ERASE_64K);
    if (ret != BSP_ERROR_NONE) {
      printf("Error erasing flash: %d\n", ret);
    }

    ret = BSP_XSPI_NOR_Write(0, data, 0x5A20000LU, 256);
    if (ret != BSP_ERROR_NONE) {
      printf("Error writing to flash: %d\n", ret);
    }
    ret = BSP_XSPI_NOR_Read(0, read_data, 0x5A20000LU, 256);
    if (ret != BSP_ERROR_NONE) {
      printf("Error reading from flash: %d\n", ret);
    }
    for (int i = 0; i < 16; i++) {
      printf("%u, %u\n", read_data[i], data[i]);
    }
    printf("\n");
    HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
    HAL_Delay(10000);
    k++;
  }
}

/**
  * @brief XSPI MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hxspi: XSPI handle pointer
  * @retval None
  */
void HAL_XSPI_MspInit(XSPI_HandleTypeDef* hxspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hxspi->Instance==XSPI2)
  {
    /* USER CODE BEGIN XSPI2_MspInit 0 */

    /* USER CODE END XSPI2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_XSPI2;
    PeriphClkInitStruct.Xspi2ClockSelection = RCC_XSPI2CLKSOURCE_IC3;
    PeriphClkInitStruct.ICSelection[RCC_IC3].ClockSelection = RCC_ICCLKSOURCE_PLL4;
    PeriphClkInitStruct.ICSelection[RCC_IC3].ClockDivider = 1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while (1);
    }

    /* Peripheral clock enable */
    __HAL_RCC_XSPIM_CLK_ENABLE();
    __HAL_RCC_XSPI2_CLK_ENABLE();

    __HAL_RCC_GPION_CLK_ENABLE();
    /**XSPI2 GPIO Configuration
    PN4     ------> XSPIM_P2_IO2
    PN6     ------> XSPIM_P2_CLK
    PN8     ------> XSPIM_P2_IO4
    PN0     ------> XSPIM_P2_DQS0
    PN3     ------> XSPIM_P2_IO1
    PN5     ------> XSPIM_P2_IO3
    PN1     ------> XSPIM_P2_NCS1
    PN9     ------> XSPIM_P2_IO5
    PN2     ------> XSPIM_P2_IO0
    PN10     ------> XSPIM_P2_IO6
    PN11     ------> XSPIM_P2_IO7
    */
    GPIO_InitStruct.Pin = OCTOSPI_IO2_Pin|OCTOSPI_CLK_Pin|OCTOSPI_IO4_Pin|OCTOSPI_DQS_Pin
                          |OCTOSPI_IO1_Pin|OCTOSPI_IO3_Pin|OCTOSPI_NCS_Pin|OCTOSPI_IO5_Pin
                          |OCTOSPI_IO0_Pin|OCTOSPI_IO6_Pin|OCTOSPI_IO7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_XSPIM_P2;
    HAL_GPIO_Init(GPION, &GPIO_InitStruct);

    /* USER CODE BEGIN XSPI2_MspInit 1 */

    /* USER CODE END XSPI2_MspInit 1 */

  }

}

/**
  * @brief XSPI MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hxspi: XSPI handle pointer
  * @retval None
  */
void HAL_XSPI_MspDeInit(XSPI_HandleTypeDef* hxspi)
{
  if(hxspi->Instance==XSPI2)
  {
    /* USER CODE BEGIN XSPI2_MspDeInit 0 */

    /* USER CODE END XSPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_XSPIM_CLK_DISABLE();
    __HAL_RCC_XSPI2_CLK_DISABLE();

    /**XSPI2 GPIO Configuration
    PN4     ------> XSPIM_P2_IO2
    PN6     ------> XSPIM_P2_CLK
    PN8     ------> XSPIM_P2_IO4
    PN0     ------> XSPIM_P2_DQS0
    PN3     ------> XSPIM_P2_IO1
    PN5     ------> XSPIM_P2_IO3
    PN1     ------> XSPIM_P2_NCS1
    PN9     ------> XSPIM_P2_IO5
    PN2     ------> XSPIM_P2_IO0
    PN10     ------> XSPIM_P2_IO6
    PN11     ------> XSPIM_P2_IO7
    */
    HAL_GPIO_DeInit(GPION, OCTOSPI_IO2_Pin|OCTOSPI_CLK_Pin|OCTOSPI_IO4_Pin|OCTOSPI_DQS_Pin
                          |OCTOSPI_IO1_Pin|OCTOSPI_IO3_Pin|OCTOSPI_NCS_Pin|OCTOSPI_IO5_Pin
                          |OCTOSPI_IO0_Pin|OCTOSPI_IO6_Pin|OCTOSPI_IO7_Pin);

    /* USER CODE BEGIN XSPI2_MspDeInit 1 */

    /* USER CODE END XSPI2_MspDeInit 1 */
  }

}

/**
//   * @brief XSPI2 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_XSPI2_Init(void)
// {

//   /* USER CODE BEGIN XSPI2_Init 0 */

//   /* USER CODE END XSPI2_Init 0 */

//   XSPIM_CfgTypeDef sXspiManagerCfg = {0};

//   /* USER CODE BEGIN XSPI2_Init 1 */

//   /* USER CODE END XSPI2_Init 1 */
//   /* XSPI2 parameter configuration*/
//   hxspi2.Instance = XSPI2;
//   hxspi2.Init.FifoThresholdByte = 4;
//   hxspi2.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
//   hxspi2.Init.MemoryType = HAL_XSPI_MEMTYPE_MICRON;
//   hxspi2.Init.MemorySize = HAL_XSPI_SIZE_1GB;
//   hxspi2.Init.ChipSelectHighTimeCycle = 1;
//   hxspi2.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
//   hxspi2.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
//   hxspi2.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
//   hxspi2.Init.ClockPrescaler = 0;
//   hxspi2.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
//   hxspi2.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
//   hxspi2.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
//   hxspi2.Init.MaxTran = 0;
//   hxspi2.Init.Refresh = 0;
//   hxspi2.Init.MemorySelect = HAL_XSPI_CSSEL_NCS1;
//   if (HAL_XSPI_Init(&hxspi2) != HAL_OK)
//   {
//     while (1);
//   }
//   sXspiManagerCfg.nCSOverride = HAL_XSPI_CSSEL_OVR_NCS1;
//   sXspiManagerCfg.IOPort = HAL_XSPIM_IOPORT_2;
//   sXspiManagerCfg.Req2AckTime = 1;
//   if (HAL_XSPIM_Config(&hxspi2, &sXspiManagerCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//   {
//     while (1);
//   }
//   /* USER CODE BEGIN XSPI2_Init 2 */

//   /* USER CODE END XSPI2_Init 2 */

// }

static void Hardware_init(void)
{
  /* enable MPU configuration to create non cacheable sections */
  MPU_Config();

  /* Power on ICACHE */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_ICACTIVE_Msk;

  SCB_EnableICache();

#if defined(USE_DCACHE)
  /* Power on DCACHE */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_DCACTIVE_Msk;
  SCB_EnableDCache();
#endif

  HAL_Init();

  SystemClock_Config();

  HAL_Init();

  /* Set all required IPs as secure privileged */
  Security_Config();

  GPIO_Config();

  CONSOLE_Config();
  printf("\n\n\n\n");

  PRINTF_START("IRIS Init");
  Timer_Config();
  PRINTF_END("IRIS Init");

  // PRINTF_START("IRIS Init");
  // iris_config();
  // iris_handshake_blocking();
  // PRINTF_END("IRIS Init");

  Fuse_Programming();

  /* External RAM */
  // PRINTF_START("External Memory Init");
  // BSP_XSPI_RAM_Init(0);
  // BSP_XSPI_RAM_EnableMemoryMappedMode(0);

    BSP_XSPI_NOR_Init_t NOR_Init;
  NOR_Init.InterfaceMode = BSP_XSPI_NOR_OPI_MODE;
  NOR_Init.TransferRate = BSP_XSPI_NOR_DTR_TRANSFER;
  BSP_XSPI_NOR_Init(0, &NOR_Init);
  // BSP_XSPI_NOR_EnableMemoryMappedMode(0);

  // MX_XSPI2_Init();

  // PRINTF_END("External Memory Init");

  // // init SD card
  // PRINTF_START("SD Card Init");
  // SD_Card_Init();
  // PRINTF_END("SD Card Init");

  IAC_Config();
  set_clk_sleep_mode();

}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  UNUSED(file);
  UNUSED(line);
  __BKPT(0);
  while (1)
  {
  }
}

#endif
