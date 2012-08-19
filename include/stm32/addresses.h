#ifndef CPPMETA_STM32_ADDRESSES_H
#define CPPMETA_STM32_ADDRESSES_H

#include <stdint.h>
#include "meta/value.h"

namespace CppMeta {
  namespace Stm32 {
    namespace Addresses {
      using FLASH =            UInt32<0x08000000>; /*!< FLASH(up to 1 MB) base address in the alias region                         */
      using CCMDATARAM =       UInt32<0x10000000>; /*!< CCM(core coupled memory) data RAM(64 KB) base address in the alias region  */
      using SRAM1 =            UInt32<0x20000000>; /*!< SRAM1(112 KB) base address in the alias region                             */
      using SRAM2 =            UInt32<0x2001C000>; /*!< SRAM2(16 KB) base address in the alias region                              */
      using PERIPH =           UInt32<0x40000000>; /*!< Peripheral base address in the alias region                                */
      using BKPSRAM =          UInt32<0x40024000>; /*!< Backup SRAM(4 KB) base address in the alias region                         */
      using FSMC_R =           UInt32<0xA0000000>; /*!< FSMC registers base address                                                */

      using CCMDATARAM_BB =    UInt32<0x12000000>; /*!< CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region  */
      using SRAM1_BB =         UInt32<0x22000000>; /*!< SRAM1(112 KB) base address in the bit-band region                             */
      using SRAM2_BB =         UInt32<0x2201C000>; /*!< SRAM2(16 KB) base address in the bit-band region                              */
      using PERIPH_BB =        UInt32<0x42000000>; /*!< Peripheral base address in the bit-band region                                */
      using BKPSRAM_BB =       UInt32<0x42024000>; /*!< Backup SRAM(4 KB) base address in the bit-band region                         */

/* Legacy defines */
      using SRAM =             SRAM1;
      using SRAM_BB =          SRAM1;

/*!< Peripheral memory map */
      using APB1PERIPH =       PERIPH;
      using APB2PERIPH =       Plus<PERIPH, UInt32<0x00010000>>;
      using AHB1PERIPH =       Plus<PERIPH, UInt32<0x00020000>>;
      using AHB2PERIPH =       Plus<PERIPH, UInt32<0x10000000>>;

/*!< APB1 peripherals */
      using TIM2 =             Plus<APB1PERIPH, UInt32<0x0000>>;
      using TIM3 =             Plus<APB1PERIPH, UInt32<0x0400>>;
      using TIM4 =             Plus<APB1PERIPH, UInt32<0x0800>>;
      using TIM5 =             Plus<APB1PERIPH, UInt32<0x0C00>>;
      using TIM6 =             Plus<APB1PERIPH, UInt32<0x1000>>;
      using TIM7 =             Plus<APB1PERIPH, UInt32<0x1400>>;
      using TIM12 =            Plus<APB1PERIPH, UInt32<0x1800>>;
      using TIM13 =            Plus<APB1PERIPH, UInt32<0x1C00>>;
      using TIM14 =            Plus<APB1PERIPH, UInt32<0x2000>>;
      using RTC =              Plus<APB1PERIPH, UInt32<0x2800>>;
      using WWDG =             Plus<APB1PERIPH, UInt32<0x2C00>>;
      using IWDG =             Plus<APB1PERIPH, UInt32<0x3000>>;
      using I2S2ext =          Plus<APB1PERIPH, UInt32<0x3400>>;
      using SPI2 =             Plus<APB1PERIPH, UInt32<0x3800>>;
      using SPI3 =             Plus<APB1PERIPH, UInt32<0x3C00>>;
      using I2S3ext =          Plus<APB1PERIPH, UInt32<0x4000>>;
      using USART2 =           Plus<APB1PERIPH, UInt32<0x4400>>;
      using USART3 =           Plus<APB1PERIPH, UInt32<0x4800>>;
      using UART4 =            Plus<APB1PERIPH, UInt32<0x4C00>>;
      using UART5 =            Plus<APB1PERIPH, UInt32<0x5000>>;
      using I2C1 =             Plus<APB1PERIPH, UInt32<0x5400>>;
      using I2C2 =             Plus<APB1PERIPH, UInt32<0x5800>>;
      using I2C3 =             Plus<APB1PERIPH, UInt32<0x5C00>>;
      using CAN1 =             Plus<APB1PERIPH, UInt32<0x6400>>;
      using CAN2 =             Plus<APB1PERIPH, UInt32<0x6800>>;
      using PWR =              Plus<APB1PERIPH, UInt32<0x7000>>;
      using DAC =              Plus<APB1PERIPH, UInt32<0x7400>>;

/*!< APB2 peripherals */
      using TIM1 =             Plus<APB2PERIPH, UInt32<0x0000>>;
      using TIM8 =             Plus<APB2PERIPH, UInt32<0x0400>>;
      using USART1 =           Plus<APB2PERIPH, UInt32<0x1000>>;
      using USART6 =           Plus<APB2PERIPH, UInt32<0x1400>>;
      using ADC1 =             Plus<APB2PERIPH, UInt32<0x2000>>;
      using ADC2 =             Plus<APB2PERIPH, UInt32<0x2100>>;
      using ADC3 =             Plus<APB2PERIPH, UInt32<0x2200>>;
      using ADC =              Plus<APB2PERIPH, UInt32<0x2300>>;
      using SDIO =             Plus<APB2PERIPH, UInt32<0x2C00>>;
      using SPI1 =             Plus<APB2PERIPH, UInt32<0x3000>>;
      using SYSCFG =           Plus<APB2PERIPH, UInt32<0x3800>>;
      using EXTI =             Plus<APB2PERIPH, UInt32<0x3C00>>;
      using TIM9 =             Plus<APB2PERIPH, UInt32<0x4000>>;
      using TIM10 =            Plus<APB2PERIPH, UInt32<0x4400>>;
      using TIM11 =            Plus<APB2PERIPH, UInt32<0x4800>>;

/*!< AHB1 peripherals */
      using GPIOA =            Plus<AHB1PERIPH, UInt32<0x0000>>;
      using GPIOB =            Plus<AHB1PERIPH, UInt32<0x0400>>;
      using GPIOC =            Plus<AHB1PERIPH, UInt32<0x0800>>;
      using GPIOD =            Plus<AHB1PERIPH, UInt32<0x0C00>>;
      using GPIOE =            Plus<AHB1PERIPH, UInt32<0x1000>>;
      using GPIOF =            Plus<AHB1PERIPH, UInt32<0x1400>>;
      using GPIOG =            Plus<AHB1PERIPH, UInt32<0x1800>>;
      using GPIOH =            Plus<AHB1PERIPH, UInt32<0x1C00>>;
      using GPIOI =            Plus<AHB1PERIPH, UInt32<0x2000>>;
      using CRC =              Plus<AHB1PERIPH, UInt32<0x3000>>;
      using RCC =              Plus<AHB1PERIPH, UInt32<0x3800>>;
      using FLASH_R =          Plus<AHB1PERIPH, UInt32<0x3C00>>;
      using DMA1 =             Plus<AHB1PERIPH, UInt32<0x6000>>;
      using DMA1_Stream0 =     Plus<DMA1, UInt32<0x010>>;
      using DMA1_Stream1 =     Plus<DMA1, UInt32<0x028>>;
      using DMA1_Stream2 =     Plus<DMA1, UInt32<0x040>>;
      using DMA1_Stream3 =     Plus<DMA1, UInt32<0x058>>;
      using DMA1_Stream4 =     Plus<DMA1, UInt32<0x070>>;
      using DMA1_Stream5 =     Plus<DMA1, UInt32<0x088>>;
      using DMA1_Stream6 =     Plus<DMA1, UInt32<0x0A0>>;
      using DMA1_Stream7 =     Plus<DMA1, UInt32<0x0B8>>;
      using DMA2 =             Plus<AHB1PERIPH, UInt32<0x6400>>;
      using DMA2_Stream0 =     Plus<DMA2, UInt32<0x010>>;
      using DMA2_Stream1 =     Plus<DMA2, UInt32<0x028>>;
      using DMA2_Stream2 =     Plus<DMA2, UInt32<0x040>>;
      using DMA2_Stream3 =     Plus<DMA2, UInt32<0x058>>;
      using DMA2_Stream4 =     Plus<DMA2, UInt32<0x070>>;
      using DMA2_Stream5 =     Plus<DMA2, UInt32<0x088>>;
      using DMA2_Stream6 =     Plus<DMA2, UInt32<0x0A0>>;
      using DMA2_Stream7 =     Plus<DMA2, UInt32<0x0B8>>;
      using ETH =              Plus<AHB1PERIPH, UInt32<0x8000>>;
      using ETH_MAC =          ETH;
      using ETH_MMC =          Plus<ETH, UInt32<0x0100>>;
      using ETH_PTP =          Plus<ETH, UInt32<0x0700>>;
      using ETH_DMA =          Plus<ETH, UInt32<0x1000>>;

/*!< AHB2 peripherals */
      using DCMI =             Plus<AHB2PERIPH, UInt32<0x50000>>;
      using CRYP =             Plus<AHB2PERIPH, UInt32<0x60000>>;
      using HASH =             Plus<AHB2PERIPH, UInt32<0x60400>>;
      using RNG =              Plus<AHB2PERIPH, UInt32<0x60800>>;

/*!< FSMC Bankx registers base address */
      using FSMC_Bank1_R =     Plus<FSMC_R, UInt32<0x0000>>;
      using FSMC_Bank1E_R =    Plus<FSMC_R, UInt32<0x0104>>;
      using FSMC_Bank2_R =     Plus<FSMC_R, UInt32<0x0060>>;
      using FSMC_Bank3_R =     Plus<FSMC_R, UInt32<0x0080>>;
      using FSMC_Bank4_R =     Plus<FSMC_R, UInt32<0x00A0>>;

/* Debug MCU registers base address */
      using DBGMCU =           UInt32<0xE0042000>;
    }
  }
}

#endif
