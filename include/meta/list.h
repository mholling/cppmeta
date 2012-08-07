#ifndef CPPMETA_META_LIST_H
#define CPPMETA_META_LIST_H

#include "meta.h"

namespace CppMeta {
  template <typename...> struct List;
  
  template <typename Type> struct IsListImpl { using Result = Bool<false>; };
  template <typename... Elements> struct IsListImpl<List<Elements...>> { using Result = Bool<true>; };
  template <typename Type> using IsList = typename IsListImpl<Type>::Result;
  
  template <typename List, typename Type> struct AppendImpl;
  template <typename Type, typename... Elements>
  struct AppendImpl<List<Elements...>, Type> { using Result = List<Elements..., Type>; };
  template <typename List, typename Type> using Append = typename AppendImpl<List, Type>::Result;
  
  template <typename Type, typename List> struct PrependImpl;
  template <typename Type, typename... Elements>
  struct PrependImpl<Type, List<Elements...>> { using Result = List<Type, Elements...>; };
  template <typename Type, typename List> using Prepend = typename PrependImpl<Type, List>::Result;
  
  template <typename List1, typename List2> struct ConcatImpl;
  template <typename... Elements1, typename... Elements2>
  struct ConcatImpl<List<Elements1...>, List<Elements2...>> { using Result = List<Elements1..., Elements2...>; };
  template <typename List1, typename List2> using Concat = typename ConcatImpl<List1, List2>::Result;
  
  template <typename List> struct ReverseImpl;
  template <typename List> using Reverse = typename ReverseImpl<List>::Result;
  template <typename Type, typename... Elements>
  struct ReverseImpl<List<Type, Elements...>> { using Result = Append<Reverse<List<Elements...>>, Type>; };
  template <> struct ReverseImpl<List<>> { using Result = List<>; };
  
  template <typename List, template <typename, typename> class Function, typename Memo> struct InjectImpl;
  template <typename List, template <typename, typename> class Function, typename Memo> using Inject = typename InjectImpl<List, Function, Memo>::Result;
  template <typename Head, typename... Tail, typename Memo, template <typename, typename> class Function>
  struct InjectImpl<List<Head, Tail...>, Function, Memo> { using Result = Inject<List<Tail...>, Function, Function<Memo, Head>>; };
  template <typename Memo, template <typename, typename> class Function>
  struct InjectImpl<List<>, Function, Memo> { using Result = Memo; };
  
  template <typename List, template <typename, typename> class Function> struct InjectFirstImpl;
  template <typename List, template <typename, typename> class Function> using InjectFirst = typename InjectFirstImpl<List, Function>::Result;
  template <typename Type, template <typename, typename> class Function, typename... Elements>
  struct InjectFirstImpl<List<Type, Elements...>, Function> { using Result = Inject<List<Elements...>, Function, Type>; };
  
  template <typename List>
  struct LengthImpl {
    template <typename Memo, typename Type> using Counter = Increment<Memo>;
    using Result = Inject<List, Counter, Int<0>>;
  };
  template <typename List> using Length = typename LengthImpl<List>::Result;
  
  template <typename L, template <typename> class Function>
  struct MapImpl {
    template <typename Memo, typename Type> using Mapper = Append<Memo, Function<Type>>;
    using Result = Inject<L, Mapper, List<>>;
  };
  template <typename List, template <typename> class Function> using Map = typename MapImpl<List, Function>::Result;
  
  template <typename L, template <typename, typename> class Function>
  struct MapWithIndexImpl {
    template <typename Memo, typename Type> using Mapper = Append<Memo, Function<Type, Length<Memo>>>;
    using Result = Inject<L, Mapper, List<>>;
  };
  template <typename List, template <typename, typename> class Function> using MapWithIndex = typename MapWithIndexImpl<List, Function>::Result;
  
  template <typename L, template <typename> class Predicate>
  struct SelectImpl {
    template <typename Memo, typename Type> using Selector = If<Predicate<Type>, AppendImpl<Memo, Type>, Memo>;
    using Result = Inject<L, Selector, List<>>;
  };
  template <typename List, template <typename> class Predicate> using Select = typename SelectImpl<List, Predicate>::Result;
  
  template <typename L, template <typename> class Predicate>
  struct RejectImpl {
    template <typename Memo, typename Type> using Rejector = If<Predicate<Type>, Memo, AppendImpl<Memo, Type>>;
    using Result = Inject<L, Rejector, List<>>;
  };
  template <typename List, template <typename> class Predicate> using Reject = typename RejectImpl<List, Predicate>::Result;
  
