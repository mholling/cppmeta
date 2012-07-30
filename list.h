namespace CppMeta {
  template <typename...> struct List;
  
  template <typename> struct IsList { using Result = Bool<false>; };
  template <typename... Elements> struct IsList<List<Elements...>> { using Result = Bool<true>; };
  
  template <typename, typename> struct Append;
  template <typename Type, typename... Elements>
  struct Append<List<Elements...>, Type> { using Result = List<Elements..., Type>; };
  
  template <typename, typename> struct Prepend;
  template <typename Type, typename... Elements>
  struct Prepend<Type, List<Elements...>> { using Result = List<Type, Elements...>; };
  
  template <typename, typename> struct Concat;
  template <typename... Elements1, typename... Elements2>
  struct Concat<List<Elements1...>, List<Elements2...>> { using Result = List<Elements1..., Elements2...>; };
  
  template <typename> struct Reverse;
  template <typename Type, typename... Elements>
  struct Reverse<List<Type, Elements...>> { using Result = typename Append<typename Reverse<List<Elements...>>::Result, Type>::Result; };
  template <> struct Reverse<List<>> { using Result = List<>; };
  
  template <typename L, template <typename, typename> class Function, typename Memo> struct Inject;
  template <typename Head, typename... Tail, typename Memo, template <typename, typename> class Function>
  struct Inject<List<Head, Tail...>, Function, Memo> {
    using Result = typename Inject<List<Tail...>, Function, typename Function<Memo, Head>::Result>::Result;
  };
  template <typename Memo, template <typename, typename> class Function>
  struct Inject<List<>, Function, Memo> { using Result = Memo; };
  
  template <typename L, template <typename, typename> class Function> struct InjectFirst;
  template <typename Type, template <typename, typename> class Function, typename... Elements>
  struct InjectFirst<List<Type, Elements...>, Function> {
    using Result = typename Inject<List<Elements...>, Function, Type>::Result;
  };
  
  template <typename L>
  struct Length {
    template <typename Memo, typename Type> using Counter = Increment<Memo>;
    using Result = typename Inject<L, Counter, Int<0>>::Result;
  };
  
  template <typename L, template <typename> class Function>
  struct Map {
    template <typename Memo, typename Type> using Mapper = Append<Memo, typename Function<Type>::Result>;
    using Result = typename Inject<L, Mapper, List<>>::Result;
  };
  
  template <typename L, template <typename, typename> class Function>
  struct MapWithIndex {
    template <typename Memo, typename Type> using Mapper = Append<Memo, typename Function<Type, typename Length<Memo>::Result>::Result>;
    using Result = typename Inject<L, Mapper, List<>>::Result;
  };
  
  template <typename L, template <typename> class Predicate>
  struct Select {
    template <typename Memo, typename Type> using Selector = If<typename Predicate<Type>::Result, Append<Memo, Type>, Memo>;
    using Result = typename Inject<L, Selector, List<>>::Result;
  };
  
  template <typename L, template <typename> class Predicate>
  struct Reject {
    template <typename Memo, typename Type> using Rejector = If<typename Predicate<Type>::Result, Memo, Append<Memo, Type>>;
    using Result = typename Inject<L, Rejector, List<>>::Result;
  };
  
  template <typename L, template <typename> class Predicate> struct Find;
  template <typename Head, typename... Tail, template <typename> class Predicate>
  struct Find<List<Head, Tail...>, Predicate> {
    using Result = typename If<typename Predicate<Head>::Result, Head, Find<List<Tail...>, Predicate>>::Result;
  };
  
  template <typename L> struct Any;
  template <typename... Elements> struct Any<List<Elements...>> { using Result = Bool<true>; };
  template <> struct Any<List<>> { using Result = Bool<false>; };
  
  template <typename L> struct Empty;
  template <typename... Elements> struct Empty<List<Elements...>> { using Result = Bool<false>; };
  template <> struct Empty<List<>> { using Result = Bool<true>; };
  
  template <typename L, typename Type> struct Contains;
  template <typename... Tail, typename Head, typename Type>
  struct Contains<List<Head, Tail...>, Type> { using Result = typename Contains<List<Tail...>, Type>::Result; };
  template <typename Type, typename... Tail>
  struct Contains<List<Type, Tail...>, Type> { using Result = Bool<true>; };
  template <typename Type>
  struct Contains<List<>, Type> { using Result = Bool<false>; };
  
  template <typename L>
  struct Unique {
    template <typename Memo, typename Type> using Check = If<typename Contains<Memo, Type>::Result, Memo, Append<Memo, Type>>;
    using Result = typename Inject<L, Check, List<>>::Result;
  };
  
  template <typename L, typename Type> struct Index;
  template <typename Head, typename... Tail, typename Type>
  struct Index<List<Head, Tail...>, Type> { using Result = typename Increment<typename Index<List<Tail...>, Type>::Result>::Result; };
  template <typename... Tail, typename Type>
  struct Index<List<Type, Tail...>, Type> { using Result = Int<0>; };
  
  template <typename L> struct First;
  template <typename Head, typename... Tail> struct First<List<Head, Tail...>> { using Result = Head; };
  
  template <typename L> struct Last;
  template <typename Head, typename... Tail> struct Last<List<Head, Tail...>> { using Result = typename Last<List<Tail...>>::Result; };
  template <typename Head> struct Last<List<Head>> { using Result = Head; };
  
  template <typename L, typename Index> struct At;
  template <typename Head, typename... Tail, typename Index>
  struct At<List<Head, Tail...>, Index> { using Result = typename At<List<Tail...>, typename Decrement<Index>::Result>::Result; };
  template <typename Head, typename... Tail>
  struct At<List<Head, Tail...>, Int<0>> { using Result = Head; };
  
  template <typename Type> struct Flatten { using Result = List<Type>; };
  template <typename Head, typename... Tail>
  struct Flatten<List<Head, Tail...>> {
    using Result = typename Concat<typename Flatten<Head>::Result, typename Flatten<List<Tail...>>::Result>::Result; 
  };
  template <>
  struct Flatten<List<>> { using Result = List<>; };
  
  template <typename L, typename IndexOrElement> struct Before;
  template <typename Index, typename Head, typename... Tail>
  struct Before<List<Head, Tail...>, Index> { using Result = typename Prepend<Head, typename Before<List<Tail...>, typename Decrement<Index>::Result>::Result>::Result; };
  template <typename Head, typename... Tail>
  struct Before<List<Head, Tail...>, Int<0>> { using Result = List<>; };
  template <>
  struct Before<List<>, Int<0>> { using Result = List<>; };
  
  template <typename L, typename Index> struct After;
  template <typename Head, typename... Tail, typename Index>
  struct After<List<Head, Tail...>, Index> { using Result = typename After<List<Tail...>, typename Decrement<Index>::Result>::Result; };
  template <typename Head, typename... Tail>
  struct After<List<Head, Tail...>, Int<-1>> { using Result = List<Head, Tail...>; };
  template <>
  struct After<List<>, Int<-1>> { using Result = List<>; };
  
  template <typename L, typename Type> struct UpTo;
  template <typename Head, typename... Tail, typename Type>
  struct UpTo<List<Head, Tail...>, Type> { using Result = typename Prepend<Head, typename UpTo<List<Tail...>, Type>::Result>::Result; };
  template <typename Type, typename... Tail>
  struct UpTo<List<Type, Tail...>, Type> { using Result = List<>; };
  
  template <typename L, template <typename, typename> class LessThan>
  struct Sort {
    template <typename, typename> struct Merge;
    template <typename Head1, typename... Tail1, typename Head2, typename... Tail2>
    struct Merge<List<Head1, Tail1...>, List<Head2, Tail2...>> {
      using Lt = typename LessThan<Head1, Head2>::Result;
      using First = typename If<Lt, Head1, Head2>::Result;
      using Rest = typename If<Lt, Merge<List<Tail1...>, List<Head2, Tail2...>>, Merge<List<Head1, Tail1...>, List<Tail2...>>>::Result;
      using Result = typename Prepend<First, Rest>::Result;
    };
    template <typename... Elements> struct Merge<List<Elements...>, List<>> { using Result = List<Elements...>; };
    template <typename... Elements> struct Merge<List<>, List<Elements...>> { using Result = List<Elements...>; };
    
    using Left = typename Before<L, Int<(Length<L>::Result::value+1)/2>>::Result;
    using Right = typename  After<L, Int<(Length<L>::Result::value-1)/2>>::Result;
    using Result = typename Merge<typename Sort<Left, LessThan>::Result, typename Sort<Right, LessThan>::Result>::Result;
  };
  template <template <typename, typename> class LessThan>
  struct Sort<List<>, LessThan> { using Result = List<>; };
  template <typename Head, template <typename, typename> class LessThan>
  struct Sort<List<Head>, LessThan> { using Result = List<Head>; };
  
  template <typename L> using Max = InjectFirst<L, GreaterOf>;
  template <typename L> using Min = InjectFirst<L, LesserOf>;
  
  template <typename L, template <typename> class Function = Self> struct Each;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Each<List<Head, Tail...>, Function> {
    template <typename... Args>
    void operator ()(Args... args) { typename Function<Head>::Result()(args...); Each<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Each<List<>, Function> {
    template <typename... Args>
    void operator ()(Args... args) { }
  };
  
  template <typename L, template <typename> class Function = Self> struct While;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct While<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return typename Function<Head>::Result()(args...) && While<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct While<List<>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return true; }
  };
  
  template <typename L, template <typename> class Function = Self> struct Until;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Until<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return typename Function<Head>::Result()(args...) || Until<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Until<List<>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return false; }
  };
}
