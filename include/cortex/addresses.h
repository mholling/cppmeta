#ifndef CPPMETA_CORTEX_ADDRESSES_H
#define CPPMETA_CORTEX_ADDRESSES_H

#include "meta/value.h"

namespace CppMeta {
  namespace Cortex {
    namespace Addresses {
      using SCS =       UInt32<0xE000E000UL>;                           /*!< System Control Space Base Address  */
      using ITM =       UInt32<0xE0000000UL>;                           /*!< ITM Base Address                   */
      using CoreDebug = UInt32<0xE000EDF0UL>;                           /*!< Core Debug Base Address            */
      using SysTick =   Plus<SCS, UInt32<0x0010UL>>;                    /*!< SysTick Base Address               */
      using NVIC =      Plus<SCS, UInt32<0x0100UL>>;                    /*!< NVIC Base Address                  */
      using SCB =       Plus<SCS, UInt32<0x0D00UL>>;                    /*!< System Control Block Base Address  */
      using MPU =       Plus<SCS, UInt32<0x0D90UL>>;                    /*!< Memory Protection Unit             */
      using FPU =       Plus<SCS, UInt32<0x0F30UL>>;                    /*!< Floating Point Unit                */
    }
  }
}

#endif
