#ifndef CPPMETA_STM32_RCC_H
#define CPPMETA_STM32_RCC_H

#include <stdint.h>
#include "meta/value.h"
#include "stm32/registers.h"
#include "stm32/addresses.h"
#include "stm32/irqs.h"

namespace CppMeta {
  namespace Stm32 {
    struct RCC_Type {
      volatile uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
      volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
      volatile uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
      volatile uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
      volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
      volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
      volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
               uint32_t RESERVED0;     /*!< Reserved, 0x1C                                                                    */
      volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
      volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
               uint32_t RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
      volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
      volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
      volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
               uint32_t RESERVED2;     /*!< Reserved, 0x3C                                                                    */
      volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
      volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
               uint32_t RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
      volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
      volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
      volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
               uint32_t RESERVED4;     /*!< Reserved, 0x5C                                                                    */
      volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
      volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
               uint32_t RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
      volatile uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
      volatile uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
               uint32_t RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
      volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
      volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
    };
    
    struct Rcc : Registers<RCC_Type, Addresses::RCC> {
      using CR_HSION                = UInt32<0x00000001>;
      using CR_HSIRDY               = UInt32<0x00000002>;
      using CR_HSITRIM              = UInt32<0x000000F8>;
      using CR_HSITRIM_0            = UInt32<0x00000008>; /*!<Bit 0 */
      using CR_HSITRIM_1            = UInt32<0x00000010>; /*!<Bit 1 */
      using CR_HSITRIM_2            = UInt32<0x00000020>; /*!<Bit 2 */
      using CR_HSITRIM_3            = UInt32<0x00000040>; /*!<Bit 3 */
      using CR_HSITRIM_4            = UInt32<0x00000080>; /*!<Bit 4 */
      using CR_HSICAL               = UInt32<0x0000FF00>;
      using CR_HSICAL_0             = UInt32<0x00000100>; /*!<Bit 0 */
      using CR_HSICAL_1             = UInt32<0x00000200>; /*!<Bit 1 */
      using CR_HSICAL_2             = UInt32<0x00000400>; /*!<Bit 2 */
      using CR_HSICAL_3             = UInt32<0x00000800>; /*!<Bit 3 */
      using CR_HSICAL_4             = UInt32<0x00001000>; /*!<Bit 4 */
      using CR_HSICAL_5             = UInt32<0x00002000>; /*!<Bit 5 */
      using CR_HSICAL_6             = UInt32<0x00004000>; /*!<Bit 6 */
      using CR_HSICAL_7             = UInt32<0x00008000>; /*!<Bit 7 */
      using CR_HSEON                = UInt32<0x00010000>;
      using CR_HSERDY               = UInt32<0x00020000>;
      using CR_HSEBYP               = UInt32<0x00040000>;
      using CR_CSSON                = UInt32<0x00080000>;
      using CR_PLLON                = UInt32<0x01000000>;
      using CR_PLLRDY               = UInt32<0x02000000>;
      using CR_PLLI2SON             = UInt32<0x04000000>;
      using CR_PLLI2SRDY            = UInt32<0x08000000>;
      using PLLCFGR_PLLM            = UInt32<0x0000003F>;
      using PLLCFGR_PLLM_0          = UInt32<0x00000001>;
      using PLLCFGR_PLLM_1          = UInt32<0x00000002>;
      using PLLCFGR_PLLM_2          = UInt32<0x00000004>;
      using PLLCFGR_PLLM_3          = UInt32<0x00000008>;
      using PLLCFGR_PLLM_4          = UInt32<0x00000010>;
      using PLLCFGR_PLLM_5          = UInt32<0x00000020>;
      using PLLCFGR_PLLN            = UInt32<0x00007FC0>;
      using PLLCFGR_PLLN_0          = UInt32<0x00000040>;
      using PLLCFGR_PLLN_1          = UInt32<0x00000080>;
      using PLLCFGR_PLLN_2          = UInt32<0x00000100>;
      using PLLCFGR_PLLN_3          = UInt32<0x00000200>;
      using PLLCFGR_PLLN_4          = UInt32<0x00000400>;
      using PLLCFGR_PLLN_5          = UInt32<0x00000800>;
      using PLLCFGR_PLLN_6          = UInt32<0x00001000>;
      using PLLCFGR_PLLN_7          = UInt32<0x00002000>;
      using PLLCFGR_PLLN_8          = UInt32<0x00004000>;
      using PLLCFGR_PLLP            = UInt32<0x00030000>;
      using PLLCFGR_PLLP_0          = UInt32<0x00010000>;
      using PLLCFGR_PLLP_1          = UInt32<0x00020000>;
      using PLLCFGR_PLLSRC          = UInt32<0x00400000>;
      using PLLCFGR_PLLSRC_HSE      = UInt32<0x00400000>;
      using PLLCFGR_PLLSRC_HSI      = UInt32<0x00000000>;
      using PLLCFGR_PLLQ            = UInt32<0x0F000000>;
      using PLLCFGR_PLLQ_0          = UInt32<0x01000000>;
      using PLLCFGR_PLLQ_1          = UInt32<0x02000000>;
      using PLLCFGR_PLLQ_2          = UInt32<0x04000000>;
      using PLLCFGR_PLLQ_3          = UInt32<0x08000000>;
      using CFGR_SW                 = UInt32<0x00000003>;         /*!< SW[1:0] bits (System clock Switch) */
      using CFGR_SW_0               = UInt32<0x00000001>;         /*!< Bit 0 */
      using CFGR_SW_1               = UInt32<0x00000002>;         /*!< Bit 1 */
      using CFGR_SW_HSI             = UInt32<0x00000000>;         /*!< HSI selected as system clock */
      using CFGR_SW_HSE             = UInt32<0x00000001>;         /*!< HSE selected as system clock */
      using CFGR_SW_PLL             = UInt32<0x00000002>;         /*!< PLL selected as system clock */
      using CFGR_SWS                = UInt32<0x0000000C>;         /*!< SWS[1:0] bits (System Clock Switch Status) */
      using CFGR_SWS_0              = UInt32<0x00000004>;         /*!< Bit 0 */
      using CFGR_SWS_1              = UInt32<0x00000008>;         /*!< Bit 1 */
      using CFGR_SWS_HSI            = UInt32<0x00000000>;         /*!< HSI oscillator used as system clock */
      using CFGR_SWS_HSE            = UInt32<0x00000004>;         /*!< HSE oscillator used as system clock */
      using CFGR_SWS_PLL            = UInt32<0x00000008>;         /*!< PLL used as system clock */
      using CFGR_HPRE               = UInt32<0x000000F0>;         /*!< HPRE[3:0] bits (AHB prescaler) */
      using CFGR_HPRE_0             = UInt32<0x00000010>;         /*!< Bit 0 */
      using CFGR_HPRE_1             = UInt32<0x00000020>;         /*!< Bit 1 */
      using CFGR_HPRE_2             = UInt32<0x00000040>;         /*!< Bit 2 */
      using CFGR_HPRE_3             = UInt32<0x00000080>;         /*!< Bit 3 */
      using CFGR_HPRE_DIV1          = UInt32<0x00000000>;         /*!< SYSCLK not divided */
      using CFGR_HPRE_DIV2          = UInt32<0x00000080>;         /*!< SYSCLK divided by 2 */
      using CFGR_HPRE_DIV4          = UInt32<0x00000090>;         /*!< SYSCLK divided by 4 */
      using CFGR_HPRE_DIV8          = UInt32<0x000000A0>;         /*!< SYSCLK divided by 8 */
      using CFGR_HPRE_DIV16         = UInt32<0x000000B0>;         /*!< SYSCLK divided by 16 */
      using CFGR_HPRE_DIV64         = UInt32<0x000000C0>;         /*!< SYSCLK divided by 64 */
      using CFGR_HPRE_DIV128        = UInt32<0x000000D0>;         /*!< SYSCLK divided by 128 */
      using CFGR_HPRE_DIV256        = UInt32<0x000000E0>;         /*!< SYSCLK divided by 256 */
      using CFGR_HPRE_DIV512        = UInt32<0x000000F0>;         /*!< SYSCLK divided by 512 */
      using CFGR_PPRE1              = UInt32<0x00001C00>;         /*!< PRE1[2:0] bits (APB1 prescaler) */
      using CFGR_PPRE1_0            = UInt32<0x00000400>;         /*!< Bit 0 */
      using CFGR_PPRE1_1            = UInt32<0x00000800>;         /*!< Bit 1 */
      using CFGR_PPRE1_2            = UInt32<0x00001000>;         /*!< Bit 2 */
      using CFGR_PPRE1_DIV1         = UInt32<0x00000000>;         /*!< HCLK not divided */
      using CFGR_PPRE1_DIV2         = UInt32<0x00001000>;         /*!< HCLK divided by 2 */
      using CFGR_PPRE1_DIV4         = UInt32<0x00001400>;         /*!< HCLK divided by 4 */
      using CFGR_PPRE1_DIV8         = UInt32<0x00001800>;         /*!< HCLK divided by 8 */
      using CFGR_PPRE1_DIV16        = UInt32<0x00001C00>;         /*!< HCLK divided by 16 */
      using CFGR_PPRE2              = UInt32<0x0000E000>;         /*!< PRE2[2:0] bits (APB2 prescaler) */
      using CFGR_PPRE2_0            = UInt32<0x00002000>;         /*!< Bit 0 */
      using CFGR_PPRE2_1            = UInt32<0x00004000>;         /*!< Bit 1 */
      using CFGR_PPRE2_2            = UInt32<0x00008000>;         /*!< Bit 2 */
      using CFGR_PPRE2_DIV1         = UInt32<0x00000000>;         /*!< HCLK not divided */
      using CFGR_PPRE2_DIV2         = UInt32<0x00008000>;         /*!< HCLK divided by 2 */
      using CFGR_PPRE2_DIV4         = UInt32<0x0000A000>;         /*!< HCLK divided by 4 */
      using CFGR_PPRE2_DIV8         = UInt32<0x0000C000>;         /*!< HCLK divided by 8 */
      using CFGR_PPRE2_DIV16        = UInt32<0x0000E000>;         /*!< HCLK divided by 16 */
      using CFGR_RTCPRE             = UInt32<0x001F0000>;
      using CFGR_RTCPRE_0           = UInt32<0x00010000>;
      using CFGR_RTCPRE_1           = UInt32<0x00020000>;
      using CFGR_RTCPRE_2           = UInt32<0x00040000>;
      using CFGR_RTCPRE_3           = UInt32<0x00080000>;
      using CFGR_RTCPRE_4           = UInt32<0x00100000>;
      using CFGR_MCO1               = UInt32<0x00600000>;
      using CFGR_MCO1_0             = UInt32<0x00200000>;
      using CFGR_MCO1_1             = UInt32<0x00400000>;
      using CFGR_I2SSRC             = UInt32<0x00800000>;
      using CFGR_MCO1PRE            = UInt32<0x07000000>;
      using CFGR_MCO1PRE_0          = UInt32<0x01000000>;
      using CFGR_MCO1PRE_1          = UInt32<0x02000000>;
      using CFGR_MCO1PRE_2          = UInt32<0x04000000>;
      using CFGR_MCO2PRE            = UInt32<0x38000000>;
      using CFGR_MCO2PRE_0          = UInt32<0x08000000>;
      using CFGR_MCO2PRE_1          = UInt32<0x10000000>;
      using CFGR_MCO2PRE_2          = UInt32<0x20000000>;
      using CFGR_MCO2               = UInt32<0xC0000000>;
      using CFGR_MCO2_0             = UInt32<0x40000000>;
      using CFGR_MCO2_1             = UInt32<0x80000000>;
      using CIR_LSIRDYF             = UInt32<0x00000001>;
      using CIR_LSERDYF             = UInt32<0x00000002>;
      using CIR_HSIRDYF             = UInt32<0x00000004>;
      using CIR_HSERDYF             = UInt32<0x00000008>;
      using CIR_PLLRDYF             = UInt32<0x00000010>;
      using CIR_PLLI2SRDYF          = UInt32<0x00000020>;
      using CIR_CSSF                = UInt32<0x00000080>;
      using CIR_LSIRDYIE            = UInt32<0x00000100>;
      using CIR_LSERDYIE            = UInt32<0x00000200>;
      using CIR_HSIRDYIE            = UInt32<0x00000400>;
      using CIR_HSERDYIE            = UInt32<0x00000800>;
      using CIR_PLLRDYIE            = UInt32<0x00001000>;
      using CIR_PLLI2SRDYIE         = UInt32<0x00002000>;
      using CIR_LSIRDYC             = UInt32<0x00010000>;
      using CIR_LSERDYC             = UInt32<0x00020000>;
      using CIR_HSIRDYC             = UInt32<0x00040000>;
      using CIR_HSERDYC             = UInt32<0x00080000>;
      using CIR_PLLRDYC             = UInt32<0x00100000>;
      using CIR_PLLI2SRDYC          = UInt32<0x00200000>;
      using CIR_CSSC                = UInt32<0x00800000>;
      using AHB1RSTR_GPIOARST       = UInt32<0x00000001>;
      using AHB1RSTR_GPIOBRST       = UInt32<0x00000002>;
      using AHB1RSTR_GPIOCRST       = UInt32<0x00000004>;
      using AHB1RSTR_GPIODRST       = UInt32<0x00000008>;
      using AHB1RSTR_GPIOERST       = UInt32<0x00000010>;
      using AHB1RSTR_GPIOFRST       = UInt32<0x00000020>;
      using AHB1RSTR_GPIOGRST       = UInt32<0x00000040>;
      using AHB1RSTR_GPIOHRST       = UInt32<0x00000080>;
      using AHB1RSTR_GPIOIRST       = UInt32<0x00000100>;
      using AHB1RSTR_CRCRST         = UInt32<0x00001000>;
      using AHB1RSTR_DMA1RST        = UInt32<0x00200000>;
      using AHB1RSTR_DMA2RST        = UInt32<0x00400000>;
      using AHB1RSTR_ETHMACRST      = UInt32<0x02000000>;
      using AHB1RSTR_OTGHRST        = UInt32<0x10000000>;
      using AHB2RSTR_DCMIRST        = UInt32<0x00000001>;
      using AHB2RSTR_CRYPRST        = UInt32<0x00000010>;
      using AHB2RSTR_HASHRST        = UInt32<0x00000020>;
      using AHB2RSTR_RNGRST         = UInt32<0x00000040>;
      using AHB2RSTR_OTGFSRST       = UInt32<0x00000080>;
      using AHB3RSTR_FSMCRST        = UInt32<0x00000001>;
      using APB1RSTR_TIM2RST        = UInt32<0x00000001>;
      using APB1RSTR_TIM3RST        = UInt32<0x00000002>;
      using APB1RSTR_TIM4RST        = UInt32<0x00000004>;
      using APB1RSTR_TIM5RST        = UInt32<0x00000008>;
      using APB1RSTR_TIM6RST        = UInt32<0x00000010>;
      using APB1RSTR_TIM7RST        = UInt32<0x00000020>;
      using APB1RSTR_TIM12RST       = UInt32<0x00000040>;
      using APB1RSTR_TIM13RST       = UInt32<0x00000080>;
      using APB1RSTR_TIM14RST       = UInt32<0x00000100>;
      using APB1RSTR_WWDGEN         = UInt32<0x00000800>;
      using APB1RSTR_SPI2RST        = UInt32<0x00008000>;
      using APB1RSTR_SPI3RST        = UInt32<0x00010000>;
      using APB1RSTR_USART2RST      = UInt32<0x00020000>;
      using APB1RSTR_USART3RST      = UInt32<0x00040000>;
      using APB1RSTR_UART4RST       = UInt32<0x00080000>;
      using APB1RSTR_UART5RST       = UInt32<0x00100000>;
      using APB1RSTR_I2C1RST        = UInt32<0x00200000>;
      using APB1RSTR_I2C2RST        = UInt32<0x00400000>;
      using APB1RSTR_I2C3RST        = UInt32<0x00800000>;
      using APB1RSTR_CAN1RST        = UInt32<0x02000000>;
      using APB1RSTR_CAN2RST        = UInt32<0x04000000>;
      using APB1RSTR_PWRRST         = UInt32<0x10000000>;
      using APB1RSTR_DACRST         = UInt32<0x20000000>;
      using APB2RSTR_TIM1RST        = UInt32<0x00000001>;
      using APB2RSTR_TIM8RST        = UInt32<0x00000002>;
      using APB2RSTR_USART1RST      = UInt32<0x00000010>;
      using APB2RSTR_USART6RST      = UInt32<0x00000020>;
      using APB2RSTR_ADCRST         = UInt32<0x00000100>;
      using APB2RSTR_SDIORST        = UInt32<0x00000800>;
      using APB2RSTR_SPI1RST        = UInt32<0x00001000>;
      using APB2RSTR_SYSCFGRST      = UInt32<0x00004000>;
      using APB2RSTR_TIM9RST        = UInt32<0x00010000>;
      using APB2RSTR_TIM10RST       = UInt32<0x00020000>;
      using APB2RSTR_TIM11RST       = UInt32<0x00040000>;
      using AHB1ENR_GPIOAEN         = UInt32<0x00000001>;
      using AHB1ENR_GPIOBEN         = UInt32<0x00000002>;
      using AHB1ENR_GPIOCEN         = UInt32<0x00000004>;
      using AHB1ENR_GPIODEN         = UInt32<0x00000008>;
      using AHB1ENR_GPIOEEN         = UInt32<0x00000010>;
      using AHB1ENR_GPIOFEN         = UInt32<0x00000020>;
      using AHB1ENR_GPIOGEN         = UInt32<0x00000040>;
      using AHB1ENR_GPIOHEN         = UInt32<0x00000080>;
      using AHB1ENR_GPIOIEN         = UInt32<0x00000100>;
      using AHB1ENR_CRCEN           = UInt32<0x00001000>;
      using AHB1ENR_BKPSRAMEN       = UInt32<0x00040000>;
      using AHB1ENR_CCMDATARAMEN    = UInt32<0x00100000>;
      using AHB1ENR_DMA1EN          = UInt32<0x00200000>;
      using AHB1ENR_DMA2EN          = UInt32<0x00400000>;
      using AHB1ENR_ETHMACEN        = UInt32<0x02000000>;
      using AHB1ENR_ETHMACTXEN      = UInt32<0x04000000>;
      using AHB1ENR_ETHMACRXEN      = UInt32<0x08000000>;
      using AHB1ENR_ETHMACPTPEN     = UInt32<0x10000000>;
      using AHB1ENR_OTGHSEN         = UInt32<0x20000000>;
      using AHB1ENR_OTGHSULPIEN     = UInt32<0x40000000>;
      using AHB2ENR_DCMIEN          = UInt32<0x00000001>;
      using AHB2ENR_CRYPEN          = UInt32<0x00000010>;
      using AHB2ENR_HASHEN          = UInt32<0x00000020>;
      using AHB2ENR_RNGEN           = UInt32<0x00000040>;
      using AHB2ENR_OTGFSEN         = UInt32<0x00000080>;
      using AHB3ENR_FSMCEN          = UInt32<0x00000001>;
      using APB1ENR_TIM2EN          = UInt32<0x00000001>;
      using APB1ENR_TIM3EN          = UInt32<0x00000002>;
      using APB1ENR_TIM4EN          = UInt32<0x00000004>;
      using APB1ENR_TIM5EN          = UInt32<0x00000008>;
      using APB1ENR_TIM6EN          = UInt32<0x00000010>;
      using APB1ENR_TIM7EN          = UInt32<0x00000020>;
      using APB1ENR_TIM12EN         = UInt32<0x00000040>;
      using APB1ENR_TIM13EN         = UInt32<0x00000080>;
      using APB1ENR_TIM14EN         = UInt32<0x00000100>;
      using APB1ENR_WWDGEN          = UInt32<0x00000800>;
      using APB1ENR_SPI2EN          = UInt32<0x00004000>;
      using APB1ENR_SPI3EN          = UInt32<0x00008000>;
      using APB1ENR_USART2EN        = UInt32<0x00020000>;
      using APB1ENR_USART3EN        = UInt32<0x00040000>;
      using APB1ENR_UART4EN         = UInt32<0x00080000>;
      using APB1ENR_UART5EN         = UInt32<0x00100000>;
      using APB1ENR_I2C1EN          = UInt32<0x00200000>;
      using APB1ENR_I2C2EN          = UInt32<0x00400000>;
      using APB1ENR_I2C3EN          = UInt32<0x00800000>;
      using APB1ENR_CAN1EN          = UInt32<0x02000000>;
      using APB1ENR_CAN2EN          = UInt32<0x04000000>;
      using APB1ENR_PWREN           = UInt32<0x10000000>;
      using APB1ENR_DACEN           = UInt32<0x20000000>;
      using APB2ENR_TIM1EN          = UInt32<0x00000001>;
      using APB2ENR_TIM8EN          = UInt32<0x00000002>;
      using APB2ENR_USART1EN        = UInt32<0x00000010>;
      using APB2ENR_USART6EN        = UInt32<0x00000020>;
      using APB2ENR_ADC1EN          = UInt32<0x00000100>;
      using APB2ENR_ADC2EN          = UInt32<0x00000200>;
      using APB2ENR_ADC3EN          = UInt32<0x00000400>;
      using APB2ENR_SDIOEN          = UInt32<0x00000800>;
      using APB2ENR_SPI1EN          = UInt32<0x00001000>;
      using APB2ENR_SYSCFGEN        = UInt32<0x00004000>;
      using APB2ENR_TIM11EN         = UInt32<0x00040000>;
      using APB2ENR_TIM10EN         = UInt32<0x00020000>;
      using APB2ENR_TIM9EN          = UInt32<0x00010000>;
      using AHB1LPENR_GPIOALPEN     = UInt32<0x00000001>;
      using AHB1LPENR_GPIOBLPEN     = UInt32<0x00000002>;
      using AHB1LPENR_GPIOCLPEN     = UInt32<0x00000004>;
      using AHB1LPENR_GPIODLPEN     = UInt32<0x00000008>;
      using AHB1LPENR_GPIOELPEN     = UInt32<0x00000010>;
      using AHB1LPENR_GPIOFLPEN     = UInt32<0x00000020>;
      using AHB1LPENR_GPIOGLPEN     = UInt32<0x00000040>;
      using AHB1LPENR_GPIOHLPEN     = UInt32<0x00000080>;
      using AHB1LPENR_GPIOILPEN     = UInt32<0x00000100>;
      using AHB1LPENR_CRCLPEN       = UInt32<0x00001000>;
      using AHB1LPENR_FLITFLPEN     = UInt32<0x00008000>;
      using AHB1LPENR_SRAM1LPEN     = UInt32<0x00010000>;
      using AHB1LPENR_SRAM2LPEN     = UInt32<0x00020000>;
      using AHB1LPENR_BKPSRAMLPEN   = UInt32<0x00040000>;
      using AHB1LPENR_DMA1LPEN      = UInt32<0x00200000>;
      using AHB1LPENR_DMA2LPEN      = UInt32<0x00400000>;
      using AHB1LPENR_ETHMACLPEN    = UInt32<0x02000000>;
      using AHB1LPENR_ETHMACTXLPEN  = UInt32<0x04000000>;
      using AHB1LPENR_ETHMACRXLPEN  = UInt32<0x08000000>;
      using AHB1LPENR_ETHMACPTPLPEN = UInt32<0x10000000>;
      using AHB1LPENR_OTGHSLPEN     = UInt32<0x20000000>;
      using AHB1LPENR_OTGHSULPILPEN = UInt32<0x40000000>;
      using AHB2LPENR_DCMILPEN      = UInt32<0x00000001>;
      using AHB2LPENR_CRYPLPEN      = UInt32<0x00000010>;
      using AHB2LPENR_HASHLPEN      = UInt32<0x00000020>;
      using AHB2LPENR_RNGLPEN       = UInt32<0x00000040>;
      using AHB2LPENR_OTGFSLPEN     = UInt32<0x00000080>;
      using AHB3LPENR_FSMCLPEN      = UInt32<0x00000001>;
      using APB1LPENR_TIM2LPEN      = UInt32<0x00000001>;
      using APB1LPENR_TIM3LPEN      = UInt32<0x00000002>;
      using APB1LPENR_TIM4LPEN      = UInt32<0x00000004>;
      using APB1LPENR_TIM5LPEN      = UInt32<0x00000008>;
      using APB1LPENR_TIM6LPEN      = UInt32<0x00000010>;
      using APB1LPENR_TIM7LPEN      = UInt32<0x00000020>;
      using APB1LPENR_TIM12LPEN     = UInt32<0x00000040>;
      using APB1LPENR_TIM13LPEN     = UInt32<0x00000080>;
      using APB1LPENR_TIM14LPEN     = UInt32<0x00000100>;
      using APB1LPENR_WWDGLPEN      = UInt32<0x00000800>;
      using APB1LPENR_SPI2LPEN      = UInt32<0x00004000>;
      using APB1LPENR_SPI3LPEN      = UInt32<0x00008000>;
      using APB1LPENR_USART2LPEN    = UInt32<0x00020000>;
      using APB1LPENR_USART3LPEN    = UInt32<0x00040000>;
      using APB1LPENR_UART4LPEN     = UInt32<0x00080000>;
      using APB1LPENR_UART5LPEN     = UInt32<0x00100000>;
      using APB1LPENR_I2C1LPEN      = UInt32<0x00200000>;
      using APB1LPENR_I2C2LPEN      = UInt32<0x00400000>;
      using APB1LPENR_I2C3LPEN      = UInt32<0x00800000>;
      using APB1LPENR_CAN1LPEN      = UInt32<0x02000000>;
      using APB1LPENR_CAN2LPEN      = UInt32<0x04000000>;
      using APB1LPENR_PWRLPEN       = UInt32<0x10000000>;
      using APB1LPENR_DACLPEN       = UInt32<0x20000000>;
      using APB2LPENR_TIM1LPEN      = UInt32<0x00000001>;
      using APB2LPENR_TIM8LPEN      = UInt32<0x00000002>;
      using APB2LPENR_USART1LPEN    = UInt32<0x00000010>;
      using APB2LPENR_USART6LPEN    = UInt32<0x00000020>;
      using APB2LPENR_ADC1LPEN      = UInt32<0x00000100>;
      using APB2LPENR_ADC2PEN       = UInt32<0x00000200>;
      using APB2LPENR_ADC3LPEN      = UInt32<0x00000400>;
      using APB2LPENR_SDIOLPEN      = UInt32<0x00000800>;
      using APB2LPENR_SPI1LPEN      = UInt32<0x00001000>;
      using APB2LPENR_SYSCFGLPEN    = UInt32<0x00004000>;
      using APB2LPENR_TIM9LPEN      = UInt32<0x00010000>;
      using APB2LPENR_TIM10LPEN     = UInt32<0x00020000>;
      using APB2LPENR_TIM11LPEN     = UInt32<0x00040000>;
      using BDCR_LSEON              = UInt32<0x00000001>;
      using BDCR_LSERDY             = UInt32<0x00000002>;
      using BDCR_LSEBYP             = UInt32<0x00000004>;
      using BDCR_RTCSEL             = UInt32<0x00000300>;
      using BDCR_RTCSEL_0           = UInt32<0x00000100>;
      using BDCR_RTCSEL_1           = UInt32<0x00000200>;
      using BDCR_RTCEN              = UInt32<0x00008000>;
      using BDCR_BDRST              = UInt32<0x00010000>;
      using CSR_LSION               = UInt32<0x00000001>;
      using CSR_LSIRDY              = UInt32<0x00000002>;
      using CSR_RMVF                = UInt32<0x01000000>;
      using CSR_BORRSTF             = UInt32<0x02000000>;
      using CSR_PADRSTF             = UInt32<0x04000000>;
      using CSR_PORRSTF             = UInt32<0x08000000>;
      using CSR_SFTRSTF             = UInt32<0x10000000>;
      using CSR_WDGRSTF             = UInt32<0x20000000>;
      using CSR_WWDGRSTF            = UInt32<0x40000000>;
      using CSR_LPWRRSTF            = UInt32<0x80000000>;
      using SSCGR_MODPER            = UInt32<0x00001FFF>;
      using SSCGR_INCSTEP           = UInt32<0x0FFFE000>;
      using SSCGR_SPREADSEL         = UInt32<0x40000000>;
      using SSCGR_SSCGEN            = UInt32<0x80000000>;
      using PLLI2SCFGR_PLLI2SN      = UInt32<0x00007FC0>;
      using PLLI2SCFGR_PLLI2SR      = UInt32<0x70000000>;
      
      template <typename Kernel, typename Interrupt> struct Handle;
      template <typename Kernel>
      struct Handle<Kernel, Irqs::RCC> {
        // TODO
      };
      
      template <typename Kernel>
      struct Initialise {
        void operator()() {
          // TODO
        }
      };
    };
  }
}

#endif
