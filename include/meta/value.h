#ifndef CPPMETA_META_VALUE_H
#define CPPMETA_META_VALUE_H

#include <stdint.h>

namespace CppMeta {
  template <typename Type, Type t> struct Value { static constexpr Type value = t; };
  template <bool b> using Bool = Value<bool, b>;
  template <int  i> using Int  = Value<int,  i>;
  template <long l> using Long = Value<long, l>;
  template <char c> using Char = Value<char, c>;
  template <uint32_t u> using UInt32 = Value<uint32_t, u>;
  template <uint64_t u> using UInt64 = Value<uint64_t, u>;
  
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
  
  template <int position, typename Type = int> using Bit = Value<Type, (1<<position)>;
  template <int width, int position, typename Type = int> using BitMask = Value<Type, (((1ull<<width)-1)<<position)>;
  
  template <typename Type> struct IncrementImpl;
  template <typename Type> using Increment = Eval<IncrementImpl<Type>>;
  template <typename Type, Type t> struct IncrementImpl<Value<Type, t>> { using Result = Value<Type, t+1>; };
  template <typename Type> struct DecrementImpl;
  template <typename Type> using Decrement = Eval<DecrementImpl<Type>>;
  template <typename Type, Type t> struct DecrementImpl<Value<Type, t>> { using Result = Value<Type, t-1>; };
  
  template <typename Type1, typename Type2> struct PlusImpl;
  template <typename Type1, typename Type2> using Plus = Eval<PlusImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct PlusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1+t2>; };
  template <typename Type1, typename Type2> struct MinusImpl;
  template <typename Type1, typename Type2> using Minus = Eval<MinusImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct MinusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1-t2>; };
  
  template <typename Type1, typename Type2> struct MultiplyImpl;
  template <typename Type1, typename Type2> using Multiply = Eval<MultiplyImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct MultiplyImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1*t2>; };
  template <typename Type1, typename Type2> struct DivideImpl;
  template <typename Type1, typename Type2> using Divide = Eval<DivideImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct DivideImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1/t2>; };
  
  template <typename Type1, typename Type2> struct ModulusImpl;
  template <typename Type1, typename Type2> using Modulus = Eval<ModulusImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct ModulusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1%t2>; };
  
  template <typename Type1, typename Type2> struct LeftShiftImpl;
  template <typename Type1, typename Type2> using LeftShift = Eval<LeftShiftImpl<Type1, Type2>>;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct LeftShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1<<t2)>; };
  template <typename Type1, typename Type2> struct RightShiftImpl;
  template <typename Type1, typename Type2> using RightShift = Eval<RightShiftImpl<Type1, Type2>>;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct RightShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1>>t2)>; };
  
  template <typename Exponent> using PowerOfTwo = LeftShift<Int<1>, Exponent>;
  
  template <typename Type1, typename Type2> struct LessThanImpl;
  template <typename Type1, typename Type2> using LessThan = Eval<LessThanImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct LessThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 < t2)>; };
  template <typename Type1, typename Type2> struct GreaterThanImpl;
  template <typename Type1, typename Type2> using GreaterThan = Eval<GreaterThanImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct GreaterThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 > t2)>; };
  
  template <typename Type1, typename Type2> struct LessThanOrEqualToImpl;
  template <typename Type1, typename Type2> using LessThanOrEqualTo = Eval<LessThanOrEqualToImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct LessThanOrEqualToImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 <= t2)>; };
  template <typename Type1, typename Type2> struct GreaterThanOrEqualToImpl;
  template <typename Type1, typename Type2> using GreaterThanOrEqualTo = Eval<GreaterThanOrEqualToImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct GreaterThanOrEqualToImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 >= t2)>; };
  
  template <typename Type1, typename Type2> struct AndImpl;
  template <typename Type1, typename Type2> using And = Eval<AndImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct AndImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 && t2)>; };
  template <typename Type1, typename Type2> struct OrImpl;
  template <typename Type1, typename Type2> using Or = Eval<OrImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct OrImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 || t2)>; };
  template <typename Type> struct NotImpl;
  template <typename Type> using Not = Eval<NotImpl<Type>>;
  template <typename Type, Type t>
  struct NotImpl<Value<Type, t>> { using Result = Bool<!t>; };
  
  template <typename Type1, typename Type2> struct BitwiseOrImpl;
  template <typename Type1, typename Type2> using BitwiseOr = Eval<BitwiseOrImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct BitwiseOrImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 | t2> ;};
  template <typename Type1, typename Type2> struct BitwiseAndImpl;
  template <typename Type1, typename Type2> using BitwiseAnd = Eval<BitwiseAndImpl<Type1, Type2>>;
  template <typename Type, Type t1, Type t2>
  struct BitwiseAndImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1 & t2> ;};
  template <typename Type> struct BitwiseNotImpl;
  template <typename Type> using BitwiseNot = Eval<BitwiseNotImpl<Type>>;
  template <typename Type, Type t>
  struct BitwiseNotImpl<Value<Type, t>> { using Result = Value<Type, ~t> ;};
  
  template <typename Type> struct LowBitPositionImpl;
  template <typename Type> using LowBitPosition = Eval<LowBitPositionImpl<Type>>;
  template <typename Type, Type t>
  struct LowBitPositionImpl<Value<Type, t>> { using Result = Int<t & 1 ? 0 : 1 + LowBitPosition<Value<Type, (t>>1)>>::value>; };
  template <typename Type> struct LowBitPositionImpl<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename Type> struct HighBitPositionImpl;
  template <typename Type> using HighBitPosition = Eval<HighBitPositionImpl<Type>>;
  template <typename Type, Type t>
  struct HighBitPositionImpl<Value<Type, t>> { using Result = Increment<HighBitPosition<Value<Type, (t>>1)>>>; };
  template <typename Type> struct HighBitPositionImpl<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename Type> struct BitCountImpl;
  template <typename Type> using BitCount = Eval<BitCountImpl<Type>>;
  template <typename Type, Type t>
  struct BitCountImpl<Value<Type, t>> { using Result = Plus<Int<t & 1>, BitCount<Value<Type, (t>>1)>>>; };
  template <typename Type>
  struct BitCountImpl<Value<Type, Type(0)>> { using Result = Int<0>; };
  
  template <typename Type> struct IsZeroImpl;
  template <typename Type> using IsZero = Eval<IsZeroImpl<Type>>;
  template <typename Type, Type t> struct IsZeroImpl<Value<Type, t>> { using Result = Bool<t == Type()>; };
  
  template <typename Type> using IsPowerOfTwo = And<IsZero<BitwiseAnd<Type, Decrement<Type>>>, Not<IsZero<Type>>>;
}

#endif
