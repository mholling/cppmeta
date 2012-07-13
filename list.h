namespace Meta {
  template <typename...> struct List;
  
  template <typename> struct IsList { typedef Bool<false> Result; };
  template <typename... Elements> struct IsList<List<Elements...>> { typedef Bool<true> Result; };
  
  template <typename, typename> struct Append;
  template <typename Type, typename... Elements>
  struct Append<List<Elements...>, Type> { typedef List<Elements..., Type> Result; };
  
  template <typename, typename> struct Prepend;
  template <typename Type, typename... Elements>
  struct Prepend<Type, List<Elements...>> { typedef List<Type, Elements...> Result; };
  
  template <typename, typename> struct Concat;
  template <typename... Elements1, typename... Elements2>
  struct Concat<List<Elements1...>, List<Elements2...>> { typedef List<Elements1..., Elements2...> Result; };
  
  template <typename> struct Reverse;
  template <typename Type, typename... Elements>
  struct Reverse<List<Type, Elements...>> { typedef typename Append<typename Reverse<List<Elements...>>::Result, Type>::Result Result; };
  template <> struct Reverse<List<>> { typedef List<> Result; };
  
  template <typename L, template <typename, typename> class Function, typename Memo> struct Inject;
  template <typename Head, typename... Tail, typename Memo, template <typename, typename> class Function>
  struct Inject<List<Head, Tail...>, Function, Memo> {
    typedef typename Inject<List<Tail...>, Function, typename Function<Memo, Head>::Result>::Result Result;
  };
  template <typename Memo, template <typename, typename> class Function>
  struct Inject<List<>, Function, Memo> { typedef Memo Result; };
  
  template <typename L, template <typename, typename> class Function> struct InjectFirst;
  template <typename Type, template <typename, typename> class Function, typename... Elements>
  struct InjectFirst<List<Type, Elements...>, Function> {
    typedef typename Inject<List<Elements...>, Function, Type>::Result Result;
  };
  
  template <typename L>
  struct Length {
    template <typename Memo, typename Type> using Counter = Increment<Memo>;
    typedef typename Inject<L, Counter, Int<0>>::Result Result;
  };
  
  template <typename L, template <typename> class Function>
  struct Map {
    template <typename Memo, typename Type> using Mapper = Append<Memo, typename Function<Type>::Result>;
    typedef typename Inject<L, Mapper, List<>>::Result Result;
  };
  
  template <typename L, template <typename, typename> class Function>
  struct MapWithIndex {
    template <typename Memo, typename Type> using Mapper = Append<Memo, typename Function<Type, typename Length<Memo>::Result>::Result>;
    typedef typename Inject<L, Mapper, List<>>::Result Result;
  };
  
  template <typename L, template <typename> class Predicate>
  struct Select {
    template <typename Memo, typename Type> using Selector = If<typename Predicate<Type>::Result, Append<Memo, Type>, Memo>;
    typedef typename Inject<L, Selector, List<>>::Result Result;
  };
  
  template <typename L, template <typename> class Predicate>
  struct Reject {
    template <typename Memo, typename Type> using Rejector = If<typename Predicate<Type>::Result, Memo, Append<Memo, Type>>;
    typedef typename Inject<L, Rejector, List<>>::Result Result;
  };
  
  template <typename L, template <typename> class Predicate> struct Find;
  template <typename Head, typename... Tail, template <typename> class Predicate>
  struct Find<List<Head, Tail...>, Predicate> {
    typedef typename If<typename Predicate<Head>::Result, Head, Find<List<Tail...>, Predicate>>::Result Result;
  };
  
  template <typename L> struct Any;
  template <typename... Elements> struct Any<List<Elements...>> { typedef Bool<true> Result; };
  template <> struct Any<List<>> { typedef Bool<false> Result; };
  
  template <typename L> struct Empty;
  template <typename... Elements> struct Empty<List<Elements...>> { typedef Bool<false> Result; };
  template <> struct Empty<List<>> { typedef Bool<true> Result; };
  
  template <typename L, typename Type> struct Contains;
  template <typename... Tail, typename Head, typename Type>
  struct Contains<List<Head, Tail...>, Type> { typedef typename Contains<List<Tail...>, Type>::Result Result; };
  template <typename Type, typename... Tail>
  struct Contains<List<Type, Tail...>, Type> { typedef Bool<true> Result; };
  template <typename Type>
  struct Contains<List<>, Type> { typedef Bool<false> Result; };
  
  template <typename L>
  struct Unique {
    template <typename Memo, typename Type> using Check = If<typename Contains<Memo, Type>::Result, Memo, Append<Memo, Type>>;
    typedef typename Inject<L, Check, List<>>::Result Result;
  };
  
  template <typename L, typename Type> struct Index;
  template <typename Head, typename... Tail, typename Type>
  struct Index<List<Head, Tail...>, Type> { typedef typename Increment<typename Index<List<Tail...>, Type>::Result>::Result Result; };
  template <typename... Tail, typename Type>
  struct Index<List<Type, Tail...>, Type> { typedef Int<0> Result; };
  
  template <typename L> struct First;
  template <typename Head, typename... Tail> struct First<List<Head, Tail...>> { typedef Head Result; };
  
  template <typename L> struct Last;
  template <typename Head, typename... Tail> struct Last<List<Head, Tail...>> { typedef typename Last<List<Tail...>>::Result Result; };
  template <typename Head> struct Last<List<Head>> { typedef Head Result; };
  
  template <typename L, typename Index> struct At;
  template <typename Head, typename... Tail, typename Index>
  struct At<List<Head, Tail...>, Index> { typedef typename At<List<Tail...>, typename Decrement<Index>::Result>::Result Result; };
  template <typename Head, typename... Tail>
  struct At<List<Head, Tail...>, Int<0>> { typedef Head Result; };
  
  template <typename Type> struct Flatten { typedef List<Type> Result; };
  template <typename Head, typename... Tail>
  struct Flatten<List<Head, Tail...>> {
    typedef typename Concat<typename Flatten<Head>::Result, typename Flatten<List<Tail...>>::Result>::Result Result; 
  };
  template <>
  struct Flatten<List<>> { typedef List<> Result; };
  
  template <typename L, typename IndexOrElement> struct Before;
  template <typename Index, typename Head, typename... Tail>
  struct Before<List<Head, Tail...>, Index> { typedef typename Prepend<Head, typename Before<List<Tail...>, typename Decrement<Index>::Result>::Result>::Result Result; };
  template <typename Head, typename... Tail>
  struct Before<List<Head, Tail...>, Int<0>> { typedef List<> Result; };
  template <>
  struct Before<List<>, Int<0>> { typedef List<> Result; };
  
  template <typename L, typename Index> struct After;
  template <typename Head, typename... Tail, typename Index>
  struct After<List<Head, Tail...>, Index> { typedef typename After<List<Tail...>, typename Decrement<Index>::Result>::Result Result; };
  template <typename Head, typename... Tail>
  struct After<List<Head, Tail...>, Int<-1>> { typedef List<Head, Tail...> Result; };
  template <>
  struct After<List<>, Int<-1>> { typedef List<> Result; };
  
  template <typename L, typename Type> struct UpTo;
  template <typename Head, typename... Tail, typename Type>
  struct UpTo<List<Head, Tail...>, Type> { typedef typename Prepend<Head, typename UpTo<List<Tail...>, Type>::Result>::Result Result; };
  template <typename Type, typename... Tail>
  struct UpTo<List<Type, Tail...>, Type> { typedef List<> Result; };
  
  template <typename L, template <typename, typename> class LessThan>
  struct Sort {
    template <typename, typename> struct Merge;
    template <typename Head1, typename... Tail1, typename Head2, typename... Tail2>
    struct Merge<List<Head1, Tail1...>, List<Head2, Tail2...>> {
      typedef typename LessThan<Head1, Head2>::Result Lt;
      typedef typename If<Lt, Head1, Head2>::Result First;
      typedef typename If<Lt, Merge<List<Tail1...>, List<Head2, Tail2...>>, Merge<List<Head1, Tail1...>, List<Tail2...>>>::Result Rest;
      typedef typename Prepend<First, Rest>::Result Result;
    };
    template <typename... Elements> struct Merge<List<Elements...>, List<>> { typedef List<Elements...> Result; };
    template <typename... Elements> struct Merge<List<>, List<Elements...>> { typedef List<Elements...> Result; };
    
    typedef typename Before<L, Int<(Length<L>::Result::value+1)/2>>::Result Left;
    typedef typename  After<L, Int<(Length<L>::Result::value-1)/2>>::Result Right;
    typedef typename Merge<typename Sort<Left, LessThan>::Result, typename Sort<Right, LessThan>::Result>::Result Result;
  };
  template <template <typename, typename> class LessThan>
  struct Sort<List<>, LessThan> { typedef List<> Result; };
  template <typename Head, template <typename, typename> class LessThan>
  struct Sort<List<Head>, LessThan> { typedef List<Head> Result; };
  
  template <typename L> using Max = InjectFirst<L, GreaterOf>;
  template <typename L> using Min = InjectFirst<L, LesserOf>;
  
  template <typename L> struct Each;
  template <typename Head, typename... Tail>
  struct Each<List<Head, Tail...>> {
    template <typename... Args>
    void operator ()(Args... args) { Head()(args...); Each<List<Tail...>>()(args...); }
  };
  template <>
  struct Each<List<>> {
    template <typename... Args>
    void operator ()(Args... args) { }
  };
  
  template <typename L> struct While;
  template <typename Head, typename... Tail>
  struct While<List<Head, Tail...>> {
    template <typename... Args>
    bool operator ()(Args... args) { return Head()(args...) && While<List<Tail...>>()(args...); }
  };
  template <>
  struct While<List<>> {
    template <typename... Args>
    bool operator ()(Args... args) { return true; }
  };
  
  template <typename L> struct Until;
  template <typename Head, typename... Tail>
  struct Until<List<Head, Tail...>> {
    template <typename... Args>
    bool operator ()(Args... args) { return Head()(args...) || Until<List<Tail...>>()(args...); }
  };
  template <>
  struct Until<List<>> {
    template <typename... Args>
    bool operator ()(Args... args) { return false; }
  };
}