  template <typename List, template <typename> class Predicate> struct FindImpl;
  template <typename List, template <typename> class Predicate> using Find = typename FindImpl<List, Predicate>::Result;
  template <typename Head, typename... Tail, template <typename> class Predicate>
  struct FindImpl<List<Head, Tail...>, Predicate> { using Result = If<Predicate<Head>, Head, FindImpl<List<Tail...>, Predicate>>; };
  
  template <typename List> using Any = GreaterThan<Length<List>, Int<0>>;
  template <typename List> using Many = GreaterThan<Length<List>, Int<1>>;
  template <typename List> using Empty = Same<Length<List>, Int<0>>;
  
  template <typename List, typename Type> struct ContainsImpl;
  template <typename List, typename Type> using Contains = typename ContainsImpl<List, Type>::Result;
  template <typename Head, typename... Tail, typename Type>
  struct ContainsImpl<List<Head, Tail...>, Type> { using Result = Contains<List<Tail...>, Type>; };
  template <typename... Tail, typename Type>
  struct ContainsImpl<List<Type, Tail...>, Type> { using Result = Bool<true>; };
  template <typename Type>
  struct ContainsImpl<List<>, Type> { using Result = Bool<false>; };
  
  template <typename L>
  struct UniqueImpl {
    template <typename Memo, typename Type> using AppendIfNew = If<Contains<Memo, Type>, Memo, AppendImpl<Memo, Type>>;
    using Result = Inject<L, AppendIfNew, List<>>;
  };
  template <typename List> using Unique = typename UniqueImpl<List>::Result;
  
  template <typename List, typename Type> struct IndexImpl;
  template <typename List, typename Type> using Index = typename IndexImpl<List, Type>::Result;
  template <typename Head, typename... Tail, typename Type>
  struct IndexImpl<List<Head, Tail...>, Type> { using Result = Increment<Index<List<Tail...>, Type>>; };
  template <typename... Tail, typename Type>
  struct IndexImpl<List<Type, Tail...>, Type> { using Result = Int<0>; };
  
  template <typename List> struct FirstImpl;
  template <typename Head, typename... Tail> struct FirstImpl<List<Head, Tail...>> { using Result = Head; };
  template <typename List> using First = typename FirstImpl<List>::Result;
  
  template <typename List> struct LastImpl;
  template <typename List> using Last = typename LastImpl<List>::Result;
  template <typename Head, typename... Tail> struct LastImpl<List<Head, Tail...>> { using Result = Last<List<Tail...>>; };
  template <typename Head> struct LastImpl<List<Head>> { using Result = Head; };
  
  template <typename List, typename Index> struct AtImpl;
  template <typename List, typename Index> using At = typename AtImpl<List, Index>::Result;
  template <typename Head, typename... Tail, typename Index>
  struct AtImpl<List<Head, Tail...>, Index> { using Result = At<List<Tail...>, Decrement<Index>>; };
  template <typename Head, typename... Tail>
  struct AtImpl<List<Head, Tail...>, Int<0>> { using Result = Head; };
  
  template <typename Type> struct FlattenImpl;
  template <typename Type> using Flatten = typename FlattenImpl<Type>::Result;
  template <typename Type> struct FlattenImpl { using Result = List<Type>; };
  template <typename Head, typename... Tail>
  struct FlattenImpl<List<Head, Tail...>> { using Result = Concat<Flatten<Head>, Flatten<List<Tail...>>>; };
  template <>
  struct FlattenImpl<List<>> { using Result = List<>; };
  
  template <typename List, typename Index> struct BeforeIndexImpl;
  template <typename List, typename Index> using BeforeIndex = typename BeforeIndexImpl<List, Index>::Result;
  template <typename Index, typename Head, typename... Tail>
  struct BeforeIndexImpl<List<Head, Tail...>, Index> { using Result = Prepend<Head, BeforeIndex<List<Tail...>, Decrement<Index>>>; };
  template <typename Head, typename... Tail>
  struct BeforeIndexImpl<List<Head, Tail...>, Int<0>> { using Result = List<>; };
  template <>
  struct BeforeIndexImpl<List<>, Int<0>> { using Result = List<>; };
  
