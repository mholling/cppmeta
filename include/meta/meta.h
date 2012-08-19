#ifndef CPPMETA_META_META_H
#define CPPMETA_META_META_H

#include "meta/value.h"

namespace CppMeta {
  template <typename Type1, typename Type2> struct SameImpl { using Result = Bool<false>; };
  template <typename Type> struct SameImpl<Type, Type> { using Result = Bool<true>; };
  template <typename Type1, typename Type2> using Same = typename SameImpl<Type1, Type2>::Result;
  
  template <typename Type1, typename Type2> struct DifferentImpl { using Result = Bool<true>; };
  template <typename Type> struct DifferentImpl<Type, Type> { using Result = Bool<false>; };
  template <typename Type1, typename Type2> using Different = typename DifferentImpl<Type1, Type2>::Result;
  
  template <typename Type> using Self = Type;
  
  template <typename Type>
  struct CanEvalImpl {
    template <typename U> static Bool<true>  test(typename U::Result *);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  template <typename Type> using CanEval = typename CanEvalImpl<Type>::Result;
  
  template <typename Type, typename Evalable = CanEval<Type>> struct EvalImpl;
  template <typename Type> struct EvalImpl<Type, Bool<false>> { using Result = Type; };
  template <typename Type> struct EvalImpl<Type, Bool<true>> { using Result = typename Type::Result; };
  template <typename Type> using Eval = typename EvalImpl<Type>::Result;
  
  template <typename B, typename Type1, typename Type2> struct IfImpl;
  template <typename Type1, typename Type2> struct IfImpl<Bool<true>, Type1, Type2> { using Result = Eval<Type1>; };
  template <typename Type1, typename Type2> struct IfImpl<Bool<false>, Type1, Type2> { using Result = Eval<Type2>; };
  template <typename B, typename Type1, typename Type2> using If = typename IfImpl<B, Type1, Type2>::Result;
  
  template <typename Type1, typename Type2> using LesserOf = If<LessThan<Type1, Type2>, Type1, Type2>;
  template <typename Type1, typename Type2> using GreaterOf = If<GreaterThan<Type1, Type2>, Type1, Type2>;
  
  template <typename Type, typename Base>
  struct IsOrInheritsImpl {
    static Bool<true>  test(Base *);
    static Bool<false> test(...);
    using Result = decltype(test(static_cast<Type *>(0)));
  };
  template <typename Type, typename Base> using IsOrInherits = typename IsOrInheritsImpl<Type, Base>::Result;
  template <typename Type, typename Base> using Inherits = And<IsOrInherits<Type, Base>, Different<Type, Base>>;
  
  template <typename Type, typename Return, typename... Args>
  struct HasCallOperatorImpl {
    template <typename U, Return (U::*)(Args... args)> struct Signature;
    template <typename U> static Bool<true>  test(Signature<U, &U::operator()> *);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  template <typename Type, typename Return, typename... Args> using HasCallOperator = typename HasCallOperatorImpl<Type, Return, Args...>::Result;
  
  template <typename Type, typename... Args> using HasBoolCallOperator = HasCallOperator<Type, bool, Args...>;
  template <typename Type, typename... Args> using HasVoidCallOperator = HasCallOperator<Type, void, Args...>;
  
  template <typename Type>
  struct IsClassImpl {
    template <typename U> static Bool<true>  test(int U::*);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  template <typename Type> using IsClass = typename IsClassImpl<Type>::Result;
  
  template <typename Type>
  struct IsCompleteImpl {
    template <typename U> static Bool<true>  test(int(*)[sizeof(U)]);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  template <typename Type> using IsComplete = typename IsCompleteImpl<Type>::Result;
  template <typename Type> using IsIncomplete = Not<IsComplete<Type>>;
  
  template <typename Type> struct IsVolatileImpl { using Result = Bool<false>; };
  template <typename Type> struct IsVolatileImpl<volatile Type> { using Result = Bool<true>; };
  template <typename Type> using IsVolatile = typename IsVolatileImpl<Type>::Result;
  
  template <typename Type> struct IsConstImpl { using Result = Bool<false>; };
  template <typename Type> struct IsConstImpl<const Type> { using Result = Bool<true>; };
  template <typename Type> using IsConst = typename IsConstImpl<Type>::Result;
  
  struct DoNothing { template <typename... Args> void operator()(Args...) { } };
  
  template <typename Predicate, typename ActionIfTrue, typename ActionIfFalse = DoNothing>
  struct DoIf {
    template <typename... Args>
    void operator()(Args... args) { If<Predicate, ActionIfTrue, ActionIfFalse>()(args...); }
  };
}

#endif
