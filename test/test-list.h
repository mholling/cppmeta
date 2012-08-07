#ifndef CPPMETA_TEST_LIST_H
#define CPPMETA_TEST_LIST_H

#include <assert.h>
#include "meta/meta.h"
#include "meta/list.h"

namespace CppMeta {
  namespace Test {
    namespace TestList {
      static_assert(IsList<List<int, char>>::value, "failed");
      static_assert(!IsList<int>::value, "failed");

      static_assert(Same<Append<List<char, int>, float>, List<char, int, float>>::value, "failed");
      static_assert(Same<Append<List<>, float>, List<float>>::value, "failed");
      static_assert(Same<Prepend<float, List<char, int>>, List<float, char, int>>::value, "failed");
      static_assert(Same<Prepend<float, List<>>, List<float>>::value, "failed");
      static_assert(Same<Concat<List<char, int>, List<float, bool>>, List<char, int, float, bool>>::value, "failed");

      static_assert(Same<Reverse<List<float, char, bool, int>>, List<int, bool, char, float>>::value, "failed");
      
      template <typename T> using IsFloat = Same<T, float>;
      static_assert(Same<Map<List<int, char, float, bool>, IsFloat>, List<Bool<false>,Bool<false>,Bool<true>,Bool<false>>>::value, "failed");

      template <typename T, typename Index> using GetIndex = Index;
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetIndex>, List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>>>::value, "failed");
      template <typename T, typename Index> using GetElement = T;
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetElement>, List<int, bool, char, long, float>>::value, "failed");

      static_assert(Same<Select<List<int, char, float, bool, float>, IsFloat>, List<float, float>>::value, "failed");
      static_assert(Same<Reject<List<int, char, float, bool, float>, IsFloat>, List<int, char, bool>>::value, "failed");
      
      static_assert(Same<Find<List<bool, char, float, int>, IsFloat>, float>::value, "failed");

      static_assert(Length<List<int, int, float>>::value == 3, "failed");
      static_assert(Length<List<>>::value == 0, "failed");

      static_assert(Any<List<int, char>>::value, "failed");
      static_assert(!Any<List<>>::value, "failed");

      static_assert(Many<List<int, char>>::value, "failed");
      static_assert(!Many<List<int>>::value, "failed");
      static_assert(!Many<List<>>::value, "failed");

      static_assert(!Empty<List<int, char>>::value, "failed");
      static_assert(Empty<List<>>::value, "failed");

      static_assert(Contains<List<int, char, float>, float>::value, "failed");
      static_assert(!Contains<List<int, char, float>, bool>::value, "failed");
      static_assert(!Contains<List<>, char>::value, "failed");

      static_assert(Same<Unique<List<int, char, bool>>, List<int, char, bool>>::value, "failed");
      static_assert(Same<Unique<List<int, char, bool, char, float>>, List<int, char, bool, float>>::value, "failed");

      static_assert(Same<Index<List<int, char, bool>, int>, Int<0>>::value, "failed");
      static_assert(Same<Index<List<int, char, bool, float>, bool>, Int<2>>::value, "failed");

      static_assert(Same<First<List<int, char, bool>>, int>::value, "failed");
      static_assert(Same<Last<List<int, char, bool>>, bool>::value, "failed");
      static_assert(Same<At<List<int, float, char, bool>, Int<2>>, char>::value, "failed");

      static_assert(Same<Flatten<List<int, List<char, bool>, float>>, List<int, char, bool, float>>::value, "failed");
      static_assert(Same<Flatten<List<int, List<bool, List<char, long>, List<double, List<float>>>>>, List<int, bool, char, long, double, float>>::value, "failed");

      static_assert(Same<BeforeIndex<List<int, char, bool, float>, Int<3>>, List<int, char, bool>>::value, "failed");
      static_assert(Same<BeforeIndex<List<int, char, bool, float>, Int<0>>, List<>>::value, "failed");
      static_assert(Same<AfterIndex<List<int, char, bool, float>, Int<1>>, List<bool, float>>::value, "failed");
      static_assert(Same<AfterIndex<List<int, char, bool, float>, Int<3>>, List<>>::value, "failed");
      static_assert(Same<Before<List<int, char, bool, float>, float>, List<int, char, bool>>::value, "failed");
      static_assert(Same<Before<List<int, char, bool, float>, int>, List<>>::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, char>, List<bool, float>>::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, float>, List<>>::value, "failed");

      using Unsorted = List<Int<1>, Int<6>, Int<0>, Int<4>, Int<3>, Int<2>, Int<5>>;
      using Sorted   = List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>, Int<5>, Int<6>>;
      static_assert(Same<Sort<Unsorted, LessThan>, Sorted>::value, "failed");
      
      static_assert(Same<Max<Unsorted>, Int<6>>::value, "failed");
      static_assert(Same<Min<Unsorted>, Int<0>>::value, "failed");
      static_assert(Same<Max<List<Int<0>>>, Int<0>>::value, "failed");
      static_assert(Same<Min<List<Int<0>>>, Int<0>>::value, "failed");
      
      static_assert(Same<Sequence<Int<-1>, Int<3>>, List<Int<-1>, Int<0>, Int<1>, Int<2>, Int<3>>>::value, "failed");
      
      int x, y, z;
      struct SetX { void operator()(int v) { x = v; } };
      struct SetY { void operator()(int v) { y = v; } };
      struct SetZ { void operator()(int v) { z = v; } };
      using Setters = List<SetX, SetY, SetZ>;
      
      bool a[4] = { false };
      template <typename I>
      struct SetA {
        using Result = SetA;
        void operator()() { a[I::value] = true; }
      };
      
      template <typename I>
      struct GetA {
        using Result = GetA;
        bool operator()() { return a[I::value]; }
      };
      
      
      int count;
      struct StartFalse { bool operator()() { count++; return false; } };
      struct StillFalse { bool operator()() { count++; return false; } };
      struct NowTrue    { bool operator()() { count++; return true; } };
      struct TrueAgain  { bool operator()() { count++; return true; } };
      struct FinalFalse { bool operator()() { count++; return false; } };
      using SomeSuccesses = List<StartFalse, StillFalse, NowTrue, TrueAgain, FinalFalse>;
      using AllFailures =  List<StartFalse, StillFalse, FinalFalse>;
      using SucceedTwiceThenFail = List<NowTrue, TrueAgain, FinalFalse>;
      using SucceedTwice = List<NowTrue, TrueAgain>;
      
      void test() {
        Each<List<SetX, SetY, SetZ>>()(10);
        assert(x == 10 && y == 10 && z == 10);
        
        Each<List<Int<1>, Int<3>>, SetA>()();
        assert(!a[0] && a[1] && !a[2] && a[3]);
        
        count = 0;
        bool result;
        assert(result = Until<SomeSuccesses>()());
        assert(count == 3);
        
        count = 0;
        assert(result = !Until<AllFailures>()());
        assert(count == 3);
        
        result = Until<List<Int<0>, Int<1>, Int<2>, Int<3>>, GetA>()();
        assert(result);
        
        result = !Until<List<Int<0>, Int<2>>, GetA>()();
        assert(result);
        
        count = 0;
        assert(result = While<SucceedTwice>()());
        assert(count == 2);
        
        count = 0;
        assert(result = !While<SucceedTwiceThenFail>()());
        assert(count == 3);
        
        result = While<List<Int<1>, Int<3>>, GetA>()();
        assert(result);
        
        result = !While<List<Int<1>, Int<2>>, GetA>()();
        assert(result);
      }
    };
  }
}

#endif