  template <typename List, typename Index> struct AfterIndexImpl;
  template <typename List, typename Index> using AfterIndex = typename AfterIndexImpl<List, Index>::Result;
  template <typename Head, typename... Tail, typename Index>
  struct AfterIndexImpl<List<Head, Tail...>, Index> { using Result = AfterIndex<List<Tail...>, Decrement<Index>>; };
  template <typename Head, typename... Tail>
  struct AfterIndexImpl<List<Head, Tail...>, Int<-1>> { using Result = List<Head, Tail...>; };
  template <>
  struct AfterIndexImpl<List<>, Int<-1>> { using Result = List<>; };
  
  template <typename List, typename Type> struct BeforeImpl;
  template <typename List, typename Type> using Before = typename BeforeImpl<List, Type>::Result;
  template <typename Head, typename... Tail, typename Type>
  struct BeforeImpl<List<Head, Tail...>, Type> { using Result = Prepend<Head, Before<List<Tail...>, Type>>; };
  template <typename Type, typename... Tail>
  struct BeforeImpl<List<Type, Tail...>, Type> { using Result = List<>; };
  
  template <typename List, typename Type> struct AfterImpl;
  template <typename List, typename Type> using After = typename AfterImpl<List, Type>::Result;
  template <typename Head, typename... Tail, typename Type>
  struct AfterImpl<List<Head, Tail...>, Type> { using Result = After<List<Tail...>, Type>; };
  template <typename Type, typename... Tail>
  struct AfterImpl<List<Type, Tail...>, Type> { using Result = List<Tail...>; };
  
  template <typename List, template <typename, typename> class LessThan> struct SortImpl;
  template <typename List, template <typename, typename> class LessThan> using Sort = typename SortImpl<List, LessThan>::Result;
  template <typename L, template <typename, typename> class LessThan>
  struct SortImpl {
    template <typename List1, typename List2> struct MergeImpl;
    template <typename Head1, typename... Tail1, typename Head2, typename... Tail2>
    struct MergeImpl<List<Head1, Tail1...>, List<Head2, Tail2...>> {
      using Lt = LessThan<Head1, Head2>;
      using First = If<Lt, Head1, Head2>;
      using Rest = If<Lt, MergeImpl<List<Tail1...>, List<Head2, Tail2...>>, MergeImpl<List<Head1, Tail1...>, List<Tail2...>>>;
      using Result = Prepend<First, Rest>;
    };
    template <typename... Elements> struct MergeImpl<List<Elements...>, List<>> { using Result = List<Elements...>; };
    template <typename... Elements> struct MergeImpl<List<>, List<Elements...>> { using Result = List<Elements...>; };
    template <typename List1, typename List2> using Merge = typename MergeImpl<List1, List2>::Result;
    
    using Left = BeforeIndex<L, Int<(Length<L>::value+1)/2>>;
    using Right = AfterIndex<L, Int<(Length<L>::value-1)/2>>;
    using Result = Merge<Sort<Left, LessThan>, Sort<Right, LessThan>>;
  };
  template <template <typename, typename> class LessThan>
  struct SortImpl<List<>, LessThan> { using Result = List<>; };
  template <typename Head, template <typename, typename> class LessThan>
  struct SortImpl<List<Head>, LessThan> { using Result = List<Head>; };
  
  template <typename List> using Max = InjectFirst<List, GreaterOf>;
  template <typename List> using Min = InjectFirst<List, LesserOf>;
  
  template <typename First, typename Last> struct SequenceImpl;
  template <typename First, typename Last> using Sequence = typename SequenceImpl<First, Last>::Result;
  template <typename First, typename Last>
  struct SequenceImpl { using Result = Append<Sequence<First, Decrement<Last>>, Last>; };
  template <typename First>
  struct SequenceImpl<First, First> { using Result = List<First>; };
  
  template <typename List, template <typename> class Function = Self> struct Each;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Each<List<Head, Tail...>, Function> {
    template <typename... Args>
    void operator ()(Args... args) { Function<Head>()(args...); Each<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Each<List<>, Function> {
    template <typename... Args>
    void operator ()(Args... args) { }
  };
  
  template <typename List, template <typename> class Function = Self> struct While;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct While<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return Function<Head>()(args...) && While<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct While<List<>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return true; }
  };
  
  template <typename List, template <typename> class Function = Self> struct Until;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Until<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return Function<Head>()(args...) || Until<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Until<List<>, Function> {
    template <typename... Args>
    bool operator ()(Args... args) { return false; }
  };
}

#endif
