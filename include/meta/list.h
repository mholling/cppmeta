#ifndef CPPMETA_META_LIST_H
#define CPPMETA_META_LIST_H

#include "meta.h"

namespace CppMeta {
  template <typename...> struct List {};
  
  template <typename Type> struct IsListImpl { using Result = Bool<false>; };
  template <typename... Elements> struct IsListImpl<List<Elements...>> { using Result = Bool<true>; };
  template <typename Type> using IsList = Eval<IsListImpl<Type>>;
  
  template <typename List, typename Type> struct AppendImpl;
  template <typename Type, typename... Elements>
  struct AppendImpl<List<Elements...>, Type> { using Result = List<Elements..., Type>; };
  template <typename List, typename Type> using Append = Eval<AppendImpl<List, Type>>;
  
  template <typename Type, typename List> struct PrependImpl;
  template <typename Type, typename... Elements>
  struct PrependImpl<Type, List<Elements...>> { using Result = List<Type, Elements...>; };
  template <typename Type, typename List> using Prepend = Eval<PrependImpl<Type, List>>;
  
  template <typename List, typename Element, typename Type> struct InsertBeforeImpl;
  template <typename List, typename Element, typename Type> using InsertBefore = Eval<InsertBeforeImpl<List, Element, Type>>;
  template <typename Head, typename... Tail, typename Element, typename Type>
  struct InsertBeforeImpl<List<Head, Tail...>, Element, Type> { using Result = Prepend<Head, InsertBefore<List<Tail...>, Element, Type>>; };
  template <typename Head, typename... Tail, typename Type>
  struct InsertBeforeImpl<List<Head, Tail...>, Head, Type> { using Result = List<Type, Head, Tail...>; };
  
  template <typename List, typename Element, typename Type> struct InsertAfterImpl;
  template <typename List, typename Element, typename Type> using InsertAfter = Eval<InsertAfterImpl<List, Element, Type>>;
  template <typename Head, typename... Tail, typename Element, typename Type>
  struct InsertAfterImpl<List<Head, Tail...>, Element, Type> { using Result = Prepend<Head, InsertAfter<List<Tail...>, Element, Type>>; };
  template <typename Head, typename... Tail, typename Type>
  struct InsertAfterImpl<List<Head, Tail...>, Head, Type> { using Result = List<Head, Type, Tail...>; };
  
  template <typename List, typename Element, typename Replacement> struct ReplaceImpl;
  template <typename List, typename Element, typename Replacement> using Replace = Eval<ReplaceImpl<List, Element, Replacement>>;
  template <typename Head, typename... Tail, typename Element, typename Replacement>
  struct ReplaceImpl<List<Head, Tail...>, Element, Replacement> { using Result = Prepend<Head, Replace<List<Tail...>, Element, Replacement>>; };
  template <typename... Tail, typename Element, typename Replacement>
  struct ReplaceImpl<List<Element, Tail...>, Element, Replacement> { using Result = List<Replacement, Tail...>; };
  
  
  template <typename List1, typename List2> struct ConcatImpl;
  template <typename... Elements1, typename... Elements2>
  struct ConcatImpl<List<Elements1...>, List<Elements2...>> { using Result = List<Elements1..., Elements2...>; };
  template <typename List1, typename List2> using Concat = Eval<ConcatImpl<List1, List2>>;
  
  template <typename List> struct ReverseImpl;
  template <typename List> using Reverse = Eval<ReverseImpl<List>>;
  template <typename Type, typename... Elements>
  struct ReverseImpl<List<Type, Elements...>> { using Result = Append<Reverse<List<Elements...>>, Type>; };
  template <> struct ReverseImpl<List<>> { using Result = List<>; };
  
  template <typename List, template <typename, typename> class Function, typename Memo> struct InjectImpl;
  template <typename List, template <typename, typename> class Function, typename Memo> using Inject = Eval<InjectImpl<List, Function, Memo>>;
  template <typename Head, typename... Tail, typename Memo, template <typename, typename> class Function>
  struct InjectImpl<List<Head, Tail...>, Function, Memo> { using Result = Inject<List<Tail...>, Function, Function<Memo, Head>>; };
  template <typename Memo, template <typename, typename> class Function>
  struct InjectImpl<List<>, Function, Memo> { using Result = Memo; };
  
  template <typename List, template <typename, typename> class Function> struct InjectFirstImpl;
  template <typename List, template <typename, typename> class Function> using InjectFirst = Eval<InjectFirstImpl<List, Function>>;
  template <typename Type, template <typename, typename> class Function, typename... Elements>
  struct InjectFirstImpl<List<Type, Elements...>, Function> { using Result = Inject<List<Elements...>, Function, Type>; };
  
