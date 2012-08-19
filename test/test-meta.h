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
      
      struct Parent { };
      struct Child : Parent { };
      static_assert( IsOrInherits<Child, Parent>::value, "failed");
      static_assert(!IsOrInherits<Parent, Child>::value, "failed");
      static_assert(IsOrInherits<Parent, Parent>::value, "failed");
      static_assert( Inherits<Child, Parent>::value, "failed");
      static_assert(!Inherits<Parent, Child>::value, "failed");
      static_assert(!Inherits<Parent, Parent>::value, "failed");
      
      struct VoidReturn { void operator()() { } };
      struct BoolReturn { bool operator()() { return true; } };
      struct VoidReturnIntCharArgs { void operator()(int, char) { } };
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
      
      static_assert( IsVolatile<volatile int>::value, "failed");
      static_assert( IsVolatile<volatile const int>::value, "failed");
      static_assert(!IsVolatile<int>::value, "failed");
      static_assert(!IsVolatile<const int>::value, "failed");
      
      static_assert(!IsConst<volatile int>::value, "failed");
      static_assert( IsConst<volatile const int>::value, "failed");
      static_assert(!IsConst<int>::value, "failed");
      static_assert( IsConst<const int>::value, "failed");
      
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
