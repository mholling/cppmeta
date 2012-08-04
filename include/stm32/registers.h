#ifndef CPPMETA_STM32_REGISTERS_H
#define CPPMETA_STM32_REGISTERS_H

#include <stdint.h>
#include "meta/meta.h"
#include "cortex/registers.h"

namespace CppMeta {
  namespace Stm32 {
    template <typename Type, uint32_t address>
    struct Registers : Cortex::Registers<Type, address> {
      template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
      struct Bit {
        static volatile uint32_t& bit;
      };
      
      template <volatile uint32_t Type::*rg, typename Mask>
      static volatile uint32_t& bit() { return Bit<uint32_t, rg, Mask>::bit; }
      
      template <volatile uint16_t Type::*rg, typename Mask>
      static volatile uint32_t& bit() { return Bit<uint16_t, rg, Mask>::bit; }
      
      template <volatile uint8_t Type::*rg, typename Mask>
      static volatile uint32_t& bit() { return Bit<uint8_t, rg, Mask>::bit; }
    };
    
    template <typename Type, uint32_t address>
    template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
    volatile uint32_t& Registers<Type, address>::Bit<RegisterType, rg, Mask>::bit = *reinterpret_cast<volatile uint32_t *>(0x42000000 | ((reinterpret_cast<unsigned long>(&(Registers::registers.*rg)) - 0x40000000) << 5) | (BitPosition<Mask>::Result::value << 2));
  }
}

#endif
