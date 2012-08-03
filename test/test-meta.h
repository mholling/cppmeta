#ifndef CPPMETA_TEST_META_H
#define CPPMETA_TEST_META_H

#include <assert.h>
#include "meta/meta.h"

namespace CppMeta {
  namespace Test {
    namespace TestMeta {
      static_assert(Same<int, int>::Result::value, "failed");
      static_assert(!Same<int, char>::Result::value, "failed");
      static_assert(!Different<int, int>::Result::value, "failed");
      static_assert(Different<int, char>::Result::value, "failed");
      
      static_assert(Same<Decrement<Int<10>>::Result, Int<9>>::Result::value, "failed");
      static_assert(Same<Increment<Int<9>>::Result, Int<10>>::Result::value, "failed");
      
      static_assert(Same<Plus<Int<10>, Int<3>>::Result, Int<13>>::Result::value, "failed");
      static_assert(Same<Minus<Int<10>, Int<3>>::Result, Int<7>>::Result::value, "failed");
      
      static_assert(LessThan<Int<4>, Int<5>>::Result::value, "failed");
      static_assert(!LessThan<Int<5>, Int<5>>::Result::value, "failed");
      static_assert(!LessThan<Int<6>, Int<5>>::Result::value, "failed");
      static_assert(!GreaterThan<Int<4>, Int<5>>::Result::value, "failed");
      static_assert(!GreaterThan<Int<5>, Int<5>>::Result::value, "failed");
      static_assert(GreaterThan<Int<6>, Int<5>>::Result::value, "failed");
      
      static_assert(Same<And<Bool<true>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<And<Bool<true>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<true>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<false>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<Not<Bool<false>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Not<Bool<true>>::Result, Bool<false>>::Result::value, "failed");
      
      static_assert(Same<Self<int>::Result, int>::Result::value, "failed");
      static_assert(!Same<Self<bool>::Result, int>::Result::value, "failed");

      struct MakeChar { using Result = char; };
      template <typename Type> struct MakeType { using Result = Type; };
      static_assert(!CanEval<int>::Result::value, "failed");
      static_assert(CanEval<MakeChar>::Result::value, "failed");
      static_assert(CanEval<MakeType<long>>::Result::value, "failed");
      static_assert(Same<Eval<int>::Result, int>::Result::value, "failed");
      static_assert(Same<Eval<MakeChar>::Result, char>::Result::value, "failed");
      static_assert(Same<Eval<MakeType<long>>::Result, long>::Result::value, "failed");

      static_assert(Same<If<Bool<true>, int, char>::Result, int>::Result::value, "failed");
      static_assert(Same<If<Bool<false>, int, char>::Result, char>::Result::value, "failed");
      
      static_assert(Same<LesserOf<Int<4>, Int<5>>::Result, Int<4>>::Result::value, "failed");
      static_assert(Same<GreaterOf<Int<4>, Int<5>>::Result, Int<5>>::Result::value, "failed");

      struct Parent { };
      struct Child : Parent { };
      static_assert( IsOrInherits<Child, Parent>::Result::value, "failed");
      static_assert(!IsOrInherits<Parent, Child>::Result::value, "failed");
      static_assert(IsOrInherits<Parent, Parent>::Result::value, "failed");
      static_assert( Inherits<Child, Parent>::Result::value, "failed");
      static_assert(!Inherits<Parent, Child>::Result::value, "failed");
      static_assert(!Inherits<Parent, Parent>::Result::value, "failed");

      struct VoidReturn { void operator ()() { } };
      struct BoolReturn { bool operator ()() { return true; } };
      struct VoidReturnIntCharArgs { void operator ()(int, char) { } };
      static_assert( HasCallOperator<VoidReturn, void>::Result::value, "failed");
      static_assert(!HasCallOperator<BoolReturn, void>::Result::value, "failed");
      static_assert( HasCallOperator<VoidReturnIntCharArgs, void, int, char>::Result::value, "failed");
      static_assert(!HasCallOperator<VoidReturnIntCharArgs, void, char, int>::Result::value, "failed");
      static_assert(!HasCallOperator<VoidReturn,            void, int, char>::Result::value, "failed");
      static_assert(!HasBoolCallOperator<VoidReturn>::Result::value, "failed");
      static_assert( HasBoolCallOperator<BoolReturn>::Result::value, "failed");
      
      struct SomeClass;
      static_assert( IsClass<SomeClass>::Result::value, "failed");
      static_assert(!IsClass<int>::Result::value, "failed");
      
      struct CompleteClass { };
      struct IncompleteClass;
      static_assert( IsComplete<CompleteClass>::Result::value, "failed");
      static_assert(!IsComplete<IncompleteClass>::Result::value, "failed");
      static_assert(!IsIncomplete<CompleteClass>::Result::value, "failed");
      static_assert( IsIncomplete<IncompleteClass>::Result::value, "failed");
      
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
