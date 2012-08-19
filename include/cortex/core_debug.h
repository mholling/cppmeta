#ifndef CPPMETA_CORTEX_CORE_DEBUG_H
#define CPPMETA_CORTEX_CORE_DEBUG_H

#include <stdint.h>
#include "meta/value.h"
#include "meta/meta.h"
#include "meta/list.h"
#include "cortex/registers.h"
#include "cortex/addresses.h"

namespace CppMeta {
  namespace Cortex {
    struct CoreDebug_Type {
      volatile       uint32_t DHCSR;       /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register    */
      volatile const uint32_t DCRSR;       /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register        */
      volatile       uint32_t DCRDR;       /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register            */
      volatile       uint32_t DEMCR;       /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
    };
    
    struct CoreDebug : Registers<CoreDebug_Type, Addresses::CoreDebug> {
      using DHCSR_DBGKEY       = UInt32<0xFFFFUL<<16>;
      using DHCSR_S_RESET_ST   = UInt32<1UL<<25>;
      using DHCSR_S_RETIRE_ST  = UInt32<1UL<<24>;
      using DHCSR_S_LOCKUP     = UInt32<1UL<<19>;
      using DHCSR_S_SLEEP      = UInt32<1UL<<18>;
      using DHCSR_S_HALT       = UInt32<1UL<<17>;
      using DHCSR_S_REGRDY     = UInt32<1UL<<16>;
      using DHCSR_C_SNAPSTALL  = UInt32<1UL<<5>;
      using DHCSR_C_MASKINTS   = UInt32<1UL<<3>;
      using DHCSR_C_STEP       = UInt32<1UL<<2>;
      using DHCSR_C_HALT       = UInt32<1UL<<1>;
      using DHCSR_C_DEBUGEN    = UInt32<1UL<<0>;
      using DCRSR_REGWnR       = UInt32<1UL<<16>;
      using DCRSR_REGSEL       = UInt32<0x1FUL<<0>;
      using DEMCR_TRCENA       = UInt32<1UL<<24>;
      using DEMCR_MON_REQ      = UInt32<1UL<<19>;
      using DEMCR_MON_STEP     = UInt32<1UL<<18>;
      using DEMCR_MON_PEND     = UInt32<1UL<<17>;
      using DEMCR_MON_EN       = UInt32<1UL<<16>;
      using DEMCR_VC_HARDERR   = UInt32<1UL<<10>;
      using DEMCR_VC_INTERR    = UInt32<1UL<<9>;
      using DEMCR_VC_BUSERR    = UInt32<1UL<<8>;
      using DEMCR_VC_STATERR   = UInt32<1UL<<7>;
      using DEMCR_VC_CHKERR    = UInt32<1UL<<6>;
      using DEMCR_VC_NOCPERR   = UInt32<1UL<<5>;
      using DEMCR_VC_MMERR     = UInt32<1UL<<4>;
      using DEMCR_VC_CORERESET = UInt32<1UL<<0>;
      
      using SilentLevel =  Int<0>;
      using ErrorLevel =   Int<1>;
      using WarningLevel = Int<2>;
      using InfoLevel =    Int<3>;
      using DebugLevel =   Int<4>;
      
      struct DefaultConfiguration {
        using level = ErrorLevel;
      };
      
      template <typename Kernel> using Config = typename Kernel::template Configuration<CoreDebug>;
      
      struct DCCLog {
        static constexpr uint32_t busy = 0x01;
        static constexpr uint32_t sending_tracepoint = 0x00;
        static constexpr uint32_t sending_hex = 0x01;
        static constexpr uint32_t sending_ascii = 0x01;
        static constexpr uint32_t sending_char = 0x02;
        
        static void write(unsigned int data) {
          for (int n = 0; n < 4; ++n, data >>= 8) {
            while (CoreDebug::registers.DCRDR & busy) ;
            CoreDebug::registers.DCRDR = (data & 0xff) << 8 | busy;
          }
        }
        
        template <typename T>
        void write_body(const volatile T* t, unsigned int length) {
          constexpr unsigned int multiple = sizeof(uint32_t)/sizeof(T);
          for (; length >= multiple; length -= multiple, t += multiple)
            write(*reinterpret_cast<const volatile uint32_t *>(t));
          if (length > 0) {
            unsigned int last = 0;
            for (unsigned int n = 0; n < length; ++n, ++t)
              last |= static_cast<unsigned int>(*t) << 8 * sizeof(T) * n;
            write(last);
          }
        }
        
        template <typename T>
        void operator ()(const volatile T *t, unsigned int length) {
          static_assert(Contains<List<unsigned int, unsigned short, unsigned char>, T>::value, "must be unsigned int, short or char");
          write(sending_hex | sizeof(T) << 8 | length << 16);
          write_body(t, length);
        }
        
        void operator ()(const volatile char s[]) {
          unsigned int length = 0;
          for (; s[length]; ++length) ;
          write(sending_ascii | length << 16);
          write_body(s, length);
        }
        
        void operator ()(char c) {
          write(sending_char | static_cast<unsigned int>(c) << 16);
        }
        
        void operator ()(unsigned int tracepoint) {
          write(sending_tracepoint | tracepoint << 8);
        }
      };
      
      struct NullLog { template <typename... Args> void operator()(Args... args) { } };
      
      template <typename Kernel, typename Level> using Log = If<GreaterThan<Level, typename Kernel::template Configuration<CoreDebug>::Level>, NullLog, DCCLog>;
      template <typename Kernel> using Error   = Log<Kernel, Int<1>>;
      template <typename Kernel> using Warning = Log<Kernel, Int<2>>;
      template <typename Kernel> using Info    = Log<Kernel, Int<3>>;
      template <typename Kernel> using Debug   = Log<Kernel, Int<4>>;
    };
  }
}

#endif
