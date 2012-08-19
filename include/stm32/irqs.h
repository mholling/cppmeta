#ifndef CPPMETA_STM32_IRQS_H
#define CPPMETA_STM32_IRQS_H

#include "meta/list.h"
#include "cortex/irqs.h"

namespace CppMeta {
  namespace Stm32 {
    namespace Irqs {
      using WWDG           = Int<0>;      /*!< Window WatchDog Interrupt                                         */  
      using PVD            = Int<1>;      /*!< PVD through EXTI Line detection Interrupt                         */  
      using TAMP_STAMP     = Int<2>;      /*!< Tamper and TimeStamp interrupts through the EXTI line             */  
      using RTC_WKUP       = Int<3>;      /*!< RTC Wakeup interrupt through the EXTI line                        */  
      using FLASH          = Int<4>;      /*!< FLASH Interrupt                                                   */  
      using RCC            = Int<5>;      /*!< RCC Interrupt                                                     */  
      using EXTI0          = Int<6>;      /*!< EXTI Line0 Interrupt                                              */  
      using EXTI1          = Int<7>;      /*!< EXTI Line1 Interrupt                                              */  
      using EXTI2          = Int<8>;      /*!< EXTI Line2 Interrupt                                              */  
      using EXTI3          = Int<9>;      /*!< EXTI Line3 Interrupt                                              */  
      using EXTI4          = Int<10>;     /*!< EXTI Line4 Interrupt                                              */  
      using DMA1_Stream0   = Int<11>;     /*!< DMA1 Stream 0 Interrupt                                           */  
      using DMA1_Stream1   = Int<12>;     /*!< DMA1 Stream 1 Interrupt                                           */  
      using DMA1_Stream2   = Int<13>;     /*!< DMA1 Stream 2 Interrupt                                           */  
      using DMA1_Stream3   = Int<14>;     /*!< DMA1 Stream 3 Interrupt                                           */  
      using DMA1_Stream4   = Int<15>;     /*!< DMA1 Stream 4 Interrupt                                           */  
      using DMA1_Stream5   = Int<16>;     /*!< DMA1 Stream 5 Interrupt                                           */  
      using DMA1_Stream6   = Int<17>;     /*!< DMA1 Stream 6 Interrupt                                           */  
      using ADC1_2         = Int<18>;     /*!< ADC1 and ADC2 Interrupt                              */  
      using ADC            = Int<18>;     /*!< ADC1, ADC2 and ADC3 Interrupt       s                             */  
      using CAN1_TX        = Int<19>;     /*!< CAN1 TX Interrupt                                                 */  
      using USB_HP         = Int<19>;     /*!< USB Device High Priority Interrupts                  */  
      using CAN1_RX0       = Int<20>;     /*!< CAN1 RX0 Interrupt                                                */  
      using USB_LP         = Int<20>;     /*!< USB Device Low Priority             Interrupts       */  
      using CAN1_RX1       = Int<21>;     /*!< CAN1 RX1 Interrupt                                                */  
      using CAN1_SCE       = Int<22>;     /*!< CAN1 SCE Interrupt                                                */  
      using EXTI9_5        = Int<23>;     /*!< External Line[9:5] Interrupts                                     */  
      using TIM1_BRK       = Int<24>;     /*!< TIM1 Break Interrupt                                 */  
      using TIM15          = Int<24>;     /*!< IM15 Interrupts                                      */  
      using TIM9           = Int<24>;     /*!< TIM9 interrupt                                       */  
      using TIM1_UP        = Int<25>;     /*!< TIM1 Update Interrupt                                */  
      using TIM10          = Int<25>;     /*!< TIM10 interrupt                                      */  
      using TIM16          = Int<25>;     /*!< TIM16 Interrupts                                     */  
      using TIM1_TRG_COM   = Int<26>;     /*!< TIM1 Trigger and Commutation Interrupt               */  
      using TIM11          = Int<26>;     /*!< TIM11 interrupt                                      */  
      using TIM17          = Int<26>;     /*!< TIM17 Interrupt                                      */  
      using TIM1_CC        = Int<27>;     /*!< TIM1 Capture Compare Interrupt                                    */  
      using TIM2           = Int<28>;     /*!< TIM2 Interrupt                                                    */  
      using TIM3           = Int<29>;     /*!< TIM3 Interrupt                                                    */  
      using TIM4           = Int<30>;     /*!< TIM4 Interrupt                                                    */  
      using I2C1_EV        = Int<31>;     /*!< I2C1 Event Interrupt                                              */  
      using I2C1_ER        = Int<32>;     /*!< I2C1 Error Interrupt                                              */  
      using I2C2_EV        = Int<33>;     /*!< I2C2 Event Interrupt                                              */  
      using I2C2_ER        = Int<34>;     /*!< I2C2 Error Interrupt                                              */    
      using SPI1           = Int<35>;     /*!< SPI1 Interrupt                                                    */  
      using SPI2           = Int<36>;     /*!< SPI2 Interrupt                                                    */  
      using USART1         = Int<37>;     /*!< USART1 Interrupt                                                  */  
      using USART2         = Int<38>;     /*!< USART2 Interrupt                                                  */  
      using USART3         = Int<39>;     /*!< USART3 Interrupt                                                  */  
      using EXTI15_10      = Int<40>;     /*!< External Line[15:10] Interrupts                                   */  
      using RTC_Alarm      = Int<41>;     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */  
      using CEC            = Int<42>;     /*!< HDMI-CEC Interrupt                                   */  
      using OTG_FS_WKUP    = Int<42>;     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */      
      using TIM12          = Int<43>;     /*!< TIM12 Interrupt                                      */  
      using TIM8_BRK       = Int<43>;     /*!< TIM8 Break Interrupt                                 */  
      using TIM13          = Int<44>;     /*!< TIM13 Interrupt                                      */  
      using TIM8_UP        = Int<44>;     /*!< TIM8 Update Interrupt                                */  
      using TIM14          = Int<45>;     /*!< TIM14 Interrupt                                      */  
      using TIM8_TRG_COM   = Int<45>;     /*!< TIM8 Trigger and Commutation Interrupt               */  
      using TIM8_CC        = Int<46>;     /*!< TIM8 Capture Compare Interrupt                                    */  
      using ADC3           = Int<47>;     /*!< ADC3 Interrupt                                       */  
      using DMA1_Stream7   = Int<47>;     /*!< DMA1 Stream7 Interrupt                                            */  
      using FSMC           = Int<48>;     /*!< FSMC Interrupt                                                    */  
      using SDIO           = Int<49>;     /*!< SDIO Interrupt                                                    */  
      using TIM5           = Int<50>;     /*!< TIM5 Interrupt                                                    */  
      using SPI3           = Int<51>;     /*!< SPI3 Interrupt                                                    */  
      using UART4          = Int<52>;     /*!< UART4 Interrupt                                                   */  
      using UART5          = Int<53>;     /*!< UART5 Interrupt                                                   */  
      using TIM6           = Int<54>;     /*!< TIM6 Interrupt                                       */  
      using DAC            = Int<54>;     /*!< DAC1&2 underrun error  interrupts                    */  
      using TIM7           = Int<55>;     /*!< TIM7 Interrupt                                                    */  
      using DMA2_Stream0   = Int<56>;     /*!< DMA2 Stream 0 Interrupt                                           */  
      using DMA2_Stream1   = Int<57>;     /*!< DMA2 Stream 1 Interrupt                                           */  
      using DMA2_Stream2   = Int<58>;     /*!< DMA2 Stream 2 Interrupt                                           */  
      using DMA2_Stream3_4 = Int<59>;     /*!< DMA2 Channel 4 and Channel 5 Interrupt               */
      using DMA2_Stream3   = Int<59>;     /*!< DMA2 Stream 3 Interrupt                                           */  
      using DMA2_Stream4   = Int<60>;     /*!< DMA2 Stream 4 Interrupt                                           */  
      using ETH            = Int<61>;     /*!< Ethernet Interrupt                                                */  
      using ETH_WKUP       = Int<62>;     /*!< Ethernet Wakeup through EXTI line Interrupt                       */  
      using CAN2_TX        = Int<63>;     /*!< CAN2 TX Interrupt                                                 */  
      using CAN2_RX0       = Int<64>;     /*!< CAN2 RX0 Interrupt                                                */  
      using CAN2_RX1       = Int<65>;     /*!< CAN2 RX1 Interrupt                                                */  
      using CAN2_SCE       = Int<66>;     /*!< CAN2 SCE Interrupt                                                */  
      using OTG_FS         = Int<67>;     /*!< USB OTG FS Interrupt                                              */  
      using DMA2_Stream5   = Int<68>;     /*!< DMA2 Stream 5 Interrupt                                           */  
      using DMA2_Stream6   = Int<69>;     /*!< DMA2 Stream 6 Interrupt                                           */  
      using DMA2_Stream7   = Int<70>;     /*!< DMA2 Stream 7 Interrupt                                           */  
      using USART6         = Int<71>;     /*!< USART6 Interrupt                                                  */   
      using I2C3_EV        = Int<72>;     /*!< I2C3 event interrupt                                              */  
      using I2C3_ER        = Int<73>;     /*!< I2C3 error interrupt                                              */  
      using OTG_HS_EP1_OUT = Int<74>;     /*!< USB OTG HS End Point 1 Out Interrupt                              */  
      using OTG_HS_EP1_IN  = Int<75>;     /*!< USB OTG HS End Point 1 In Interrupt                               */  
      using OTG_HS_WKUP    = Int<76>;     /*!< USB OTG HS Wakeup through EXTI interrupt                          */  
      using OTG_HS         = Int<77>;     /*!< USB OTG HS Interrupt                                              */  
      using DCMI           = Int<78>;     /*!< DCMI Interrupt                                                    */  
      using CRYP           = Int<79>;     /*!< CRYP crypto Interrupt                                             */  
      using HASH_RNG       = Int<80>;     /*!< Hash and Rng Interrupt                                            */
      using FPU            = Int<81>;     /*!< FPU Interrupt                                                     */
    }
  }
}

#endif
