#ifndef CPPMETA_TEST_VALUE_H
#define CPPMETA_TEST_VALUE_H

#include "meta/value.h"
#include "meta/meta.h"

namespace CppMeta {
  namespace Test {
    namespace TestValue {
      static_assert(Same<Bit<0>, Int<(1<<0)>>::value, "failed");
      static_assert(Same<Bit<1>, Int<(1<<1)>>::value, "failed");
      static_assert(Same<Bit<2>, Int<(1<<2)>>::value, "failed");
      static_assert(Same<Bit<3>, Int<(1<<3)>>::value, "failed");
      static_assert(Same<Bit<4>, Int<(1<<4)>>::value, "failed");
      static_assert(Same<Bit<31>, Int<(1<<31)>>::value, "failed");
      
      static_assert(Same<BitMask<0, 0>, Int<0>>::value, "failed");
      static_assert(Same<BitMask<0, 1>, Int<0>>::value, "failed");
      static_assert(Same<BitMask<1, 0>, Int<(1<<0)>>::value, "failed");
      static_assert(Same<BitMask<1, 1>, Int<(1<<1)>>::value, "failed");
      static_assert(Same<BitMask<1, 2>, Int<(1<<2)>>::value, "failed");
      static_assert(Same<BitMask<2, 0>, Int<(3<<0)>>::value, "failed");
      static_assert(Same<BitMask<2, 1>, Int<(3<<1)>>::value, "failed");
      static_assert(Same<BitMask<2, 2>, Int<(3<<2)>>::value, "failed");
      static_assert(Same<BitMask<4, 0>, Int<(15<<0)>>::value, "failed");
      static_assert(Same<BitMask<4, 2>, Int<(15<<2)>>::value, "failed");
      static_assert(Same<BitMask<31, 0>, Int<0x7fffffff>>::value, "failed");
      static_assert(Same<BitMask<31, 1>, Int<0xfffffffe>>::value, "failed");
      static_assert(Same<BitMask<32, 0>, Int<0xffffffff>>::value, "failed");
      
      static_assert(Same<Decrement<Int<10>>, Int<9>>::value, "failed");
      static_assert(Same<Increment<Int<9>>, Int<10>>::value, "failed");
      
      static_assert(Same<Plus<Int<10>, Int<3>>, Int<13>>::value, "failed");
      static_assert(Same<Minus<Int<10>, Int<3>>, Int<7>>::value, "failed");
      
      static_assert(Same<Multiply<Int<10>, Int<3>>, Int<30>>::value, "failed");
      static_assert(Same<Divide<Int<10>, Int<2>>, Int<5>>::value, "failed");
      
      static_assert(Same<Modulus<Int<10>, Int<3>>, Int<1>>::value, "failed");
      static_assert(Same<Modulus<Int<10>, Int<5>>, Int<0>>::value, "failed");
      
      static_assert(Same<LeftShift<Int<10>, Int<1>>, Int<20>>::value, "failed");
      static_assert(Same<LeftShift<Int<10>, Int<3>>, Int<80>>::value, "failed");
      static_assert(Same<RightShift<Int<10>, Int<1>>, Int<5>>::value, "failed");
      static_assert(Same<RightShift<Int<10>, Int<2>>, Int<2>>::value, "failed");
      
      static_assert(Same<PowerOfTwo<Int<0>>, Int<1>>::value, "failed");
      static_assert(Same<PowerOfTwo<Int<1>>, Int<2>>::value, "failed");
      static_assert(Same<PowerOfTwo<Int<4>>, Int<16>>::value, "failed");
      
      static_assert( LessThan<Int<4>, Int<5>>::value, "failed");
      static_assert(!LessThan<Int<5>, Int<5>>::value, "failed");
      static_assert(!LessThan<Int<6>, Int<5>>::value, "failed");
      static_assert(!GreaterThan<Int<4>, Int<5>>::value, "failed");
      static_assert(!GreaterThan<Int<5>, Int<5>>::value, "failed");
      static_assert( GreaterThan<Int<6>, Int<5>>::value, "failed");
      
