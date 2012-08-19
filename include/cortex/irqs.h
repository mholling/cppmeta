#ifndef CPPMETA_CORTEX_IRQS_H
#define CPPMETA_CORTEX_IRQS_H

#include "meta/value.h"

namespace CppMeta {
  namespace Cortex {
    namespace Irqs {
      using Reset            = Int<-15>;
      using NonMaskableInt   = Int<-14>;    /*!< 2 Non Maskable Interrupt                                          */
      using HardFault        = Int<-13>;
      using MemoryManagement = Int<-12>;    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
      using BusFault         = Int<-11>;    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
      using UsageFault       = Int<-10>;    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
      using SVCall           = Int<-5>;     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
      using DebugMonitor     = Int<-4>;     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
      using PendSV           = Int<-2>;     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
      using SysTick          = Int<-1>;     /*!< 15 Cortex-M4 System Tick Interrupt                                */
    }
  }
}

#endif
