#ifndef CPPMETA_META_TREE_H
#define CPPMETA_META_TREE_H

#include "meta.h"
#include "list.h"

namespace CppMeta {
  template <typename Node, typename... Branches> struct Tree;
  
  template <typename T> struct IsTreeImpl { using Result = Bool<false>; };
  template <typename Node, typename... Branches> struct IsTreeImpl<Tree<Node, Branches...>> { using Result = Bool<true>; };
  template <typename T> using IsTree = typename IsTreeImpl<T>::Result;
  
  template <typename Tree> struct IsLeafImpl;
  template <typename Node, typename...Branches>
  struct IsLeafImpl<Tree<Node, Branches...>> { using Result = Bool<false>; };
  template <typename Node>
  struct IsLeafImpl<Tree<Node>> { using Result = Bool<true>; };
  template <typename Tree> using IsLeaf = typename IsLeafImpl<Tree>::Result;
  
  template <typename Tree> struct RootImpl;
  template <typename Node, typename... Branches>
  struct RootImpl<Tree<Node, Branches...>> { using Result = Node; };
  template <typename Tree> using Root = typename RootImpl<Tree>::Result;
  
  template <typename Tree, typename Type> struct ContainsImpl;
  template <typename Tree, typename Type> using Contains = typename ContainsImpl<Tree, Type>::Result;
  template <typename Type, typename Node, typename... Branches>
  struct ContainsImpl<Tree<Node, Branches...>, Type> {
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using Result = Any<List<Branches...>, ContainsType>;
  };
  template <typename Node, typename... Branches>
  struct ContainsImpl<Tree<Node, Branches...>, Node> { using Result = Bool<true>; };
  
