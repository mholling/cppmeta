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
      
      static_assert(IsTree<TreeA>::value, "failed");
      static_assert(IsTree<TreeADAA>::value, "failed");
      static_assert(!IsTree<int>::value, "failed");
      
      static_assert(IsLeaf<TreeADAA>::value, "failed");
      static_assert(!IsLeaf<TreeA>::value, "failed");
      
      static_assert(Contains<TreeA, A>::value, "failed");
      static_assert(Contains<TreeA, ADAB>::value, "failed");
      static_assert(!Contains<TreeA, float>::value, "failed");
      
      static_assert(Same<FindBranch<TreeA, A>, TreeA>::value, "failed");
      static_assert(Same<FindBranch<TreeA, AD>, TreeAD>::value, "failed");
      static_assert(Same<FindBranch<TreeA, ADAB>, TreeADAB>::value, "failed");
      
      static_assert(Same<Root<TreeA>, A>::value, "failed");
      
      static_assert(Same<Children<TreeA, A>, List<AA, AB, AC, AD>>::value, "failed");
      static_assert(Same<Children<TreeA, AB>, List<ABA, ABB, ABC>>::value, "failed");
      
      static_assert(Same<Ancestors<TreeA, A>, List<>>::value, "failed");
      static_assert(Same<Ancestors<TreeA, AB>, List<A>>::value, "failed");
      static_assert(Same<Ancestors<TreeA, ABC>, List<AB, A>>::value, "failed");
      static_assert(Same<Ancestors<TreeA, ADAB>, List<ADA, AD, A>>::value, "failed");
      
      static_assert(Same<SelfAndAncestors<TreeA, A>, List<A>>::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, AB>, List<AB, A>>::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ABC>, List<ABC, AB, A>>::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ADAB>, List<ADAB, ADA, AD, A>>::value, "failed");
      
      static_assert(Same<Parent<TreeA, AB>, A>::value, "failed");
      static_assert(Same<Parent<TreeA, ABB>, AB>::value, "failed");
      static_assert(Same<Parent<TreeA, ADAB>, ADA>::value, "failed");
      
      static_assert(Same<CommonBranch<TreeA, ADAB, AAB>, TreeA>::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAB, ADB>, TreeAD>::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, ADAB>, TreeADA>::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, AD>, TreeAD>::value, "failed");
      static_assert(Same<CommonBranch<TreeA, AD, AD>, TreeAD>::value, "failed");
      
      static_assert(Same<CommonAncestor<TreeA, ADAB, AAB>, A>::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAB, ADB>, AD>::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, ADAB>, ADA>::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, AD>, AD>::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, AD, AD>, AD>::value, "failed");
      
      static_assert(Same<Flatten<TreeA>, List<A, AA, AAA, AAB, AB, ABA, ABB, ABC, AC, AD, ADA, ADAA, ADAB, ADB>>::value, "failed");
      
      static_assert(Same<Index<TreeA, A>, Int<0>>::value, "failed");
      static_assert(Same<Index<TreeA, AA>, Int<1>>::value, "failed");
      static_assert(Same<Index<TreeA, AAA>, Int<2>>::value, "failed");
      static_assert(Same<Index<TreeA, AAB>, Int<3>>::value, "failed");
      static_assert(Same<Index<TreeA, AB>, Int<4>>::value, "failed");
      static_assert(Same<Index<TreeA, ABA>, Int<5>>::value, "failed");
      static_assert(Same<Index<TreeA, ABB>, Int<6>>::value, "failed");
      
      static_assert(Same<Leaves<TreeA>, List<AAA, AAB, ABA, ABB, ABC, AC, ADAA, ADAB, ADB>>::value, "failed");
      
      template <typename Type> using IsADA = Same<ADA, Type>;
      static_assert(Same<Find<TreeA, IsADA>, ADA>::value, "failed");
      
      static_assert(Same<Height<TreeA>, Int<3>>::value, "failed");
      static_assert(Same<Height<TreeAA>, Int<1>>::value, "failed");
      static_assert(Same<Height<TreeAC>, Int<0>>::value, "failed");
      
      static_assert(Same<Depth<TreeA, A>, Int<0>>::value, "failed");
      static_assert(Same<Depth<TreeA, AA>, Int<1>>::value, "failed");
      static_assert(Same<Depth<TreeA, ADAA>, Int<3>>::value, "failed");
      
      static_assert(Same<Distance<TreeA, A, A>, Int<0>>::value, "failed");
      static_assert(Same<Distance<TreeA, A, AA>, Int<1>>::value, "failed");
      static_assert(Same<Distance<TreeA, A, AAA>, Int<2>>::value, "failed");
      static_assert(Same<Distance<TreeA, AA, AB>, Int<2>>::value, "failed");
      static_assert(Same<Distance<TreeA, ABA, ABC>, Int<2>>::value, "failed");
      static_assert(Same<Distance<TreeA, ADAB, ABB>, Int<5>>::value, "failed");
    }
  }
}

#endif
