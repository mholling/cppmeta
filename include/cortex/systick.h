#ifndef CPPMETA_CORTEX_SYSTICK_H
#define CPPMETA_CORTEX_SYSTICK_H

#include <stdint.h>
#include "meta/value.h"
#include "meta/meta.h"
#include "meta/list.h"
#include "meta/many.h"
#include "cortex/registers.h"
#include "cortex/nvic.h"
#include "cortex/addresses.h"
#include "cortex/irqs.h"
#include "cortex/critical_section.h"

namespace CppMeta {
  namespace Cortex {
    struct SysTick_Type {
      volatile       uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
      volatile       uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
      volatile       uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
      volatile const uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
    };
    
    template <typename Address>
    struct SysTick : Registers<SysTick_Type, Address> {
      using CTRL_COUNTFLAG = UInt32<1UL<<16>;
      using CTRL_CLKSOURCE = UInt32<1UL<<2>;
      using CTRL_TICKINT   = UInt32<1UL<<1>;
      using CTRL_ENABLE    = UInt32<1UL<<0>;
      using LOAD_RELOAD    = UInt32<0xFFFFFFUL<<0>;
      using VAL_CURRENT    = UInt32<0xFFFFFFUL<<0>;
      using CALIB_NOREF    = UInt32<1UL<<31>;
      using CALIB_SKEW     = UInt32<1UL<<30>;
      using CALIB_TENMS    = UInt32<0xFFFFFFUL<<0>;
      
      using Dependencies = List<Nvic1>;
      
      struct DefaultConfiguration {
        using Periods = List<>;
        using ResolutionInMicroseconds = Int<1000>;
        using FrequencyInHz = Int<72000000>;
      };
      
      template <typename Kernel>
      struct Initialise {
        using Config = typename Kernel::template Configuration<SysTick>;
        using Periods = typename Config::Periods;
        
        using ResolutionInMicroseconds = UInt64<Config::ResolutionInMicroseconds::value>;
        using FrequencyInHz = UInt64<Config::FrequencyInHz::value>; // TODO: how to configure this from a dependency? get it from Kernel?
        
        using LOAD = Divide<Multiply<FrequencyInHz, ResolutionInMicroseconds>, UInt64<1000000>>;
        static_assert(Not<GreaterThan<LOAD, UInt64<0xFFFFFF>>>::value, "SysTick resolution in microseconds needs to be smaller");
        
        using CTRL = If<NotEmpty<Periods>, BitwiseOrMany<CTRL_ENABLE, CTRL_TICKINT, CTRL_CLKSOURCE>, UInt32<0>>;
        
        void operator()() {
          SysTick::registers.CTRL = CTRL::value;
          SysTick::registers.LOAD = LOAD::value;
        }
      };
      
      template <typename> struct Event;
      
      static volatile uint64_t systicks;
      
      template <typename Kernel, typename Interrupt> struct Handle;
      template <typename Kernel>
      struct Handle<Kernel, Irqs::SysTick> {
        using Config = typename Kernel::template Configuration<SysTick>;
        using Periods = typename Config::Periods;
        using ResolutionInMicroseconds = typename Config::ResolutionInMicroseconds;
        
        template <typename Period>
        struct FireIfDue {
          static_assert(IsZero<Modulus<Period, ResolutionInMicroseconds>>::value, "Period must be a multiple of the SysTick resolution");
          using Count = Divide<Period, ResolutionInMicroseconds>;
          
          void operator()() {
            static uint32_t ticks = 0;
            ++ticks %= Count::value;
            if (ticks == 0) Kernel::template post<Event<Period>>();
          }
        };
        
        void operator()() {
          critical_section([]() { // TODO: needed?
            ++systicks;
            Each<Periods, FireIfDue>()();
          });
        }
      };
    };
    
    template <typename Address>
    volatile uint64_t SysTick<Address>::systicks = 0;
    
    using SysTick1 = SysTick<Addresses::SysTick>;
  }
}

#endif
