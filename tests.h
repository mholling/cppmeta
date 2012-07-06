namespace Meta {
  namespace Tests {
    namespace TestBasics {
      static_assert(Same<int, int>::Result::value, "failed");
      static_assert(!Same<int, char>::Result::value, "failed");
      
      static_assert(Same<Self<int>::Result, int>::Result::value, "failed");
      static_assert(!Same<Self<bool>::Result, int>::Result::value, "failed");

      struct MakeChar { typedef char Result; };
      template <typename Type> struct MakeType { typedef Type Result; };
      static_assert(!CanEval<int>::Result::value, "failed");
      static_assert(CanEval<MakeChar>::Result::value, "failed");
      static_assert(CanEval<MakeType<long>>::Result::value, "failed");
      static_assert(Same<Eval<int>::Result, int>::Result::value, "failed");
      static_assert(Same<Eval<MakeChar>::Result, char>::Result::value, "failed");
      static_assert(Same<Eval<MakeType<long>>::Result, long>::Result::value, "failed");

      static_assert(Same<If<Bool<true>, int, char>::Result, int>::Result::value, "failed");
      static_assert(Same<If<Bool<false>, int, char>::Result, char>::Result::value, "failed");

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
    }
    namespace TestLists {
      static_assert(IsList<List<int, char>>::Result::value, "failed");
      static_assert(!IsList<int>::Result::value, "failed");

      static_assert(Same<Head<List<int, char>>::Result, int>::Result::value, "failed");
      static_assert(!Same<Head<List<int, char>>::Result, char>::Result::value, "failed");

      static_assert(Same<Append<List<char, int>, float>::Result, List<char, int, float>>::Result::value, "failed");
      static_assert(Same<Append<List<>, float>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<char, int>>::Result, List<float, char, int>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<>>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Concat<List<char, int>, List<float, bool>>::Result, List<char, int, float, bool>>::Result::value, "failed");

      template <typename T> struct IsFloat { typedef typename Same<T, float>::Result Result; };
      static_assert(Same<Map<List<int, char, float, bool>, IsFloat>::Result, List<Bool<false>,Bool<false>,Bool<true>,Bool<false>>>::Result::value, "failed");

      template <typename T, typename Index> struct IndexIsEven;
      template <typename T, int index> struct IndexIsEven<T, Int<index>> { typedef Bool<index % 2 == 0> Result; };
      static_assert(Same<MapWithIndex<List<int, int, int, int, int>, IndexIsEven>::Result, List<Bool<true>, Bool<false>, Bool<true>, Bool<false>, Bool<true>>>::Result::value, "failed");

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

      static_assert(Same<Before<List<int, char, bool, float>, Int<3>>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<Before<List<int, char, bool, float>, bool  >::Result, List<int, char>>::Result::value, "failed");

      static_assert(Same<After<List<int, char, bool, float>, Int<1>>::Result, List<bool, float>>::Result::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, char  >::Result, List<bool, float>>::Result::value, "failed");

      template <typename, typename> struct LessThan;
      template <char n1, char n2> struct LessThan<Char<n1>, Char<n2>> { typedef Bool<n1 < n2> Result; };
      typedef List<Char<1>, Char<6>, Char<0>, Char<4>, Char<3>, Char<2>, Char<5>> Unsorted;
      typedef List<Char<0>, Char<1>, Char<2>, Char<3>, Char<4>, Char<5>, Char<6>> Sorted;
      static_assert(Same<Sort<Unsorted, LessThan>::Result, Sorted>::Result::value, "failed");
    }
    namespace TestTrees {
      struct A; struct AA; struct AB; struct AC; struct AD; struct AAA; struct AAB; struct ABA; struct ABB; struct ABC; struct ADA; struct ADB; struct ADAA; struct ADAB;
      
          typedef Tree<AAA> TreeAAA;
          typedef Tree<AAB> TreeAAB;
        typedef Tree<AA, TreeAAA, TreeAAB> TreeAA;
          typedef Tree<ABA> TreeABA;
          typedef Tree<ABB> TreeABB;
          typedef Tree<ABC> TreeABC;
        typedef Tree<AB, TreeABA, TreeABB, TreeABC> TreeAB;
        typedef Tree<AC> TreeAC;
            typedef Tree<ADAA> TreeADAA;
            typedef Tree<ADAB> TreeADAB;
          typedef Tree<ADA, TreeADAA, TreeADAB> TreeADA;
          typedef Tree<ADB> TreeADB;
        typedef Tree<AD, TreeADA, TreeADB> TreeAD;
      typedef Tree<A, TreeAA, TreeAB, TreeAC, TreeAD> TreeA;
      
      static_assert(IsTree<TreeA>::Result::value, "failed");
      static_assert(IsTree<TreeADAA>::Result::value, "failed");
      static_assert(!IsTree<int>::Result::value, "failed");
      
      static_assert(IsLeaf<TreeADAA>::Result::value, "failed");
      static_assert(!IsLeaf<TreeA>::Result::value, "failed");
      
      static_assert(Contains<TreeA, A>::Result::value, "failed");
      static_assert(Contains<TreeA, ADAB>::Result::value, "failed");
      static_assert(!Contains<TreeA, float>::Result::value, "failed");
      
      static_assert(Same<Root<TreeA>::Result, A>::Result::value, "failed");
      
      static_assert(Same<Children<A,TreeA>::Result, List<AA, AB, AC, AD>>::Result::value, "failed");
      static_assert(Same<Children<AB,TreeA>::Result, List<ABA, ABB, ABC>>::Result::value, "failed");
      
      static_assert(Same<Ancestors<A, TreeA>::Result, List<>>::Result::value, "failed");
      static_assert(Same<Ancestors<AB, TreeA>::Result, List<A>>::Result::value, "failed");
      static_assert(Same<Ancestors<ABC, TreeA>::Result, List<AB, A>>::Result::value, "failed");
      static_assert(Same<Ancestors<ADAB, TreeA>::Result, List<ADA, AD, A>>::Result::value, "failed");
      
      static_assert(Same<Parent<AB, TreeA>::Result, A>::Result::value, "failed");
      static_assert(Same<Parent<ABB, TreeA>::Result, AB>::Result::value, "failed");
      static_assert(Same<Parent<ADAB, TreeA>::Result, ADA>::Result::value, "failed");
      
      static_assert(Same<CommonBranch<ADAB, AAB, TreeA>::Result, TreeA>::Result::value, "failed");
      static_assert(Same<CommonBranch<ADAB, ADB, TreeA>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<ADAA, ADAB, TreeA>::Result, TreeADA>::Result::value, "failed");
      static_assert(Same<CommonBranch<ADAA, AD, TreeA>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<AD, AD, TreeA>::Result, TreeAD>::Result::value, "failed");
      
      static_assert(Same<CommonNode<ADAB, AAB, TreeA>::Result, A>::Result::value, "failed");
      static_assert(Same<CommonNode<ADAB, ADB, TreeA>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonNode<ADAA, ADAB, TreeA>::Result, ADA>::Result::value, "failed");
      static_assert(Same<CommonNode<ADAA, AD, TreeA>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonNode<AD, AD, TreeA>::Result, AD>::Result::value, "failed");
      
      static_assert(Same<Flatten<TreeA>::Result, List<A, AA, AAA, AAB, AB, ABA, ABB, ABC, AC, AD, ADA, ADAA, ADAB, ADB>>::Result::value, "failed");
      
      template <typename Type> using IsADA = Same<ADA, Type>;
      static_assert(Same<Find<TreeA, IsADA>::Result, ADA>::Result::value, "failed");
    }
  }
}