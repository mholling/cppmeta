#ifndef CPPMETA_META_MANY_H
#define CPPMETA_META_MANY_H

#include "meta/value.h"
#include "meta/list.h"

namespace CppMeta {
  template <typename Type, typename... Types> using PlusMany       = Inject<List<Types...>, Plus, Type>;
  template <typename Type, typename... Types> using MinusMany      = Inject<List<Types...>, Minus, Type>;
  template <typename Type, typename... Types> using MultiplyMany   = Inject<List<Types...>, Multiply, Type>;
  template <typename Type, typename... Types> using DivideMany     = Inject<List<Types...>, Divide, Type>;
  template <typename Type, typename... Types> using OrMany         = Inject<List<Types...>, Or, Type>;
  template <typename Type, typename... Types> using AndMany        = Inject<List<Types...>, And, Type>;
  template <typename Type, typename... Types> using BitwiseOrMany  = Inject<List<Types...>, BitwiseOr, Type>;
  template <typename Type, typename... Types> using BitwiseAndMany = Inject<List<Types...>, BitwiseAnd, Type>;
}

#endif
