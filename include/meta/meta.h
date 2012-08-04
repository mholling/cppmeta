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
  
  template <typename> struct Increment;
  template <typename Type, Type t> struct Increment<Value<Type, t>> { using Result = Value<Type, t+1>; };
  template <typename> struct Decrement;
  template <typename Type, Type t> struct Decrement<Value<Type, t>> { using Result = Value<Type, t-1>; };
  
  template <typename, typename> struct Plus;
  template <typename Type, Type t1, Type t2>
  struct Plus<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1+t2>; };
  template <typename, typename> struct Minus;
  template <typename Type, Type t1, Type t2>
  struct Minus<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1-t2>; };
  
  template <typename, typename> struct LeftShift;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct LeftShift<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1<<t2)>; };
  template <typename, typename> struct RightShift;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct RightShift<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1>>t2)>; };
  
  template <typename, typename> struct LessThan;
  template <typename Type, Type t1, Type t2>
  struct LessThan<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 < t2)>; };
  template <typename, typename> struct GreaterThan;
  template <typename Type, Type t1, Type t2>
  struct GreaterThan<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 > t2)>; };
  
  template <typename, typename> struct And;
  template <typename Type, Type t1, Type t2>
  struct And<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 && t2)>; };
  template <typename, typename> struct Or;
  template <typename Type, Type t1, Type t2>
  struct Or<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 || t2)>; };
  template <typename> struct Not;
  template <typename Type, Type t>
  struct Not<Value<Type, t>> { using Result = Bool<!t>; };
  
  template <typename, typename> struct BitwiseOr;
  template <typename Type, Type t1, Type t2>
  struct BitwiseOr<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 | t2> ;};
  template <typename, typename> struct BitwiseAnd;
  template <typename Type, Type t1, Type t2>
  struct BitwiseAnd<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 & t2> ;};
  template <typename> struct BitwiseNot;
  template <typename Type, Type t>
  struct BitwiseNot<Value<Type, t>> { using Result = Value<Type, ~t> ;};
  
  template <typename> struct BitPosition;
  template <typename Type, Type t>
  struct BitPosition<Value<Type, t>> { using Result = Int<t & 1 ? 0 : 1 + BitPosition<Value<Type, (t>>1)>>::Result::value>; };
  template <typename Type> struct BitPosition<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename> struct BitCount;
  template <typename Type, Type t>
  struct BitCount<Value<Type, t>> { using Result = typename Plus<Int<t & 1>, typename BitCount<Value<Type, (t>>1)>>::Result>::Result; };
  template <typename Type>
  struct BitCount<Value<Type, Type(0)>> { using Result = Int<0>; };
  
  template <typename Type1, typename Type2> struct Same { using Result = Bool<false>; };
  template <typename Type> struct Same<Type, Type> { using Result = Bool<true>; };
  
  template <typename Type1, typename Type2> using Different = Not<typename Same<Type1, Type2>::Result>;
  
  template <typename Type> struct Self { using Result = Type; };
  
  template <typename Type>
  struct CanEval {
    template <typename U> static Bool<true>  test(typename U::Result *);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  
  template <typename Type, typename Evalable = typename CanEval<Type>::Result> struct Eval;
  template <typename Type> struct Eval<Type, Bool<false>> { using Result = Type; };
  template <typename Type> struct Eval<Type, Bool<true>> { using Result = typename Type::Result; };
  
  template <typename, typename, typename> struct If;
  template <typename Type1, typename Type2> struct If<Bool<true>, Type1, Type2> { using Result = typename Eval<Type1>::Result; };
  template <typename Type1, typename Type2> struct If<Bool<false>, Type1, Type2> { using Result = typename Eval<Type2>::Result; };
  
  template <typename T1, typename T2> using LesserOf = If<typename LessThan<T1, T2>::Result, T1, T2>;
  template <typename T1, typename T2> using GreaterOf = If<typename GreaterThan<T1, T2>::Result, T1, T2>;
  
  template <typename Type, typename Base>
  struct IsOrInherits {
    static Bool<true>  test(Base *);
    static Bool<false> test(...);
    using Result = decltype(test(static_cast<Type *>(0)));
  };
  
  template <typename Type, typename Base>
  struct Inherits {
    using Result = typename And<typename IsOrInherits<Type, Base>::Result, typename Different<Type, Base>::Result>::Result;
  };
  
  template <typename Type, typename Return, typename... Args>
  struct HasCallOperator {
    template <typename U, Return (U::*)(Args... args)> struct Signature;
    template <typename U> static Bool<true>  test(Signature<U, &U::operator ()> *);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  
  template <typename Type, typename... Args> using HasBoolCallOperator = HasCallOperator<Type, bool, Args...>;
  template <typename Type, typename... Args> using HasVoidCallOperator = HasCallOperator<Type, void, Args...>;
  
  template <typename Type>
  struct IsClass {
    template <typename U> static Bool<true>  test(int U::*);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  
  template <typename Type>
  struct IsComplete {
    template <typename U> static Bool<true>  test(int(*)[sizeof(U)]);
    template <typename>   static Bool<false> test(...);
    using Result = decltype(test<Type>(0));
  };
  
  template <typename Type> using IsIncomplete = Not<typename IsComplete<Type>::Result>;
  
  struct DoNothing { template <typename... Args> void operator()(Args...) { } };
  
  template <typename Predicate, typename ActionIfTrue, typename ActionIfFalse = DoNothing>
  struct DoIf {
    template <typename... Args>
    void operator()(Args... args) { typename If<Predicate, ActionIfTrue, ActionIfFalse>::Result()(args...); }
  };
}

#endif
