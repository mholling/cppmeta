#ifndef CPPMETA_CORTEX_VECTOR_TABLE_H
#define CPPMETA_CORTEX_VECTOR_TABLE_H

#include "meta/value.h"
#include "meta/list.h"
#include "meta/os.h"
#include "cortex/scb.h"

namespace CppMeta {
  namespace Cortex {
    template <typename Machines, typename HighestInterrupt>
    using VectorTable = OS::VectorTable<OS::Kernel<Scb1, Machines>, Sequence<Int<-15>, HighestInterrupt>>;
  }
}

#endif