  template <typename Tree, typename Type> struct FindBranchImpl;
  template <typename Tree, typename Type> using FindBranch = typename FindBranchImpl<Tree, Type>::Result;
  template <typename Type, typename Node, typename... Branches>
  struct FindBranchImpl<Tree<Node, Branches...>, Type> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using ContainingBranch = Find<List<Branches...>, ContainsType>;
    using Result = FindBranch<ContainingBranch, Type>;
  };
  template <typename Type, typename... Branches>
  struct FindBranchImpl<Tree<Type, Branches...>, Type> { using Result = Tree<Type, Branches...>; };
  
  template <typename Tree, typename Type> struct ChildrenImpl;
  template <typename Tree, typename Type> using Children = typename ChildrenImpl<Tree, Type>::Result;
  template <typename Type, typename Node, typename... Branches>
  struct ChildrenImpl<Tree<Node, Branches...>, Type> { using Result = Children<FindBranch<Tree<Node, Branches...>, Type>, Type>; };
  template <typename Type, typename... Branches>
  struct ChildrenImpl<Tree<Type, Branches...>, Type> { using Result = Map<List<Branches...>, Root>; };
  
  template <typename Tree, typename Type, typename IncludeSelf> struct LineageImpl;
  template <typename Tree, typename Type, typename IncludeSelf> using Lineage = typename LineageImpl<Tree, Type, IncludeSelf>::Result;
  template <typename Type, typename IncludeSelf, typename Node, typename... Branches>
  struct LineageImpl<Tree<Node, Branches...>, Type, IncludeSelf> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using ContainingBranch = Find<List<Branches...>, ContainsType>;
    using Result = Append<Lineage<ContainingBranch, Type, IncludeSelf>, Node>;
  };
  template <typename Type, typename IncludeSelf, typename... Branches>
  struct LineageImpl<Tree<Type, Branches...>, Type, IncludeSelf> { using Result = If<IncludeSelf, List<Type>, List<>>; };
  
  template <typename Branch, typename Type> using Ancestors = Lineage<Branch, Type, Bool<false>>;
  template <typename Branch, typename Type> using SelfAndAncestors = Lineage<Branch, Type, Bool<true>>;
  template <typename Branch, typename Type> using Parent = First<Ancestors<Branch, Type>>;
  
  template <typename Tree, typename Type1, typename Type2> struct CommonBranchImpl;
  template <typename Tree, typename Type1, typename Type2> using CommonBranch = typename CommonBranchImpl<Tree, Type1, Type2>::Result;
  template <typename Type1, typename Type2, typename Node, typename... Branches>
  struct CommonBranchImpl<Tree<Node, Branches...>, Type1, Type2> {
    static_assert(Contains<Tree<Node, Branches...>, Type1>::value, "tree does not contain specified type");
    static_assert(Contains<Tree<Node, Branches...>, Type2>::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType1 = Contains<Branch, Type1>;
    template <typename Branch> using ContainsType2 = Contains<Branch, Type2>;
    using BranchContainingType1 = Find<List<Branches...>, ContainsType1>;
    using BranchContainingType2 = Find<List<Branches...>, ContainsType2>;
    using Result = If<Same<BranchContainingType1, BranchContainingType2>, CommonBranchImpl<BranchContainingType1, Type1, Type2>, Tree<Node, Branches...>>;
  };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranchImpl<Tree<Type1, Branches...>, Type1, Type2> { using Result = Tree<Type1, Branches...>; };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranchImpl<Tree<Type2, Branches...>, Type1, Type2> { using Result = Tree<Type2, Branches...>; };
  template <typename Type, typename... Branches>
  struct CommonBranchImpl<Tree<Type, Branches...>, Type, Type> { using Result = Tree<Type, Branches...>; };
  
  template <typename Branch, typename Type1, typename Type2> using CommonAncestor = Root<CommonBranch<Branch, Type1, Type2>>;
  
  template <typename Tree> struct FlattenImpl;
  template <typename Tree> using Flatten = typename FlattenImpl<Tree>::Result;
  template <typename Node, typename... Branches>
  struct FlattenImpl<Tree<Node, Branches...>> { using Result = Prepend<Node, Flatten<Map<List<Branches...>, Flatten>>>; };
  
  template <typename Tree, typename Type> struct IndexImpl;
  template <typename Tree, typename Type> using Index = typename IndexImpl<Tree, Type>::Result;
  template <typename Type, typename Node, typename... Branches>
  struct IndexImpl<Tree<Node, Branches...>, Type> { using Result = Index<Flatten<Tree<Node, Branches...>>, Type>; };
  
  template <typename Tree> struct LeavesImpl;
  template <typename Tree> using Leaves = typename LeavesImpl<Tree>::Result;
  template <typename Node, typename... Branches>
  struct LeavesImpl<Tree<Node, Branches...>> { using Result = Inject<Map<List<Branches...>, Leaves>, Concat, List<>>; };
  template <typename Node>
  struct LeavesImpl<Tree<Node>> { using Result = List<Node>; };
  
  template <typename Tree, template <typename> class Predicate> struct FindImpl;
  template <typename Tree, template <typename> class Predicate> using Find = typename FindImpl<Tree, Predicate>::Result;
  template <typename Node, typename... Branches, template <typename> class Predicate>
  struct FindImpl<Tree<Node, Branches...>, Predicate> { using Result = Find<Flatten<Tree<Node, Branches...>>, Predicate>; };
  
  template <typename Tree> struct HeightImpl;
  template <typename Tree> using Height = typename HeightImpl<Tree>::Result;
  template <typename Node, typename... Branches>
  struct HeightImpl<Tree<Node, Branches...>> { using Result = Increment<Max<Map<List<Branches...>, Height>>>; };
  template <typename Node> struct HeightImpl<Tree<Node>> { using Result = Int<0>; };
  
  template <typename Branch, typename Type> using Depth = Length<Ancestors<Branch, Type>>;
  
  template <typename Tree, typename Type1, typename Type2>
  struct DistanceImpl {
    using Branch = CommonBranch<Tree, Type1, Type2>;
    using Result = Plus<Depth<Branch, Type1>, Depth<Branch, Type2>>;
  };
  template <typename Tree, typename Type1, typename Type2> using Distance = typename DistanceImpl<Tree, Type1, Type2>::Result;
  
  template <typename Tree, typename Type> struct AppendImpl;
  template <typename Tree, typename Type> using Append = typename AppendImpl<Tree, Type>::Result;
  template <typename Node, typename... Branches, typename Type>
  struct AppendImpl<Tree<Node, Branches...>, Type> { using Result = Tree<Node, Branches..., Tree<Type>>; };
  
  template <typename Tree, typename Type> struct PrependImpl;
  template <typename Tree, typename Type> using Prepend = typename PrependImpl<Tree, Type>::Result;
  template <typename Node, typename... Branches, typename Type>
  struct PrependImpl<Type, Tree<Node, Branches...>> { using Result = Tree<Node, Tree<Type>, Branches...>; };
}

#endif
