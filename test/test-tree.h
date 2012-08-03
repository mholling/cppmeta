#ifndef CPPMETA_TEST_TREE_H
#define CPPMETA_TEST_TREE_H

#include "meta/meta.h"
#include "meta/tree.h"

namespace CppMeta {
  namespace Test {
    namespace TestTree {
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
      
      static_assert(Same<FindBranch<TreeA, A>::Result, TreeA>::Result::value, "failed");
      static_assert(Same<FindBranch<TreeA, AD>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<FindBranch<TreeA, ADAB>::Result, TreeADAB>::Result::value, "failed");
      
      static_assert(Same<Root<TreeA>::Result, A>::Result::value, "failed");
      
      static_assert(Same<Children<TreeA, A>::Result, List<AA, AB, AC, AD>>::Result::value, "failed");
      static_assert(Same<Children<TreeA, AB>::Result, List<ABA, ABB, ABC>>::Result::value, "failed");
      
      static_assert(Same<Ancestors<TreeA, A>::Result, List<>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, AB>::Result, List<A>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, ABC>::Result, List<AB, A>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, ADAB>::Result, List<ADA, AD, A>>::Result::value, "failed");
      
      static_assert(Same<SelfAndAncestors<TreeA, A>::Result, List<A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, AB>::Result, List<AB, A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ABC>::Result, List<ABC, AB, A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ADAB>::Result, List<ADAB, ADA, AD, A>>::Result::value, "failed");
      
      static_assert(Same<Parent<TreeA, AB>::Result, A>::Result::value, "failed");
      static_assert(Same<Parent<TreeA, ABB>::Result, AB>::Result::value, "failed");
      static_assert(Same<Parent<TreeA, ADAB>::Result, ADA>::Result::value, "failed");
      
      static_assert(Same<CommonBranch<TreeA, ADAB, AAB>::Result, TreeA>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAB, ADB>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, ADAB>::Result, TreeADA>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, AD>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, AD, AD>::Result, TreeAD>::Result::value, "failed");
      
      static_assert(Same<CommonAncestor<TreeA, ADAB, AAB>::Result, A>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAB, ADB>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, ADAB>::Result, ADA>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, AD>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, AD, AD>::Result, AD>::Result::value, "failed");
      
      static_assert(Same<Flatten<TreeA>::Result, List<A, AA, AAA, AAB, AB, ABA, ABB, ABC, AC, AD, ADA, ADAA, ADAB, ADB>>::Result::value, "failed");
      
      static_assert(Same<Index<TreeA, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AAA>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AAB>::Result, Int<3>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AB>::Result, Int<4>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, ABA>::Result, Int<5>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, ABB>::Result, Int<6>>::Result::value, "failed");
      
      static_assert(Same<Leaves<TreeA>::Result, List<AAA, AAB, ABA, ABB, ABC, AC, ADAA, ADAB, ADB>>::Result::value, "failed");
      
      template <typename Type> using IsADA = Same<ADA, Type>;
      static_assert(Same<Find<TreeA, IsADA>::Result, ADA>::Result::value, "failed");
      
      static_assert(Same<Height<TreeA>::Result, Int<3>>::Result::value, "failed");
      static_assert(Same<Height<TreeAA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Height<TreeAC>::Result, Int<0>>::Result::value, "failed");
      
      static_assert(Same<Depth<TreeA, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Depth<TreeA, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Depth<TreeA, ADAA>::Result, Int<3>>::Result::value, "failed");
      
      static_assert(Same<Distance<TreeA, A, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, A, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, A, AAA>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, AA, AB>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, ABA, ABC>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, ADAB, ABB>::Result, Int<5>>::Result::value, "failed");
    }
  }
}

#endif
