namespace CppMeta {
  template <typename Node, typename... Branches> struct Tree;
  
  template <typename> struct IsTree { using Result = Bool<false>; };
  template <typename Node, typename... Branches> struct IsTree<Tree<Node, Branches...>> { using Result = Bool<true>; };
  
  template <typename> struct IsLeaf;
  template <typename Node, typename...Branches>
  struct IsLeaf<Tree<Node, Branches...>> { using Result = Bool<false>; };
  template <typename Node>
  struct IsLeaf<Tree<Node>> { using Result = Bool<true>; };
  
  template <typename> struct Root;
  template <typename Node, typename... Branches>
  struct Root<Tree<Node, Branches...>> { using Result = Node; };
  
  template <typename, typename> struct Contains;
  template <typename Type, typename Node, typename... Branches>
  struct Contains<Tree<Node, Branches...>, Type> {
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using Result = typename Any<typename Select<List<Branches...>, ContainsType>::Result>::Result;
  };
  template <typename Node, typename... Branches>
  struct Contains<Tree<Node, Branches...>, Node> { using Result = Bool<true>; };
  
  template <typename, typename> struct FindBranch;
  template <typename Type, typename Node, typename... Branches>
  struct FindBranch<Tree<Node, Branches...>, Type> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using ContainingBranch = typename Find<List<Branches...>, ContainsType>::Result;
    using Result = typename FindBranch<ContainingBranch, Type>::Result;
  };
  template <typename Type, typename... Branches>
  struct FindBranch<Tree<Type, Branches...>, Type> { using Result = Tree<Type, Branches...>; };
  
  template <typename, typename> struct Children;
  template <typename Type, typename Node, typename... Branches>
  struct Children<Tree<Node, Branches...>, Type> {
    using Result = typename Children<typename FindBranch<Tree<Node, Branches...>, Type>::Result, Type>::Result;
  };
  template <typename Type, typename... Branches>
  struct Children<Tree<Type, Branches...>, Type> {
    using Result = typename Map<List<Branches...>, Root>::Result;
  };
  
  template <typename, typename, typename> struct Lineage;
  template <typename Type, typename IncludeSelf, typename Node, typename... Branches>
  struct Lineage<Tree<Node, Branches...>, Type, IncludeSelf> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    using ContainingBranch = typename Find<List<Branches...>, ContainsType>::Result;
    using Result = typename Append<typename Lineage<ContainingBranch, Type, IncludeSelf>::Result, Node>::Result;
  };
  template <typename Type, typename IncludeSelf, typename... Branches>
  struct Lineage<Tree<Type, Branches...>, Type, IncludeSelf> { using Result = typename If<IncludeSelf, List<Type>, List<>>::Result; };
  
  template <typename Branch, typename Type> using Ancestors = Lineage<Branch, Type, Bool<false>>;
  template <typename Branch, typename Type> using SelfAndAncestors = Lineage<Branch, Type, Bool<true>>;
  
  template <typename Branch, typename Type>
  struct Parent {
    using AncestorList = typename Ancestors<Branch, Type>::Result;
    static_assert(Any<AncestorList>::Result::value, "specified type is root of tree and has no parent");
    using Result = typename First<AncestorList>::Result;
  };
  
  template <typename, typename, typename> struct CommonBranch;
  template <typename Type1, typename Type2, typename Node, typename... Branches>
  struct CommonBranch<Tree<Node, Branches...>, Type1, Type2> {
    static_assert(Contains<Tree<Node, Branches...>, Type1>::Result::value, "tree does not contain specified type");
    static_assert(Contains<Tree<Node, Branches...>, Type2>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType1 = Contains<Branch, Type1>;
    template <typename Branch> using ContainsType2 = Contains<Branch, Type2>;
    using BranchContainingType1 = typename Find<List<Branches...>, ContainsType1>::Result;
    using BranchContainingType2 = typename Find<List<Branches...>, ContainsType2>::Result;
    using Result = typename If<
      typename Same<BranchContainingType1, BranchContainingType2>::Result,
      CommonBranch<BranchContainingType1, Type1, Type2>,
      Tree<Node, Branches...>
    >::Result;
  };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Tree<Type1, Branches...>, Type1, Type2> { using Result = Tree<Type1, Branches...>; };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Tree<Type2, Branches...>, Type1, Type2> { using Result = Tree<Type2, Branches...>; };
  template <typename Type, typename... Branches>
  struct CommonBranch<Tree<Type, Branches...>, Type, Type> { using Result = Tree<Type, Branches...>; };
  
  template <typename Branch, typename Type1, typename Type2> using CommonAncestor = Root<typename CommonBranch<Branch, Type1, Type2>::Result>;
  
  template <typename> struct Flatten;
  template <typename Node, typename... Branches>
  struct Flatten<Tree<Node, Branches...>> {
    using Result = typename Prepend<Node, typename Flatten<typename Map<List<Branches...>, CppMeta::Flatten>::Result>::Result>::Result;
  };
  
  template <typename, typename> struct Index;
  template <typename Type, typename Node, typename... Branches>
  struct Index<Tree<Node, Branches...>, Type> {
    using Result = typename Index<typename Flatten<Tree<Node, Branches...>>::Result, Type>::Result;
  };
  
  template <typename> struct Leaves;
  template <typename Node, typename... Branches>
  struct Leaves<Tree<Node, Branches...>> {
    using Result = typename Inject<typename Map<List<Branches...>, CppMeta::Leaves>::Result, Concat, List<>>::Result;
  };
  template <typename Node>
  struct Leaves<Tree<Node>> { using Result = List<Node>; };
  
  template <typename, template <typename> class> struct Find;
  template <typename Node, typename... Branches, template <typename> class Predicate>
  struct Find<Tree<Node, Branches...>, Predicate> {
    using Result = typename Find<typename Flatten<Tree<Node, Branches...>>::Result, Predicate>::Result;
  };
  
  template <typename> struct Height;
  template <typename Node, typename... Branches>
  struct Height<Tree<Node, Branches...>> {
    using BranchHeights = typename Map<List<Branches...>, CppMeta::Height>::Result;
    using MaxBranchHeight = typename Max<BranchHeights>::Result;
    using Result = typename Increment<MaxBranchHeight>::Result; 
  };
  template <typename Node> struct Height<Tree<Node>> { using Result = Int<0>; };
  
  template <typename Branch, typename Type> using Depth = Length<typename Ancestors<Branch, Type>::Result>;
  
  template <typename Tree, typename Type1, typename Type2>
  struct Distance {
    using Branch = typename CommonBranch<Tree, Type1, Type2>::Result;
    using Result = typename Plus<typename Depth<Branch, Type1>::Result, typename Depth<Branch, Type2>::Result>::Result;
  };
}
