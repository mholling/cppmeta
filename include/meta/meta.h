#ifndef CPPMETA_META_META_H
#define CPPMETA_META_META_H

#include <stdint.h>

namespace CppMeta {
  template <typename Type, Type t> struct Value { static constexpr Type value = t; };
  template <bool b> using Bool = Value<bool, b>;
  template <int  i> using Int  = Value<int,  i>;
  template <long l> using Long = Value<long, l>;
  template <char c> using Char = Value<char, c>;
  
  template <int position> using Bit = Int<(1<<position)>;
  template <int width, int position> using BitMask = Int<(((1ull<<width)-1)<<position)>;
  
  template <typename Type> struct IncrementImpl;
  template <typename Type> using Increment = typename IncrementImpl<Type>::Result;
  template <typename Type, Type t> struct IncrementImpl<Value<Type, t>> { using Result = Value<Type, t+1>; };
  template <typename Type> struct DecrementImpl;
  template <typename Type> using Decrement = typename DecrementImpl<Type>::Result;
  template <typename Type, Type t> struct DecrementImpl<Value<Type, t>> { using Result = Value<Type, t-1>; };
  
  template <typename Type1, typename Type2> struct PlusImpl;
  template <typename Type1, typename Type2> using Plus = typename PlusImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct PlusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1+t2>; };
  template <typename Type1, typename Type2> struct MinusImpl;
  template <typename Type1, typename Type2> using Minus = typename MinusImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct MinusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1-t2>; };
  
  template <typename Type1, typename Type2> struct LeftShiftImpl;
  template <typename Type1, typename Type2> using LeftShift = typename LeftShiftImpl<Type1, Type2>::Result;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct LeftShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1<<t2)>; };
  template <typename Type1, typename Type2> struct RightShiftImpl;
  template <typename Type1, typename Type2> using RightShift = typename RightShiftImpl<Type1, Type2>::Result;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct RightShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1>>t2)>; };
  
  template <typename Type1, typename Type2> struct LessThanImpl;
  template <typename Type1, typename Type2> using LessThan = typename LessThanImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct LessThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 < t2)>; };
  template <typename Type1, typename Type2> struct GreaterThanImpl;
  template <typename Type1, typename Type2> using GreaterThan = typename GreaterThanImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct GreaterThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 > t2)>; };
  
  template <typename Type1, typename Type2> struct AndImpl;
  template <typename Type1, typename Type2> using And = typename AndImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct AndImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 && t2)>; };
  template <typename Type1, typename Type2> struct OrImpl;
  template <typename Type1, typename Type2> using Or = typename OrImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct OrImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 || t2)>; };
  template <typename Type> struct NotImpl;
  template <typename Type> using Not = typename NotImpl<Type>::Result;
  template <typename Type, Type t>
  struct NotImpl<Value<Type, t>> { using Result = Bool<!t>; };
  
  template <typename Type1, typename Type2> struct BitwiseOrImpl;
  template <typename Type1, typename Type2> using BitwiseOr = typename BitwiseOrImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct BitwiseOrImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 | t2> ;};
  template <typename Type1, typename Type2> struct BitwiseAndImpl;
  template <typename Type1, typename Type2> using BitwiseAnd = typename BitwiseAndImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct BitwiseAndImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 & t2> ;};
  template <typename Type> struct BitwiseNotImpl;
  template <typename Type> using BitwiseNot = typename BitwiseNotImpl<Type>::Result;
  template <typename Type, Type t>
  struct BitwiseNotImpl<Value<Type, t>> { using Result = Value<Type, ~t> ;};
  
  template <typename Type> struct BitPositionImpl;
  template <typename Type> using BitPosition = typename BitPositionImpl<Type>::Result;
  template <typename Type, Type t>
  struct BitPositionImpl<Value<Type, t>> { using Result = Int<t & 1 ? 0 : 1 + BitPosition<Value<Type, (t>>1)>>::value>; };
  template <typename Type> struct BitPositionImpl<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename Type> struct BitCountImpl;
  template <typename Type> using BitCount = typename BitCountImpl<Type>::Result;
  template <typename Type, Type t>
  struct BitCountImpl<Value<Type, t>> { using Result = Plus<Int<t & 1>, BitCount<Value<Type, (t>>1)>>>; };
  template <typename Type>
  struct BitCountImpl<Value<Type, Type(0)>> { using Result = Int<0>; };
  
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
    template <typename U> static Bool<true>  test(Signature<U, &U::operator ()> *);
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
  
  struct DoNothing { template <typename... Args> void operator()(Args...) { } };
  
  template <typename Predicate, typename ActionIfTrue, typename ActionIfFalse = DoNothing>
  struct DoIf {
    template <typename... Args>
    void operator()(Args... args) { If<Predicate, ActionIfTrue, ActionIfFalse>()(args...); }
  };
}

#endif
