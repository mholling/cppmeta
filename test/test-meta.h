#ifndef CPPMETA_TEST_META_H
#define CPPMETA_TEST_META_H

#include <assert.h>
#include "meta/meta.h"

namespace CppMeta {
  namespace Test {
    namespace TestMeta {
      static_assert(Same<int, int>::value, "failed");
      static_assert(!Same<int, char>::value, "failed");
      static_assert(!Different<int, int>::value, "failed");
      static_assert(Different<int, char>::value, "failed");
      
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
      
      static_assert(Same<LeftShift<Int<10>, Int<1>>, Int<20>>::value, "failed");
      static_assert(Same<LeftShift<Int<10>, Int<3>>, Int<80>>::value, "failed");
      static_assert(Same<RightShift<Int<10>, Int<1>>, Int<5>>::value, "failed");
      static_assert(Same<RightShift<Int<10>, Int<2>>, Int<2>>::value, "failed");
      
      static_assert(LessThan<Int<4>, Int<5>>::value, "failed");
      static_assert(!LessThan<Int<5>, Int<5>>::value, "failed");
      static_assert(!LessThan<Int<6>, Int<5>>::value, "failed");
      static_assert(!GreaterThan<Int<4>, Int<5>>::value, "failed");
      static_assert(!GreaterThan<Int<5>, Int<5>>::value, "failed");
      static_assert(GreaterThan<Int<6>, Int<5>>::value, "failed");
      
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
      
      static_assert(Same<BitPosition<Int<0x01>>, Int<0>>::value, "failed");
      static_assert(Same<BitPosition<Int<0x02>>, Int<1>>::value, "failed");
      static_assert(Same<BitPosition<Int<0x0a>>, Int<1>>::value, "failed");
      static_assert(Same<BitPosition<Int<0x0c>>, Int<2>>::value, "failed");
      static_assert(Same<BitPosition<Int<0x3c>>, Int<2>>::value, "failed");
      
      static_assert(Same<BitCount<Int<0x01>>, Int<1>>::value, "failed");
      static_assert(Same<BitCount<Int<0x06>>, Int<2>>::value, "failed");
      static_assert(Same<BitCount<Int<0x3c>>, Int<4>>::value, "failed");
      static_assert(Same<BitCount<Int<0x81>>, Int<2>>::value, "failed");
      
      static_assert(Same<Self<int>, int>::value, "failed");
      static_assert(!Same<Self<bool>, int>::value, "failed");
      
      struct MakeChar { using Result = char; };
      template <typename Type> struct MakeType { using Result = Type; };
      static_assert(!CanEval<int>::value, "failed");
      static_assert(CanEval<MakeChar>::value, "failed");
      static_assert(CanEval<MakeType<long>>::value, "failed");
      static_assert(Same<Eval<int>, int>::value, "failed");
      static_assert(Same<Eval<MakeChar>, char>::value, "failed");
      static_assert(Same<Eval<MakeType<long>>, long>::value, "failed");
      
      static_assert(Same<If<Bool<true>, int, char>, int>::value, "failed");
      static_assert(Same<If<Bool<false>, int, char>, char>::value, "failed");
      
      static_assert(Same<LesserOf<Int<4>, Int<5>>, Int<4>>::value, "failed");
      static_assert(Same<GreaterOf<Int<4>, Int<5>>, Int<5>>::value, "failed");
      
      struct Parent { };
      struct Child : Parent { };
      static_assert( IsOrInherits<Child, Parent>::value, "failed");
      static_assert(!IsOrInherits<Parent, Child>::value, "failed");
      static_assert(IsOrInherits<Parent, Parent>::value, "failed");
      static_assert( Inherits<Child, Parent>::value, "failed");
      static_assert(!Inherits<Parent, Child>::value, "failed");
      static_assert(!Inherits<Parent, Parent>::value, "failed");
      
      struct VoidReturn { void operator ()() { } };
      struct BoolReturn { bool operator ()() { return true; } };
      struct VoidReturnIntCharArgs { void operator ()(int, char) { } };
      static_assert( HasCallOperator<VoidReturn, void>::value, "failed");
      static_assert(!HasCallOperator<BoolReturn, void>::value, "failed");
      static_assert( HasCallOperator<VoidReturnIntCharArgs, void, int, char>::value, "failed");
      static_assert(!HasCallOperator<VoidReturnIntCharArgs, void, char, int>::value, "failed");
      static_assert(!HasCallOperator<VoidReturn,            void, int, char>::value, "failed");
      static_assert(!HasBoolCallOperator<VoidReturn>::value, "failed");
      static_assert( HasBoolCallOperator<BoolReturn>::value, "failed");
      
      struct SomeClass;
      static_assert( IsClass<SomeClass>::value, "failed");
      static_assert(!IsClass<int>::value, "failed");
      
      struct CompleteClass { };
      struct IncompleteClass;
      static_assert( IsComplete<CompleteClass>::value, "failed");
      static_assert(!IsComplete<IncompleteClass>::value, "failed");
      static_assert(!IsIncomplete<CompleteClass>::value, "failed");
      static_assert( IsIncomplete<IncompleteClass>::value, "failed");
      
      int x;
      struct AddToX { void operator()(int y) { x += y; } };
      struct Add2ToX { void operator()() { x += 2; } };
      struct Add10ToX { void operator()() { x += 10; } };
      
      void test() {
        x = 0;
        DoIf<Bool<false>, AddToX>()(3);
        assert(x == 0);
        DoIf<Bool<false>, Add2ToX>()();
        assert(x == 0);
        DoIf<Bool<false>, Add2ToX, Add10ToX>()();
        assert(x == 10);
        DoIf<Bool<true>, AddToX>()(3);
        assert(x == 13);
        DoIf<Bool<true>, Add2ToX>()();
        assert(x == 15);
        DoIf<Bool<true>, Add2ToX, Add10ToX>()();
        assert(x == 17);
      }
    }
  }
}

#endif
