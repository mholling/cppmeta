#ifndef CPPMETA_CORTEX_REGISTERS_H
#define CPPMETA_CORTEX_REGISTERS_H

#include <stdint.h>
#include "meta/value.h"
#include "meta/meta.h"

namespace CppMeta {
  namespace Cortex {
    template <typename Type, typename Address>
    struct Registers {
      static constexpr Type& registers = *reinterpret_cast<Type *>(Address::value);
      
      template <volatile uint32_t Type::*rg>
      static constexpr volatile uint32_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint16_t Type::*rg>
      static constexpr volatile uint16_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint8_t Type::*rg>
      static constexpr volatile uint8_t& reg() { return Registers::registers.*rg; }
      
      template <volatile const uint32_t Type::*rg>
      static constexpr volatile const uint32_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint16_t Type::*rg>
      static constexpr volatile const uint16_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint8_t Type::*rg>
      static constexpr volatile const uint8_t& reg() { return Registers::registers.*rg; }
      
      template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
      class BitField {
        unsigned int      : LowBitPosition<Mask>::value;
        unsigned int bits : BitCount<Mask>::value;
        unsigned int      : 8 * sizeof(RegisterType) - LowBitPosition<Mask>::value - BitCount<Mask>::value;
        
        BitField(); ~BitField();
      public:
        using Qualified = If<IsConst<RegisterType>, const BitField, BitField>;
        static constexpr volatile Qualified& bitfield = *reinterpret_cast<volatile Qualified *>(&(registers.*rg));
        
        template <typename IntType>
        void operator=(const IntType& i) volatile { bits = i; }
        
        template <typename IntType>
        operator IntType() const volatile { return bits; }
      };
      
      template <volatile uint32_t Type::*rg, typename Mask>
      static constexpr volatile typename BitField<uint32_t, rg, Mask>::Qualified& bitfield() { return BitField<uint32_t, rg, Mask>::bitfield; }
      
      template <volatile uint16_t Type::*rg, typename Mask>
      static constexpr volatile typename BitField<uint16_t, rg, Mask>::Qualified& bitfield() { return BitField<uint16_t, rg, Mask>::bitfield; }
      
      template <volatile uint8_t Type::*rg, typename Mask>
      static constexpr volatile typename BitField<uint8_t, rg, Mask>::Qualified& bitfield() { return BitField<uint8_t, rg, Mask>::bitfield; }
    };
  }
}

#endif
