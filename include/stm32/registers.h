#ifndef CPPMETA_STM32_REGISTERS_H
#define CPPMETA_STM32_REGISTERS_H

#include <stdint.h>
#include "meta/value.h"
#include "cortex/registers.h"
#include "stm32/addresses.h"

namespace CppMeta {
  namespace Stm32 {
    template <typename Type, typename Address>
    struct Registers : Cortex::Registers<Type, Address> {
      template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
      struct Bit {
        using RegisterAddress = UInt32<reinterpret_cast<uint32_t>(&(Registers::registers.*rg))>;
        using BitOffset = LeftShift<UInt32<LowBitPosition<Mask>::value>, Int<2>>;
        using WordOffset = LeftShift<Minus<RegisterAddress, Addresses::PERIPH>, Int<5>>;
        using Offset = Plus<BitOffset, WordOffset>;
        using BitBandAddress = Plus<Addresses::PERIPH_BB, Offset>;
        using Qualified = If<IsConst<RegisterType>, const uint32_t, uint32_t>;
        
        static constexpr volatile Qualified& bit = *reinterpret_cast<volatile Qualified *>(BitBandAddress::value);
      };
      
      template <volatile uint32_t Type::*rg, typename Mask>
      static constexpr volatile typename Bit<uint32_t, rg, Mask>::Qualified& bit() { return Bit<uint32_t, rg, Mask>::bit; }
      
      template <volatile uint16_t Type::*rg, typename Mask>
      static constexpr volatile typename Bit<uint16_t, rg, Mask>::Qualified& bit() { return Bit<uint16_t, rg, Mask>::bit; }
      
      template <volatile uint8_t Type::*rg, typename Mask>
      static constexpr volatile typename Bit<uint8_t, rg, Mask>::Qualified& bit() { return Bit<uint8_t, rg, Mask>::bit; }
    };
  }
}

#endif
