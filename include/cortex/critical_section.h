#ifndef CPPMETA_CORTEX_CRITICAL_SECTION_H
#define CPPMETA_CORTEX_CRITICAL_SECTION_H

#include <stdint.h>

namespace CppMeta {
  namespace Cortex {
    // TODO: use BASEPRI instead?
    template <typename Block>
    void critical_section(Block block) {
      uint32_t primask;
      __asm volatile (
        "mrs    %[primask], primask \n\t"
        "cpsid  i                   \n\t" : [primask] "=r" (primask)
      );
      block();
      __asm volatile (
        "msr    primask, %[primask] \n\t" : : [primask] "r" (primask)
      );
    }
    
    template <typename Block>
    struct CriticalSection { void operator()() { critical_section(Block()); } };
  }
}

#endif
