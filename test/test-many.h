#ifndef CPPMETA_TEST_MANY_H
#define CPPMETA_TEST_MANY_H

#include "meta/meta.h"
#include "meta/value.h"
#include "meta/many.h"

namespace CppMeta {
  namespace Test {
    namespace TestMany {
      static_assert(Same<AndMany<Bool<true>, Bool<true>, Bool<true>>, Bool<true>>::value, "failed");
      static_assert(Same<AndMany<Bool<true>, Bool<true>, Bool<false>>, Bool<false>>::value, "failed");
      static_assert(Same<OrMany<Bool<false>, Bool<false>, Bool<false>>, Bool<false>>::value, "failed");
      static_assert(Same<OrMany<Bool<false>, Bool<false>, Bool<true>>, Bool<true>>::value, "failed");
      static_assert(Same<PlusMany<Int<3>, Int<2>, Int<7>>, Int<12>>::value, "failed");
      static_assert(Same<MinusMany<Int<5>, Int<3>, Int<4>>, Int<-2>>::value, "failed");
      static_assert(Same<MultiplyMany<Int<3>, Int<2>, Int<7>>, Int<42>>::value, "failed");
      static_assert(Same<DivideMany<Int<15>, Int<3>, Int<5>>, Int<1>>::value, "failed");
      static_assert(Same<BitwiseOrMany<Int<0x1>, Int<0x2>, Int<0x4>>, Int<0x7>>::value, "failed");
      static_assert(Same<BitwiseAndMany<Int<0x7>, Int<0x3>, Int<0x6>>, Int<0x2>>::value, "failed");
    }
  }
}

#endif
