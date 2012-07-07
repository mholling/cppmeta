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
  
  template <typename, typename> struct Children;
  template <typename Type, typename Node, typename... Branches>
  struct Children<Type, Tree<Node, Branches...>> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    typedef typename Find<List<Branches...>, ContainsType>::Result ContainingBranch;
    typedef typename Children<Type, ContainingBranch>::Result Result;
  };
  template <typename Type, typename... Branches>
  struct Children<Type, Tree<Type, Branches...>> {
    typedef typename Map<List<Branches...>, Root>::Result Result;
  };
  
  template <typename, typename, typename> struct Lineage;
  template <typename Type, typename IncludeSelf, typename Node, typename... Branches>
  struct Lineage<Type, Tree<Node, Branches...>, IncludeSelf> {
    static_assert(Contains<Tree<Node, Branches...>, Type>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType = Contains<Branch, Type>;
    typedef typename Find<List<Branches...>, ContainsType>::Result ContainingBranch;
    typedef typename Append<typename Lineage<Type, ContainingBranch, IncludeSelf>::Result, Node>::Result Result;
  };
  template <typename Type, typename IncludeSelf, typename... Branches>
  struct Lineage<Type, Tree<Type, Branches...>, IncludeSelf> { typedef typename If<IncludeSelf, List<Type>, List<>>::Result Result; };
  
  template <typename Type, typename Branch> using Ancestors = Lineage<Type, Branch, Bool<false>>;
  template <typename Type, typename Branch> using SelfAndAncestors = Lineage<Type, Branch, Bool<true>>;
  
  template <typename Type, typename Branch>
  struct Parent {
    typedef typename Ancestors<Type, Branch>::Result AncestorList;
    static_assert(Any<AncestorList>::Result::value, "specified type is root of tree and has no parent");
    typedef typename First<AncestorList>::Result Result;
  };
  
  template <typename, typename, typename> struct CommonBranch;
  template <typename Type1, typename Type2, typename Node, typename... Branches>
  struct CommonBranch<Type1, Type2, Tree<Node, Branches...>> {
    static_assert(Contains<Tree<Node, Branches...>, Type1>::Result::value, "tree does not contain specified type");
    static_assert(Contains<Tree<Node, Branches...>, Type2>::Result::value, "tree does not contain specified type");
    template <typename Branch> using ContainsType1 = Contains<Branch, Type1>;
    template <typename Branch> using ContainsType2 = Contains<Branch, Type2>;
    typedef typename Find<List<Branches...>, ContainsType1>::Result BranchContainingType1;
    typedef typename Find<List<Branches...>, ContainsType2>::Result BranchContainingType2;
    typedef typename If<
      typename Same<BranchContainingType1, BranchContainingType2>::Result,
      CommonBranch<Type1, Type2, BranchContainingType1>,
      Tree<Node, Branches...>
    >::Result Result;
  };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Type1, Type2, Tree<Type1, Branches...>> { typedef Tree<Type1, Branches...> Result; };
  template <typename Type1, typename Type2, typename... Branches>
  struct CommonBranch<Type1, Type2, Tree<Type2, Branches...>> { typedef Tree<Type2, Branches...> Result; };
  template <typename Type, typename... Branches>
  struct CommonBranch<Type, Type, Tree<Type, Branches...>> { typedef Tree<Type, Branches...> Result; };
  
  template <typename Type1, typename Type2, typename Branch> using CommonAncestor = Root<typename CommonBranch<Type1, Type2, Branch>::Result>;
  
  template <typename> struct Flatten;
  template <typename Node, typename... Branches>
  struct Flatten<Tree<Node, Branches...>> {
    typedef typename Prepend<Node, typename Flatten<typename Map<List<Branches...>, Meta::Flatten>::Result>::Result>::Result Result;
  };
  
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
  
  // TODO: templates for traversing up & down a list
}
