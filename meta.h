namespace Meta {
  template <typename Type, Type t> struct Value { static const Type value = t; };
  template <bool b> using Bool = Value<bool, b>;
  template <int  i> using Int  = Value<int,  i>;
  template <char c> using Char = Value<char, c>;
  
  // template <bool b> struct Bool { static const bool value = b; };
  // template <int  i> struct Int  { static const int  value = i; };
  // template <char c> struct Char { static const char value = c; };
  
  template <typename Type1, typename Type2> struct Same { typedef Bool<false> Result; };
  template <typename Type> struct Same<Type, Type> { typedef Bool<true> Result; };
  
  template <typename Type> struct Self { typedef Type Result; };
  
  template <typename Type>
  struct CanEval {
    struct Yes { char a; };
    struct No  { struct Yes a[2]; };
    template <typename U> static Yes& test(typename U::Result *);
    template <typename>   static No&  test(...);
    typedef Bool<sizeof(test<Type>(0)) == sizeof(Yes)> Result;
  };
  
  template <typename Type, typename Evalable = typename CanEval<Type>::Result> struct Eval;
  template <typename Type> struct Eval<Type, Bool<false>> { typedef Type Result; };
  template <typename Type> struct Eval<Type, Bool<true>> { typedef typename Type::Result Result; };
  
  template <typename, typename, typename> struct If;
  template <typename Type1, typename Type2> struct If<Bool<true>, Type1, Type2> { typedef typename Eval<Type1>::Result Result; };
  template <typename Type1, typename Type2> struct If<Bool<false>, Type1, Type2> { typedef typename Eval<Type2>::Result Result; };
  
  template <typename Type, typename Base>
  struct IsOrInherits {
    struct Yes { char a; };
    struct No  { struct Yes a[2]; };
    static Yes test(Base*);
    static No  test(...);
    typedef Bool<sizeof(test(static_cast<Type *>(0))) == sizeof(Yes)> Result;
  };
  
  template <typename Type, typename Base>
  struct Inherits {
    typedef Bool<IsOrInherits<Type, Base>::Result::value && !Same<Type, Base>::Result::value> Result;
  };
  
  template <typename Type, typename Return, typename... Args>
  struct HasCallOperator {
    struct Yes { char a; };
    struct No  { struct Yes a[2]; };
    template <typename C, Return (C::*)(Args... args)> struct SFINAE;
    template <typename C> static Yes& test(SFINAE<C, &C::operator ()> *);
    template <typename>   static No&  test(...);
    typedef Bool<sizeof(test<Type>(0)) == sizeof(Yes)> Result;
  };
  
  template <typename Type, typename... Args>
  struct HasBoolCallOperator {
    typedef typename HasCallOperator<Type, bool, Args...>::Result Result;
  };
  
  // 
  // template <unsigned int width, unsigned int position>
  // struct Mask {
  //   enum { value = Mask<width - 1, position + 1>::value | Mask<1, position>::value };
  // };
  // 
  // template <unsigned int position>
  // struct Mask<1, position> {
  //   enum { value = 1 << position };
  // };
  // 
  // template <unsigned int width, unsigned int position>
  // struct BitField {
  //   template <typename T>
  //   T operator ()(T t) { return (t >> position) & Mask<width, 0>::value; }
  // };
  // 
  // template <uint32_t mask>
  // struct MaskToPosition {
  //   enum { value = mask & 1 ? 0 : 1 + MaskToPosition<(mask >> 1)>::value };
  // };
  // 
  // template <>
  // struct MaskToPosition<1> {
  //   enum { value = 0 };
  // };
  // 
  // template <uint32_t i>
  // struct BitCount {
  //   enum { value = (i & 1) + BitCount<(i >> 1)>::value };
  // };
  // 
  // template <>
  // struct BitCount<0> {
  //   enum { value = 0 };
  // };
  // 
  // template <uint32_t i>
  // struct IsPowerOfTwo {
  //   enum { value = i && (i & (i - 1)) == 0 };
  // };
  // 
  // template <uint32_t i>
  // struct Bits {
  //   enum { value = 1 + Bits<(i >> 1)>::value };
  // };
  // 
  // template <>
  // struct Bits<0> {
  //   enum { value = 0 };
  // };
  // 
  // template <int i>
  // struct Int {
  //   enum { value = i };
  // };
}


