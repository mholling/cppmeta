namespace Meta {
  template <typename Node, typename... Branches> struct Tree;
  
  template <typename> struct IsTree { typedef Bool<false> Result; };
  template <typename Node, typename... Branches> struct IsTree<Tree<Node, Branches...>> { typedef Bool<true> Result; };
  
  template <typename> struct IsLeaf;
  template <typename Node, typename...Branches>
  struct IsLeaf<Tree<Node, Branches...>> { typedef Bool<false> Result; };
  template <typename Node>
  struct IsLeaf<Tree<Node>> { typedef Bool<true> Result; };
  
  template <typename> struct Root;
  template <typename Node, typename... Branches>
  struct Root<Tree<Node, Branches...>> { typedef Node Result; };
  
  template <typename, typename> struct Contains;
  template <typename Type, typename Node, typename... Branches>
  struct Contains<Tree<Node, Branches...>, Type> {
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    typedef typename Any<typename Select<List<Branches...>, ContainsType>::Result>::Result Result;
  };
  template <typename Node, typename... Branches>
  struct Contains<Tree<Node, Branches...>, Node> { typedef Bool<true> Result; };
  
  template <typename, typename> struct FindBranch;
  template <typename Type, typename Node, typename... Branches>
  struct FindBranch<Tree<Node, Branches...>, Type> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    typedef typename Find<List<Branches...>, ContainsType>::Result ContainingBranch;
    typedef typename FindBranch<ContainingBranch, Type>::Result Result;
  };
  template <typename Type, typename... Branches>
  struct FindBranch<Tree<Type, Branches...>, Type> { typedef Tree<Type, Branches...> Result; };
  
  template <typename, typename> struct Children;
  template <typename Type, typename Node, typename... Branches>
  struct Children<Tree<Node, Branches...>, Type> {
    typedef typename Children<typename FindBranch<Tree<Node, Branches...>, Type>::Result, Type>::Result Result;
  };
  template <typename Type, typename... Branches>
  struct Children<Tree<Type, Branches...>, Type> {
    typedef typename Map<List<Branches...>, Root>::Result Result;
  };
  
  template <typename, typename, typename> struct Lineage;
  template <typename Type, typename IncludeSelf, typename Node, typename... Branches>
  struct Lineage<Tree<Node, Branches...>, Type, IncludeSelf> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    typedef typename Find<List<Branches...>, ContainsType>::Result ContainingBranch;
    typedef typename Append<typename Lineage<ContainingBranch, Type, IncludeSelf>::Result, Node>::Result Result;
  };
  template <typename Type, typename IncludeSelf, typename... Branches>
  struct Lineage<Tree<Type, Branches...>, Type, IncludeSelf> { typedef typename If<IncludeSelf, List<Type>, List<>>::Result Result; };
  
  template <typename Branch, typename Type> using Ancestors = Lineage<Branch, Type, Bool<false>>;
  template <typename Branch, typename Type> using SelfAndAncestors = Lineage<Branch, Type, Bool<true>>;
  
  template <typename Branch, typename Type>
  struct Parent {
    typedef typename Ancestors<Branch, Type>::Result AncestorList;
    static_assert(Any<AncestorList>::Result::value, "specified type is root of tree and has no parent");
    typedef typename First<AncestorList>::Result Result;
  };
  
  template <typename, typename, typename> struct CommonBranch;
  template <typename Type1, typename Type2, typename Node, typename... Branches>
  struct CommonBranch<Tree<Node, Branches...>, Type1, Type2> {
    static_assert(Contains<Tree<Node, Branches...>, Type1>::Result::value, "tree does not contain specified type");
    static_assert(Contains<Tree<Node, Branches...>, Type2>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType1 = Contains<Branch, Type1>;
    template <typename Branch> using ContainsType2 = Contains<Branch, Type2>;
    typedef typename Find<List<Branches...>, ContainsType1>::Result BranchContainingType1;
    typedef typename Find<List<Branches...>, ContainsType2>::Result BranchContainingType2;
    typedef typename If<
      typename Same<BranchContainingType1, BranchContainingType2>::Result,
      CommonBranch<BranchContainingType1, Type1, Type2>,
      Tree<Node, Branches...>
    >::Result Result;
  };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Tree<Type1, Branches...>, Type1, Type2> { typedef Tree<Type1, Branches...> Result; };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Tree<Type2, Branches...>, Type1, Type2> { typedef Tree<Type2, Branches...> Result; };
  template <typename Type, typename... Branches>
  struct CommonBranch<Tree<Type, Branches...>, Type, Type> { typedef Tree<Type, Branches...> Result; };
  
  template <typename Branch, typename Type1, typename Type2> using CommonAncestor = Root<typename CommonBranch<Branch, Type1, Type2>::Result>;
  
  template <typename> struct Flatten;
  template <typename Node, typename... Branches>
  struct Flatten<Tree<Node, Branches...>> {
    typedef typename Prepend<Node, typename Flatten<typename Map<List<Branches...>, Meta::Flatten>::Result>::Result>::Result Result;
  };
  
  template <typename, typename> struct Index;
  template <typename Type, typename Node, typename... Branches>
  struct Index<Tree<Node, Branches...>, Type> {
    typedef typename Index<typename Flatten<Tree<Node, Branches...>>::Result, Type>::Result Result;
  };
  
  template <typename> struct Leaves;
  template <typename Node, typename... Branches>
  struct Leaves<Tree<Node, Branches...>> {
    typedef typename Inject<typename Map<List<Branches...>, Meta::Leaves>::Result, Concat, List<>>::Result Result;
  };
  template <typename Node>
  struct Leaves<Tree<Node>> { typedef List<Node> Result; };
  
  template <typename, template <typename> class> struct Find;
  template <typename Node, typename... Branches, template <typename> class Predicate>
  struct Find<Tree<Node, Branches...>, Predicate> {
    typedef typename Find<typename Flatten<Tree<Node, Branches...>>::Result, Predicate>::Result Result;
  };
  
  template <typename> struct Height;
  template <typename Node, typename... Branches>
  struct Height<Tree<Node, Branches...>> {
    typedef typename Map<List<Branches...>, Meta::Height>::Result BranchHeights;
    typedef typename Max<BranchHeights>::Result MaxBranchHeight;
    typedef typename Increment<MaxBranchHeight>::Result Result; 
  };
  template <typename Node> struct Height<Tree<Node>> { typedef Int<0> Result; };
  
  template <typename Branch, typename Type> using Depth = Length<typename Ancestors<Branch, Type>::Result>;
  
  template <typename Tree, typename Type1, typename Type2>
  struct Distance {
    typedef typename CommonBranch<Tree, Type1, Type2>::Result Branch;
    typedef typename Plus<typename Depth<Branch, Type1>::Result, typename Depth<Branch, Type2>::Result>::Result Result;
  };
  
  template <typename> struct Each;
  template <typename Node, typename... Branches>
  struct Each<Tree<Node, Branches...>> {
    typedef Each Result;
    template <typename... Args>
    void operator ()(Args... args) { Node()(args...); Meta::Each<typename Map<List<Branches...>, Meta::Each>::Result>()(args...); }
  };
  
  template <typename Branch, typename Type> using AscendingEach = Each<typename SelfAndAncestors<Branch, Type>::Result>;
  template <typename Branch, typename Type> using DescendingEach = Each<typename Reverse<typename SelfAndAncestors<Branch, Type>::Result>::Result>;
}
