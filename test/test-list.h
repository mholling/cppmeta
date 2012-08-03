#ifndef CPPMETA_TEST_LIST_H
#define CPPMETA_TEST_LIST_H

#include <assert.h>
#include "meta/meta.h"
#include "meta/list.h"

namespace CppMeta {
  namespace Test {
    namespace TestList {
      static_assert(IsList<List<int, char>>::Result::value, "failed");
      static_assert(!IsList<int>::Result::value, "failed");

      static_assert(Same<Append<List<char, int>, float>::Result, List<char, int, float>>::Result::value, "failed");
      static_assert(Same<Append<List<>, float>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<char, int>>::Result, List<float, char, int>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<>>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Concat<List<char, int>, List<float, bool>>::Result, List<char, int, float, bool>>::Result::value, "failed");

      static_assert(Same<Reverse<List<float, char, bool, int>>::Result, List<int, bool, char, float>>::Result::value, "failed");
      
      template <typename T> using IsFloat = Same<T, float>;
      static_assert(Same<Map<List<int, char, float, bool>, IsFloat>::Result, List<Bool<false>,Bool<false>,Bool<true>,Bool<false>>>::Result::value, "failed");

      template <typename T, typename Index> struct GetIndex { using Result = Index; };
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetIndex>::Result, List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>>>::Result::value, "failed");
      template <typename T, typename Index> struct GetElement { using Result = T; };
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetElement>::Result, List<int, bool, char, long, float>>::Result::value, "failed");

      static_assert(Same<Select<List<int, char, float, bool, float>, IsFloat>::Result, List<float, float>>::Result::value, "failed");
      static_assert(Same<Reject<List<int, char, float, bool, float>, IsFloat>::Result, List<int, char, bool>>::Result::value, "failed");
      
      static_assert(Same<Find<List<bool, char, float, int>, IsFloat>::Result, float>::Result::value, "failed");

      static_assert(Length<List<int, int, float>>::Result::value == 3, "failed");
      static_assert(Length<List<>>::Result::value == 0, "failed");

      static_assert(Any<List<int, char>>::Result::value, "failed");
      static_assert(!Any<List<>>::Result::value, "failed");

      static_assert(!Empty<List<int, char>>::Result::value, "failed");
      static_assert(Empty<List<>>::Result::value, "failed");

      static_assert(Contains<List<int, char, float>, float>::Result::value, "failed");
      static_assert(!Contains<List<int, char, float>, bool>::Result::value, "failed");
      static_assert(!Contains<List<>, char>::Result::value, "failed");

      static_assert(Same<Unique<List<int, char, bool>>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<Unique<List<int, char, bool, char, float>>::Result, List<int, char, bool, float>>::Result::value, "failed");

      static_assert(Same<Index<List<int, char, bool>, int>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Index<List<int, char, bool, float>, bool>::Result, Int<2>>::Result::value, "failed");

      static_assert(Same<First<List<int, char, bool>>::Result, int>::Result::value, "failed");
      static_assert(Same<Last<List<int, char, bool>>::Result, bool>::Result::value, "failed");
      static_assert(Same<At<List<int, float, char, bool>, Int<2>>::Result, char>::Result::value, "failed");

      static_assert(Same<Flatten<List<int, List<char, bool>, float>>::Result, List<int, char, bool, float>>::Result::value, "failed");
      static_assert(Same<Flatten<List<int, List<bool, List<char, long>, List<double, List<float>>>>>::Result, List<int, bool, char, long, double, float>>::Result::value, "failed");

      static_assert(Same<BeforeIndex<List<int, char, bool, float>, Int<3>>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<BeforeIndex<List<int, char, bool, float>, Int<0>>::Result, List<>>::Result::value, "failed");
      static_assert(Same<AfterIndex<List<int, char, bool, float>, Int<1>>::Result, List<bool, float>>::Result::value, "failed");
      static_assert(Same<AfterIndex<List<int, char, bool, float>, Int<3>>::Result, List<>>::Result::value, "failed");
      static_assert(Same<Before<List<int, char, bool, float>, float>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<Before<List<int, char, bool, float>, int>::Result, List<>>::Result::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, char>::Result, List<bool, float>>::Result::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, float>::Result, List<>>::Result::value, "failed");

      using Unsorted = List<Int<1>, Int<6>, Int<0>, Int<4>, Int<3>, Int<2>, Int<5>>;
      using Sorted   = List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>, Int<5>, Int<6>>;
      static_assert(Same<Sort<Unsorted, LessThan>::Result, Sorted>::Result::value, "failed");
      
      static_assert(Same<Max<Unsorted>::Result, Int<6>>::Result::value, "failed");
      static_assert(Same<Min<Unsorted>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Max<List<Int<0>>>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Min<List<Int<0>>>::Result, Int<0>>::Result::value, "failed");
      
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
