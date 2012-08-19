#ifndef CPPMETA_CORTEX_SCB_H
#define CPPMETA_CORTEX_SCB_H

#include <stdint.h>
#include "cortex/registers.h"
#include "cortex/addresses.h"
#include "cortex/irqs.h"

namespace CppMeta {
  namespace Cortex {
    struct SCB_Type {
      volatile uint32_t CPUID;               /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
      volatile uint32_t ICSR;                /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
      volatile uint32_t VTOR;                /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
      volatile uint32_t AIRCR;               /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
      volatile uint32_t SCR;                 /*!< Offset: 0x010 (R/W)  System Control Register                               */
      volatile uint32_t CCR;                 /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
      volatile uint8_t  SHP[12];             /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
      volatile uint32_t SHCSR;               /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
      volatile uint32_t CFSR;                /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
      volatile uint32_t HFSR;                /*!< Offset: 0x02C (R/W)  HardFault Status Register                             */
      volatile uint32_t DFSR;                /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
      volatile uint32_t MMFAR;               /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register                      */
      volatile uint32_t BFAR;                /*!< Offset: 0x038 (R/W)  BusFault Address Register                             */
      volatile uint32_t AFSR;                /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
      volatile uint32_t PFR[2];              /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
      volatile uint32_t DFR;                 /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
      volatile uint32_t ADR;                 /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
      volatile uint32_t MMFR[4];             /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
      volatile uint32_t ISAR[5];             /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register                   */
               uint32_t RESERVED0[5];
      volatile uint32_t CPACR;               /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register                   */
    };
    
    template <typename Address>
    struct Scb : Registers<SCB_Type, Address> {
      using CPUID_IMPLEMENTER    = UInt32<0xFFUL<<24>;
      using CPUID_VARIANT        = UInt32<0xFUL<<20>;
      using CPUID_ARCHITECTURE   = UInt32<0xFUL<<16>;
      using CPUID_PARTNO         = UInt32<0xFFFUL<<4>;
      using CPUID_REVISION       = UInt32<0xFUL<<0>;
      using ICSR_NMIPENDSET      = UInt32<1<<31>;
      using ICSR_PENDSVSET       = UInt32<1<<28>;
      using ICSR_PENDSVCLR       = UInt32<1<<27>;
      using ICSR_PENDSTSET       = UInt32<1<<26>;
      using ICSR_PENDSTCLR       = UInt32<1<<25>;
      using ICSR_ISRPREEMPT      = UInt32<1<<23>;
      using ICSR_ISRPENDING      = UInt32<1<<22>;
      using ICSR_VECTPENDING     = UInt32<0x1FFUL<<12>;
      using ICSR_RETTOBASE       = UInt32<1<<11>;
      using ICSR_VECTACTIVE      = UInt32<0x1FFUL<<0>;
      using VTOR_TBLOFF          = UInt32<0x1FFFFFFUL<<7>;
      using AIRCR_VECTKEY        = UInt32<0xFFFFUL<<16>;
      using AIRCR_VECTKEYSTAT    = UInt32<0xFFFFUL<<16>;
      using AIRCR_ENDIANESS      = UInt32<1<<15>;
      using AIRCR_PRIGROUP       = UInt32<7UL<<8>;
      using AIRCR_SYSRESETREQ    = UInt32<1<<2>;
      using AIRCR_VECTCLRACTIVE  = UInt32<1<<1>;
      using AIRCR_VECTRESET      = UInt32<1<<0>;
      using SCR_SEVONPEND        = UInt32<1<<4>;
      using SCR_SLEEPDEEP        = UInt32<1<<2>;
      using SCR_SLEEPONEXIT      = UInt32<1<<1>;
      using CCR_STKALIGN         = UInt32<1<<9>;
      using CCR_BFHFNMIGN        = UInt32<1<<8>;
      using CCR_DIV_0_TRP        = UInt32<1<<4>;
      using CCR_UNALIGN_TRP      = UInt32<1<<3>;
      using CCR_USERSETMPEND     = UInt32<1<<1>;
      using CCR_NONBASETHRDENA   = UInt32<1<<0>;
      using SHCSR_USGFAULTENA    = UInt32<1<<18>;
      using SHCSR_BUSFAULTENA    = UInt32<1<<17>;
      using SHCSR_MEMFAULTENA    = UInt32<1<<16>;
      using SHCSR_SVCALLPENDED   = UInt32<1<<15>;
      using SHCSR_BUSFAULTPENDED = UInt32<1<<14>;
      using SHCSR_MEMFAULTPENDED = UInt32<1<<13>;
      using SHCSR_USGFAULTPENDED = UInt32<1<<12>;
      using SHCSR_SYSTICKACT     = UInt32<1<<11>;
      using SHCSR_PENDSVACT      = UInt32<1<<10>;
      using SHCSR_MONITORACT     = UInt32<1<<8>;
      using SHCSR_SVCALLACT      = UInt32<1<<7>;
      using SHCSR_USGFAULTACT    = UInt32<1<<3>;
      using SHCSR_BUSFAULTACT    = UInt32<1<<1>;
      using SHCSR_MEMFAULTACT    = UInt32<1<<0>;
      using CFSR_USGFAULTSR      = UInt32<0xFFFFUL<<16>;
      using CFSR_BUSFAULTSR      = UInt32<0xFFUL<<8>;
      using CFSR_MEMFAULTSR      = UInt32<0xFFUL<<0>;
      using HFSR_DEBUGEVT        = UInt32<1<<31>;
      using HFSR_FORCED          = UInt32<1<<30>;
      using HFSR_VECTTBL         = UInt32<1<<1>;
      using DFSR_EXTERNAL        = UInt32<1<<4>;
      using DFSR_VCATCH          = UInt32<1<<3>;
      using DFSR_DWTTRAP         = UInt32<1<<2>;
      using DFSR_BKPT            = UInt32<1<<1>;
      using DFSR_HALTED          = UInt32<1<<0>;
      
