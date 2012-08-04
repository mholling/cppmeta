#ifndef CPPMETA_CORTEX_REGISTERS_H
#define CPPMETA_CORTEX_REGISTERS_H

#include <stdint.h>
#include "meta/meta.h"

// namespace CppMeta {
//   namespace Cortex {
//     template <typename Type, uint32_t address>
//     struct Registers {
//       static Type& registers;
//       
//       template <volatile uint32_t Type::*rg>
//       static inline volatile uint32_t& reg() { return Registers::registers.*rg; }
//       
//       template <volatile uint16_t Type::*rg>
//       static inline volatile uint16_t& reg() { return Registers::registers.*rg; }
//       
//       template <volatile uint8_t Type::*rg>
//       static inline volatile uint8_t& reg() { return Registers::registers.*rg; }
//       
//       template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
//       class BitField {
//         using Position = typename BitPosition<Mask>::Result;
//         using Width = typename BitCount<Mask>::Result;
//         
//         volatile unsigned int      : Position::value;
//         volatile unsigned int bits : Width::value;
//         volatile unsigned int      : 8 * sizeof(RegisterType) - Position::value - Width::value;
//         
//         BitField();
//         ~BitField();
//         
//       public:
//         static BitField& bitfield;
//         
//         template <typename IntType>
//         BitField &operator=(const IntType& i) {
//           bits = i;
//           return *this; // TODO: causes warning... maybe make bitfields volatile instead???
//         }
//         
//         template <typename IntType>
//         operator IntType() const { return bits; }
//       };
//       
//       template <volatile uint32_t Type::*rg, typename Mask>
//       static BitField<uint32_t, rg, Mask>& bitfield() { return BitField<uint32_t, rg, Mask>::bitfield; }
//       
//       template <volatile uint16_t Type::*rg, typename Mask>
//       static BitField<uint16_t, rg, Mask>& bitfield() { return BitField<uint16_t, rg, Mask>::bitfield; }
//       
//       template <volatile uint8_t Type::*rg, typename Mask>
//       static BitField<uint8_t, rg, Mask>& bitfield() { return BitField<uint8_t, rg, Mask>::bitfield; }
//     };
//     
//     template <typename Type, uint32_t address>
//     Type& Registers<Type, address>::registers = *reinterpret_cast<Type *>(address);
//     
//     template <typename Type, uint32_t address>
//     template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
//     Registers<Type, address>::BitField<RegisterType, rg, Mask>& Registers<Type, address>::BitField<RegisterType, rg, Mask>::bitfield = *const_cast<Registers<Type, address>::BitField<RegisterType, rg, Mask> *>(reinterpret_cast<volatile Registers<Type, address>::BitField<RegisterType, rg, Mask> *>(&(Registers::registers.*rg)));
//   }
// }

namespace CppMeta {
  namespace Cortex {
    template <typename Type, uint32_t address>
    struct Registers {
      static Type& registers;
      
      template <volatile uint32_t Type::*rg>
      static inline volatile uint32_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint16_t Type::*rg>
      static inline volatile uint16_t& reg() { return Registers::registers.*rg; }
      
      template <volatile uint8_t Type::*rg>
      static inline volatile uint8_t& reg() { return Registers::registers.*rg; }
      
      template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
      class BitField {
        using Position = typename BitPosition<Mask>::Result;
        using Width = typename BitCount<Mask>::Result;
        
        unsigned int      : Position::value;
        unsigned int bits : Width::value;
        unsigned int      : 8 * sizeof(RegisterType) - Position::value - Width::value;
        
        BitField();
        ~BitField();
      public:
        static volatile BitField& bitfield;
        
        template <typename IntType>
        void operator=(const IntType& i) volatile { bits = i; }
        
        template <typename IntType>
        operator IntType() const volatile { return bits; }
      };
      
      template <volatile uint32_t Type::*rg, typename Mask>
      static volatile BitField<uint32_t, rg, Mask>& bitfield() { return BitField<uint32_t, rg, Mask>::bitfield; }
      
      template <volatile uint16_t Type::*rg, typename Mask>
      static volatile BitField<uint16_t, rg, Mask>& bitfield() { return BitField<uint16_t, rg, Mask>::bitfield; }
      
      template <volatile uint8_t Type::*rg, typename Mask>
      static volatile BitField<uint8_t, rg, Mask>& bitfield() { return BitField<uint8_t, rg, Mask>::bitfield; }
    };
    
    template <typename Type, uint32_t address>
    Type& Registers<Type, address>::registers = *reinterpret_cast<Type *>(address);
    
    template <typename Type, uint32_t address>
    template <typename RegisterType, volatile RegisterType Type::*rg, typename Mask>
    volatile Registers<Type, address>::BitField<RegisterType, rg, Mask>& Registers<Type, address>::BitField<RegisterType, rg, Mask>::bitfield = *reinterpret_cast<volatile Registers<Type, address>::BitField<RegisterType, rg, Mask> *>(&(Registers::registers.*rg));
  }
}

#endif
