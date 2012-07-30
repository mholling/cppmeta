namespace CppMeta {
  template <typename Type, Type t> struct Value { enum : Type { value = t }; };
  // template <typename Type, Type t> struct Value { static const Type value = t; };
  template <bool b> using Bool = Value<bool, b>;
  template <int  i> using Int  = Value<int,  i>;
  template <char c> using Char = Value<char, c>;
  
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
  
  template <typename Type1, typename Type2> struct Same { using Result = Bool<false>; };
  template <typename Type> struct Same<Type, Type> { using Result = Bool<true>; };
  
  template <typename Type> struct Self { using Result = Type; };
  
  template <typename Type>
  struct CanEval {
    struct Yes; struct No;
    template <typename U> static Yes& test(typename U::Result *);
    template <typename>   static No&  test(...);
    using Result = typename Same<decltype(test<Type>(0)), Yes&>::Result;
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
    struct Yes; struct No;
    static Yes& test(Base*);
    static No&  test(...);
    using Result = typename Same<decltype(test(static_cast<Type *>(0))), Yes&>::Result;
  };
  
  template <typename Type, typename Base>
  struct Inherits {
    using Result = Bool<IsOrInherits<Type, Base>::Result::value && !Same<Type, Base>::Result::value>;
  };
  
  template <typename Type, typename Return, typename... Args>
  struct HasCallOperator {
    struct Yes; struct No;
    template <typename C, Return (C::*)(Args... args)> struct Signature;
    template <typename C> static Yes& test(Signature<C, &C::operator ()> *);
    template <typename>   static No&  test(...);
    using Result = typename Same<decltype(test<Type>(0)), Yes&>::Result;
  };
  
  template <typename Type, typename... Args> using HasBoolCallOperator = HasCallOperator<Type, bool, Args...>;
  template <typename Type, typename... Args> using HasVoidCallOperator = HasCallOperator<Type, void, Args...>;
  
  struct DoNothing { template <typename... Args> void operator()(Args...) { } };
  
  template <typename Predicate, typename ActionIfTrue, typename ActionIfFalse = DoNothing>
  struct DoIf {
    template <typename... Args>
    void operator()(Args... args) { typename If<Predicate, ActionIfTrue, ActionIfFalse>::Result()(args...); }
  };
}