      static_assert( LessThanOrEqualTo<Int<4>, Int<5>>::value, "failed");
      static_assert( LessThanOrEqualTo<Int<5>, Int<5>>::value, "failed");
      static_assert(!LessThanOrEqualTo<Int<6>, Int<5>>::value, "failed");
      static_assert(!GreaterThanOrEqualTo<Int<4>, Int<5>>::value, "failed");
      static_assert( GreaterThanOrEqualTo<Int<5>, Int<5>>::value, "failed");
      static_assert( GreaterThanOrEqualTo<Int<6>, Int<5>>::value, "failed");
      
      static_assert(Same<LesserOf<Int<4>, Int<5>>, Int<4>>::value, "failed");
      static_assert(Same<GreaterOf<Int<4>, Int<5>>, Int<5>>::value, "failed");
      
      static_assert(Same<And<Bool<true>, Bool<true>>, Bool<true>>::value, "failed");
      static_assert(Same<And<Bool<true>, Bool<false>>, Bool<false>>::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<true>>, Bool<false>>::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<false>>, Bool<false>>::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<true>>, Bool<true>>::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<false>>, Bool<true>>::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<true>>, Bool<true>>::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<false>>, Bool<false>>::value, "failed");
      static_assert(Same<Not<Bool<false>>, Bool<true>>::value, "failed");
      static_assert(Same<Not<Bool<true>>, Bool<false>>::value, "failed");
      
      static_assert(Same<BitwiseOr<Int<0x9>, Int<0x3>>, Int<0xb>>::value, "failed");
      static_assert(Same<BitwiseAnd<Int<0x9>, Int<0x3>>, Int<0x1>>::value, "failed");
      static_assert(Same<BitwiseNot<Char<0x55>>, Char<0xaa>>::value, "failed");
      
      static_assert(Same<LowBitPosition<Int<0x01>>, Int<0>>::value, "failed");
      static_assert(Same<LowBitPosition<Int<0x02>>, Int<1>>::value, "failed");
      static_assert(Same<LowBitPosition<Int<0x0a>>, Int<1>>::value, "failed");
      static_assert(Same<LowBitPosition<Int<0x0c>>, Int<2>>::value, "failed");
      static_assert(Same<LowBitPosition<Int<0x3c>>, Int<2>>::value, "failed");
      
      static_assert(Same<HighBitPosition<Int<0x01>>, Int<0>>::value, "failed");
      static_assert(Same<HighBitPosition<Int<0x02>>, Int<1>>::value, "failed");
      static_assert(Same<HighBitPosition<Int<0x0a>>, Int<3>>::value, "failed");
      static_assert(Same<HighBitPosition<Int<0x0c>>, Int<3>>::value, "failed");
      static_assert(Same<HighBitPosition<Int<0x3c>>, Int<5>>::value, "failed");
      
      static_assert(Same<BitCount<Int<0x01>>, Int<1>>::value, "failed");
      static_assert(Same<BitCount<Int<0x06>>, Int<2>>::value, "failed");
      static_assert(Same<BitCount<Int<0x3c>>, Int<4>>::value, "failed");
      static_assert(Same<BitCount<Int<0x81>>, Int<2>>::value, "failed");
      
      static_assert(IsZero<Int<0>>::value, "failed");
      static_assert(!IsZero<Int<1>>::value, "failed");
      static_assert(!IsZero<Int<-1>>::value, "failed");
      
      static_assert(!IsPowerOfTwo<Int<0>>::value, "failed");
      static_assert( IsPowerOfTwo<Int<1>>::value, "failed");
      static_assert( IsPowerOfTwo<Int<2>>::value, "failed");
      static_assert(!IsPowerOfTwo<Int<3>>::value, "failed");
      static_assert( IsPowerOfTwo<Int<4>>::value, "failed");
      static_assert(!IsPowerOfTwo<Int<5>>::value, "failed");
      static_assert(!IsPowerOfTwo<Int<6>>::value, "failed");
      static_assert(!IsPowerOfTwo<Int<7>>::value, "failed");
      static_assert( IsPowerOfTwo<Int<8>>::value, "failed");
      
    }
  }
}

#endif
