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
  
  template <int position, typename Type = int> using Bit = Value<Type, (1<<position)>;
  template <int width, int position, typename Type = int> using BitMask = Value<Type, (((1ull<<width)-1)<<position)>;
  
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
  
  template <typename Type1, typename Type2> struct MultiplyImpl;
  template <typename Type1, typename Type2> using Multiply = typename MultiplyImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct MultiplyImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1*t2>; };
  template <typename Type1, typename Type2> struct DivideImpl;
  template <typename Type1, typename Type2> using Divide = typename DivideImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct DivideImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1/t2>; };
  
  template <typename Type1, typename Type2> struct ModulusImpl;
  template <typename Type1, typename Type2> using Modulus = typename ModulusImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct ModulusImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Value<Type, t1%t2>; };
  
  template <typename Type1, typename Type2> struct LeftShiftImpl;
  template <typename Type1, typename Type2> using LeftShift = typename LeftShiftImpl<Type1, Type2>::Result;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct LeftShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1<<t2)>; };
  template <typename Type1, typename Type2> struct RightShiftImpl;
  template <typename Type1, typename Type2> using RightShift = typename RightShiftImpl<Type1, Type2>::Result;
  template <typename Type1, Type1 t1, typename Type2, Type2 t2>
  struct RightShiftImpl<Value<Type1, t1>, Value<Type2, t2>> { using Result = Value<Type1, (t1>>t2)>; };
  
  template <typename Exponent> using PowerOfTwo = LeftShift<Int<1>, Exponent>;
  
  template <typename Type1, typename Type2> struct LessThanImpl;
  template <typename Type1, typename Type2> using LessThan = typename LessThanImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct LessThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 < t2)>; };
  template <typename Type1, typename Type2> struct GreaterThanImpl;
  template <typename Type1, typename Type2> using GreaterThan = typename GreaterThanImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct GreaterThanImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 > t2)>; };
  
  template <typename Type1, typename Type2> struct LessThanOrEqualToImpl;
  template <typename Type1, typename Type2> using LessThanOrEqualTo = typename LessThanOrEqualToImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct LessThanOrEqualToImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 <= t2)>; };
  template <typename Type1, typename Type2> struct GreaterThanOrEqualToImpl;
  template <typename Type1, typename Type2> using GreaterThanOrEqualTo = typename GreaterThanOrEqualToImpl<Type1, Type2>::Result;
  template <typename Type, Type t1, Type t2>
  struct GreaterThanOrEqualToImpl<Value<Type, t1>, Value<Type, t2>> { using Result = Bool<(t1 >= t2)>; };
  
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
  
  template <typename Type> struct LowBitPositionImpl;
  template <typename Type> using LowBitPosition = typename LowBitPositionImpl<Type>::Result;
  template <typename Type, Type t>
  struct LowBitPositionImpl<Value<Type, t>> { using Result = Int<t & 1 ? 0 : 1 + LowBitPosition<Value<Type, (t>>1)>>::value>; };
  template <typename Type> struct LowBitPositionImpl<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename Type> struct HighBitPositionImpl;
  template <typename Type> using HighBitPosition = typename HighBitPositionImpl<Type>::Result;
  template <typename Type, Type t>
  struct HighBitPositionImpl<Value<Type, t>> { using Result = Increment<HighBitPosition<Value<Type, (t>>1)>>>; };
  template <typename Type> struct HighBitPositionImpl<Value<Type, Type(1)>> { using Result = Int<0>; };
  
  template <typename Type> struct BitCountImpl;
  template <typename Type> using BitCount = typename BitCountImpl<Type>::Result;
  template <typename Type, Type t>
  struct BitCountImpl<Value<Type, t>> { using Result = Plus<Int<t & 1>, BitCount<Value<Type, (t>>1)>>>; };
  template <typename Type>
  struct BitCountImpl<Value<Type, Type(0)>> { using Result = Int<0>; };
  
  template <typename Type> struct IsZeroImpl;
  template <typename Type> using IsZero = typename IsZeroImpl<Type>::Result;
  template <typename Type, Type t> struct IsZeroImpl<Value<Type, t>> { using Result = Bool<t == Type()>; };
  
  template <typename Type> using IsPowerOfTwo = And<IsZero<BitwiseAnd<Type, Decrement<Type>>>, Not<IsZero<Type>>>;
}

#endif