      template <typename Kernel>
      struct Initialise {
        void operator()() {
          Scb::registers.SHP[Plus<Int<12>, Irqs::PendSV>::value] = 0xff;
          Scb::registers.SHP[Plus<Int<12>, Irqs::SVCall>::value] = 0xff;
        }
      };
      
      static void (*preempt)();
      
      static void prepare(void (*new_preempt)()) { preempt = new_preempt; }
      static void push() { Scb::registers.ICSR = ICSR_PENDSVSET::value; }
      static void pop() { __asm volatile ("svc 0x01"); }
      static void enable() { __asm volatile ("cpsie i"); }
      __attribute__ ((noreturn))
      static void waitloop() { while (true) ; } // or: { while (true) __asm volatile ("wfi"); }
      
      template <typename Kernel, typename Interrupt> struct Handle;
      
      template <typename Kernel>
      struct Handle<Kernel, Irqs::SVCall> {
        __attribute__ ((naked))
        static void handle() {
          __asm volatile (
            "add    sp, sp, #(8*4) \n\t"
            "bx     lr             \n\t"
          );
        }
      };
    
      template <typename Kernel>
      struct Handle<Kernel, Irqs::PendSV> {
        __attribute__ ((naked))
        static void handle() {
          register uint32_t xpsr  asm("r1") = 0x01000000;
          register void (**ptr)() asm("r2") = &preempt;
          __asm volatile (
            // TODO: disable interrupts?
            "ldr    r0, [%[ptr]]   \n\t"
            "push   {r0, %[xpsr]}  \n\t"
            "sub    sp, sp, #(6*4) \n\t"
            "bx     lr             \n\t" : : [xpsr] "r" (xpsr), [ptr] "r" (ptr) : "r0"
          );
        }
      };
    };
    
    template <typename Address>
    void (*Scb<Address>::preempt)();
    
    using Scb1 = Scb<Addresses::SCB>;
  }
}

#endif
