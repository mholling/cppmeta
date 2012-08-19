#ifndef CPPMETA_STM32_VARIANTS_H
#define CPPMETA_STM32_VARIANTS_H

namespace CppMeta {
  namespace Stm32 {
    struct STM32F;
    struct STM32F4XX;
    struct STM32F10X;
    struct STM32F10X_CL;
    struct STM32F10X_LD;
    struct STM32F10X_LD_VL;
    struct STM32F10X_MD;
    struct STM32F10X_MD_VL;
    struct STM32F10X_HD;
    struct STM32F10X_HD_VL;
    struct STM32F10X_XL;
    
    using Variants =
      Tree<STM32F,
      Tree<STM32F4XX>,
      Tree<STM32F10X,
        Tree<STM32F10X_CL>,
        Tree<STM32F10X_LD,
          Tree<STM32F10X_LD_VL>>,
        Tree<STM32F10X_MD,
          Tree<STM32F10X_MD_VL>>,
        Tree<STM32F10X_HD,
          Tree<STM32F10X_HD_VL>>,
        Tree<STM32F10X_XL>>>;
  }
}

#endif