  template <typename List>
  struct LengthImpl {
    template <typename Memo, typename Type> using Counter = Increment<Memo>;
    using Result = Inject<List, Counter, Int<0>>;
  };
  template <typename List> using Length = Eval<LengthImpl<List>>;
  
  template <typename L, template <typename> class Function>
  struct MapImpl {
    template <typename Memo, typename Type> using Mapper = Append<Memo, Function<Type>>;
    using Result = Inject<L, Mapper, List<>>;
  };
  template <typename List, template <typename> class Function> using Map = Eval<MapImpl<List, Function>>;
  
  template <typename L, template <typename, typename> class Function>
  struct MapWithIndexImpl {
    template <typename Memo, typename Type> using Mapper = Append<Memo, Function<Type, Length<Memo>>>;
    using Result = Inject<L, Mapper, List<>>;
  };
  template <typename List, template <typename, typename> class Function> using MapWithIndex = Eval<MapWithIndexImpl<List, Function>>;
  
  template <typename L, template <typename> class Predicate>
  struct SelectImpl {
    template <typename Memo, typename Type> using Selector = If<Predicate<Type>, AppendImpl<Memo, Type>, Memo>;
    using Result = Inject<L, Selector, List<>>;
  };
  template <typename List, template <typename> class Predicate> using Select = Eval<SelectImpl<List, Predicate>>;
  
  template <typename L, template <typename> class Predicate>
  struct RejectImpl {
    template <typename Memo, typename Type> using Rejector = If<Predicate<Type>, Memo, AppendImpl<Memo, Type>>;
    using Result = Inject<L, Rejector, List<>>;
  };
  template <typename List, template <typename> class Predicate> using Reject = Eval<RejectImpl<List, Predicate>>;
  
  template <typename List, template <typename> class Predicate> struct FindImpl;
  template <typename List, template <typename> class Predicate> using Find = Eval<FindImpl<List, Predicate>>;
  template <typename Head, typename... Tail, template <typename> class Predicate>
  struct FindImpl<List<Head, Tail...>, Predicate> { using Result = If<Predicate<Head>, Head, FindImpl<List<Tail...>, Predicate>>; };
  
  template <typename List> using Empty = Same<Length<List>, Int<0>>;
  template <typename List> using NotEmpty = GreaterThan<Length<List>, Int<0>>;
  template <typename List> using Many = GreaterThan<Length<List>, Int<1>>;
  template <typename List> using One = Same<Length<List>, Int<1>>;
  
  template <typename List, template <typename> class Predicate> using Any = NotEmpty<Select<List, Predicate>>;
  template <typename List, template <typename> class Predicate> using All = Same<Select<List, Predicate>, List>;
  template <typename List, template <typename> class Predicate> using None = Empty<Select<List, Predicate>>;
  
  template <typename List, typename Type> struct ContainsImpl;
  template <typename List, typename Type> using Contains = Eval<ContainsImpl<List, Type>>;
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
  template <typename List> using Unique = Eval<UniqueImpl<List>>;
  
  template <typename List, typename ContainingList> struct IsSubsetOfImpl;
  template <typename List, typename ContainingList> using IsSubsetOf = Eval<IsSubsetOfImpl<List, ContainingList>>;
  template <typename List, typename ContainingList>
  struct IsSubsetOfImpl {
    template <typename Type> using IsContained = Contains<ContainingList, Type>;
    using Result = All<List, IsContained>;
  };
  
  template <typename List1, typename List2> using SameElements = And<IsSubsetOf<List1, List2>, IsSubsetOf<List2, List1>>;
  
  template <typename List, typename Type> struct IndexImpl;
  template <typename List, typename Type> using Index = Eval<IndexImpl<List, Type>>;
  template <typename Head, typename... Tail, typename Type>
  struct IndexImpl<List<Head, Tail...>, Type> { using Result = Increment<Index<List<Tail...>, Type>>; };
  template <typename... Tail, typename Type>
  struct IndexImpl<List<Type, Tail...>, Type> { using Result = Int<0>; };
  
  template <typename List> struct FirstImpl;
  template <typename Head, typename... Tail> struct FirstImpl<List<Head, Tail...>> { using Result = Head; };
  template <typename List> using First = Eval<FirstImpl<List>>;
  
  template <typename List> struct LastImpl;
  template <typename List> using Last = Eval<LastImpl<List>>;
  template <typename Head, typename... Tail> struct LastImpl<List<Head, Tail...>> { using Result = Last<List<Tail...>>; };
  template <typename Head> struct LastImpl<List<Head>> { using Result = Head; };
  
  template <typename List, typename Index> struct AtImpl;
  template <typename List, typename Index> using At = Eval<AtImpl<List, Index>>;
  template <typename Head, typename... Tail, typename Index>
  struct AtImpl<List<Head, Tail...>, Index> { using Result = At<List<Tail...>, Decrement<Index>>; };
  template <typename Head, typename... Tail>
  struct AtImpl<List<Head, Tail...>, Int<0>> { using Result = Head; };
  
  template <typename Type> struct FlattenImpl;
  template <typename Type> using Flatten = Eval<FlattenImpl<Type>>;
  template <typename Type> struct FlattenImpl { using Result = List<Type>; };
  template <typename Head, typename... Tail>
  struct FlattenImpl<List<Head, Tail...>> { using Result = Concat<Flatten<Head>, Flatten<List<Tail...>>>; };
  template <>
  struct FlattenImpl<List<>> { using Result = List<>; };
  
  template <typename List, typename Index> struct BeforeIndexImpl;
  template <typename List, typename Index> using BeforeIndex = Eval<BeforeIndexImpl<List, Index>>;
  template <typename Index, typename Head, typename... Tail>
  struct BeforeIndexImpl<List<Head, Tail...>, Index> { using Result = Prepend<Head, BeforeIndex<List<Tail...>, Decrement<Index>>>; };
  template <typename Head, typename... Tail>
  struct BeforeIndexImpl<List<Head, Tail...>, Int<0>> { using Result = List<>; };
  template <>
  struct BeforeIndexImpl<List<>, Int<0>> { using Result = List<>; };
  
  template <typename List, typename Index> struct AfterIndexImpl;
  template <typename List, typename Index> using AfterIndex = Eval<AfterIndexImpl<List, Index>>;
  template <typename Head, typename... Tail, typename Index>
  struct AfterIndexImpl<List<Head, Tail...>, Index> { using Result = AfterIndex<List<Tail...>, Decrement<Index>>; };
  template <typename Head, typename... Tail>
  struct AfterIndexImpl<List<Head, Tail...>, Int<-1>> { using Result = List<Head, Tail...>; };
  template <>
  struct AfterIndexImpl<List<>, Int<-1>> { using Result = List<>; };
  
  template <typename List, typename Type> struct BeforeImpl;
  template <typename List, typename Type> using Before = Eval<BeforeImpl<List, Type>>;
  template <typename Head, typename... Tail, typename Type>
  struct BeforeImpl<List<Head, Tail...>, Type> { using Result = Prepend<Head, Before<List<Tail...>, Type>>; };
  template <typename Type, typename... Tail>
  struct BeforeImpl<List<Type, Tail...>, Type> { using Result = List<>; };
  
  template <typename List, typename Type> struct AfterImpl;
  template <typename List, typename Type> using After = Eval<AfterImpl<List, Type>>;
  template <typename Head, typename... Tail, typename Type>
  struct AfterImpl<List<Head, Tail...>, Type> { using Result = After<List<Tail...>, Type>; };
  template <typename Type, typename... Tail>
  struct AfterImpl<List<Type, Tail...>, Type> { using Result = List<Tail...>; };
  
  template <typename List, typename Excluded>
  struct ExcludeImpl {
    template <typename Type> using IsExcluded = Contains<Excluded, Type>;
    using Result = Reject<List, IsExcluded>;
  };
  template <typename List, typename Excluded> using Exclude = Eval<ExcludeImpl<List, Excluded>>;
  
  template <typename List, template <typename, typename> class LessThan> struct SortImpl;
  template <typename List, template <typename, typename> class LessThan> using Sort = Eval<SortImpl<List, LessThan>>;
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
    template <typename List1, typename List2> using Merge = Eval<MergeImpl<List1, List2>>;
    
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
  template <typename First, typename Last> using Sequence = Eval<SequenceImpl<First, Last>>;
  template <typename First, typename Last>
  struct SequenceImpl { using Result = Append<Sequence<First, Decrement<Last>>, Last>; };
  template <typename First>
  struct SequenceImpl<First, First> { using Result = List<First>; };
  
  template <typename List, template <typename> class Function = Self> struct Each;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Each<List<Head, Tail...>, Function> {
    template <typename... Args>
    void operator()(Args... args) { Function<Head>()(args...); Each<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Each<List<>, Function> {
    template <typename... Args>
    void operator()(Args... args) { }
  };
  
  template <typename List, template <typename> class Function = Self> struct While;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct While<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator()(Args... args) { return Function<Head>()(args...) && While<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct While<List<>, Function> {
    template <typename... Args>
    bool operator()(Args... args) { return true; }
  };
  
  template <typename List, template <typename> class Function = Self> struct Until;
  template <typename Head, typename... Tail, template <typename> class Function>
  struct Until<List<Head, Tail...>, Function> {
    template <typename... Args>
    bool operator()(Args... args) { return Function<Head>()(args...) || Until<List<Tail...>, Function>()(args...); }
  };
  template <template <typename> class Function>
  struct Until<List<>, Function> {
    template <typename... Args>
    bool operator()(Args... args) { return false; }
  };
}

#endif
