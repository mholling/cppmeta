#ifndef CPPMETA_CORTEX_NVIC_H
#define CPPMETA_CORTEX_NVIC_H

#include <stdint.h>
#include "meta/value.h"
#include "meta/meta.h"
#include "meta/list.h"
#include "meta/os.h"
#include "cortex/registers.h"
#include "cortex/addresses.h"
#include "cortex/scb.h"

extern "C" int __libc_init_array(void);
extern unsigned __data_start;       /* start of .data in the linker script */
extern unsigned __data_end;         /* end of .data in the linker script   */
extern unsigned const __data_load;  /* initialization values for .data     */
extern unsigned __bss_start;        /* start of .bss in the linker script  */
extern unsigned __bss_end;          /* end of .bss in the linker script    */

namespace CppMeta {
  namespace Cortex {
    struct NVIC_Type {
      volatile uint32_t ISER[8];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
               uint32_t RESERVED0[24];
      volatile uint32_t ICER[8];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register         */
               uint32_t RESERVED1[24];
      volatile uint32_t ISPR[8];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register          */
               uint32_t RESERVED2[24];
      volatile uint32_t ICPR[8];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register        */
               uint32_t RESERVED3[24];
      volatile uint32_t IABR[8];                 /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register           */
               uint32_t RESERVED4[56];
      volatile uint8_t  IP[240];                 /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
               uint32_t RESERVED5[644];
      volatile uint32_t STIR;                    /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register     */
    };
    
    struct Nvic : Registers<NVIC_Type, Addresses::NVIC> {
      using STIR_INTID = UInt32<0x1FF>;
      using Dependencies = List<Scb>; // TODO?
      
      struct DefaultConfiguration {
        using PriorityBits = Int<4>;
      };
      
      template <typename Irq> using GetIndex = RightShift<Irq, Int<5>>;
      template <typename Irq> using GetBit = LeftShift<Int<1>, BitwiseAnd<Irq, Int<0x1f>>>;
      
      using CoreIrqs = Sequence<Int<-12>, Int<-1>>;
      using PeripheralIrqs = Sequence<Int<0>, Int<239>>;
      using PrioritisableIrqs = Concat<CoreIrqs, PeripheralIrqs>;
      
      template <typename Irq> using IsCore = Contains<CoreIrqs, Irq>;
      template <typename Irq> using IsPeripheral = Contains<PeripheralIrqs, Irq>;
      template <typename Irq> using IsPrioritisable = Contains<PrioritisableIrqs, Irq>;
      
      template <volatile uint32_t (NVIC_Type::*reg)[8], typename Irqs>
      struct WriteAll {
        static_assert(All<Irqs, IsPeripheral>::value, "only allowed for peripheral interrupts");
        template <typename Index>
        struct WriteValue {
          template <typename Memo, typename Irq> using AddToAggregate = If<Same<Index, GetIndex<Irq>>, BitwiseOr<Memo, GetBit<Irq>>, Memo>;
          using Aggregate = Inject<Irqs, AddToAggregate, Int<0>>;
          void operator()() { (Nvic::registers.*reg)[Index::value] = Aggregate::value; }
        };
        template <typename Index> using NeedsWriting = Contains<Map<Irqs, GetIndex>, Index>;
        using Indices = Select<Sequence<Int<0>, Int<7>>, NeedsWriting>;
        void operator()() { Each<Indices, WriteValue>()(); }
      };
      
      template <volatile uint32_t (NVIC_Type::*reg)[8], typename Irq>
      struct WriteOne {
        static_assert(IsPeripheral<Irq>::value, "only allowed for peripheral interrupts");
        void operator()() { (Nvic::registers.*reg)[GetIndex<Irq>::value] = GetBit<Irq>::value; }
      };
      
      template <volatile uint32_t (NVIC_Type::*reg)[8], typename Irq>
      struct ReadOne {
        static_assert(IsPeripheral<Irq>::value, "only allowed for peripheral interrupts");
        bool operator()() { return ((Nvic::registers.*reg)[GetIndex<Irq>::value] & GetBit<Irq>::value) != 0; }
      };
      
      template <typename Irqs> using EnableMany = WriteAll<&NVIC_Type::ISER, Irqs>;
      template <typename Irq> using Enable = WriteOne<&NVIC_Type::ISER, Irq>;
      template <typename Irq> using Disable = WriteOne<&NVIC_Type::ICER, Irq>;
      template <typename Irq> using SetPending = WriteOne<&NVIC_Type::ISPR, Irq>;
      template <typename Irq> using ClearPending = WriteOne<&NVIC_Type::ICPR, Irq>;
      template <typename Irq> using Active = ReadOne<&NVIC_Type::IABR, Irq>;
      
      template <typename Irq> static void enable() { Enable<Irq>()(); }
      template <typename Irq> static void disable() { Disable<Irq>()(); }
      template <typename Irq> static void set_pending() { SetPending<Irq>()(); }
      template <typename Irq> static void clear_pending() { ClearPending<Irq>()(); }
      template <typename Irq> static bool active() { return Active<Irq>()(); }
      
      template <typename IrqGroups, typename PriorityBits>
      struct SetPriorities {
        static_assert(Not<Contains<Flatten<IrqGroups>, Irqs::PendSV>>::value, "setting PendSV interrupt priority not allowed");
        static_assert(Not<Contains<Flatten<IrqGroups>, Irqs::SVCall>>::value, "setting SVCall interrupt priority not allowed");
        static_assert(All<Flatten<IrqGroups>, IsPrioritisable>::value, "not a valid prioritisable interrupt");
        
        using Groups = If<Empty<IrqGroups>, List<List<List<>>>, IrqGroups>;
        using GroupBits = HighBitPosition<Decrement<LeftShift<Length<Groups>, Int<1>>>>;
        static_assert(GreaterThanOrEqualTo<PriorityBits, GroupBits>::value, "too many interrupt priority groups");
        using SubGroupBits = Minus<PriorityBits, GroupBits>;
        using PriGroup = Minus<Int<7>, GroupBits>;
        using UnusedBits = Minus<Int<8>, PriorityBits>;
        
        template <typename IrqSubGroups, typename GroupPriority>
        struct SetGroupPriority {
          static_assert(LessThanOrEqualTo<Length<IrqSubGroups>, PowerOfTwo<SubGroupBits>>::value, "too many interrupt priority subgroups");
          template <typename IrqSubGroup, typename SubGroupPriority>
          struct SetSubPriority {
            using GroupAndSubGroup = BitwiseOr<LeftShift<GroupPriority, SubGroupBits>, SubGroupPriority>;
            using Priority = LeftShift<GroupAndSubGroup, UnusedBits>;
            static_assert(LessThan<GroupAndSubGroup, Decrement<PowerOfTwo<PriorityBits>>>::value, "too many interrupt priority groups and/or subgroups");
            
            template <typename Irq> struct SetCore { void operator()() { Scb::registers.SHP[Plus<Int<12>, Irq>::value] = Priority::value; } };
            template <typename Irq> struct SetPeripheral { void operator()() { Nvic::registers.IP[Irq::value] = Priority::value; } };
            template <typename Irq> using SetPriority = If<IsCore<Irq>, SetCore<Irq>, SetPeripheral<Irq>>;
            void operator()() { Each<IrqSubGroup, SetPriority>()(); }
          };
          void operator()() { Each<MapWithIndex<IrqSubGroups, SetSubPriority>>()(); }
        };

        void operator()() {
          for (int n = 0; n < 240; ++n) Nvic::registers.IP[n] = 0xff;
          for (int n = 0; n < 12; ++n) Scb::registers.SHP[n] = 0xff;
          Scb::template bitfield<&SCB_Type::AIRCR, Scb::AIRCR_PRIGROUP>() = PriGroup::value;
          Each<MapWithIndex<IrqGroups, SetGroupPriority>>()();
        }
      };
      
      template <typename Kernel>
      struct Initialise {
        using Drivers = typename Kernel::Drivers;
        using Config = typename Kernel::template Configuration<Nvic>;
        using PriorityBits = typename Config::PriorityBits;
        
        template <typename Irq>
        struct IsHandledImpl {
          template <typename Driver> using HandlesIrq = OS::template Handles<Kernel, Driver, Irq>;
          using Result = Any<Drivers, HandlesIrq>;
        };
        template <typename Irq> using IsHandled = Eval<IsHandledImpl<Irq>>;
        using HandledPeripheralIrqs = Select<PeripheralIrqs, IsHandled>;
        
        template <typename Driver>
        struct HasPrioritisedIrqsImpl {
          template <typename U> static Bool<true>  test(typename U::PrioritisedIrqs *);
          template <typename>   static Bool<false> test(...);
          using Result = decltype(test<Driver>(0));
        };
        template <typename Driver> using HasPrioritisedIrqs = Eval<HasPrioritisedIrqsImpl<Driver>>;
        template <typename Driver> struct GetPrioritisedIrqsImpl { using Result = typename Driver::PrioritisedIrqs; };
        
        template <typename Target, typename Source> struct NestedAppendImpl;
        template <typename Target, typename Source> using NestedAppend = Eval<NestedAppendImpl<Target, Source>>;
        template <typename Target, typename... Types>
        struct NestedAppendImpl<Target, List<List<Types...>>> { using Result = Replace<Target, Last<Target>, NestedAppend<Last<Target>, List<Types...>>>; };
        template <typename Target, typename... Types>
        struct NestedAppendImpl<Target, List<Types...>> { using Result = Concat<Target, List<Types...>>; };
        
        template <typename Target, typename Exclude = List<>> struct NestedUniqueImpl;
        template <typename Target, typename Exclude = List<>> using NestedUnique = Eval<NestedUniqueImpl<Target, Exclude>>;
        template <typename Target, typename Exclude>
        struct NestedUniqueImpl {
          template <typename Memo, typename Type>
          struct AddUniqueImpl { using Result = If<Contains<Concat<Exclude, Flatten<Memo>>, Type>, Memo, Append<Memo, Type>>; };
          template <typename Memo, typename Type> using AddUnique = Eval<AddUniqueImpl<Memo, Type>>;
          template <typename Memo, typename... Types>
          struct AddUniqueImpl<Memo, List<Types...>> {
            using AppliedToNested = NestedUnique<List<Types...>, Concat<Exclude, Flatten<Memo>>>;
            using Result = If<Empty<AppliedToNested>, Memo, Append<Memo, AppliedToNested>>;
          };
          using Result = Inject<Target, AddUnique, List<>>;
        };
        
        template <typename Memo, typename Driver>
        struct AddPrioritiesImpl {
          template <typename Irq> using HandledByDriver = OS::template Handles<Kernel, Driver, Irq>;
          using DefaultPrioritisedIrqs = List<List<Select<PrioritisableIrqs, HandledByDriver>>>;
          using DriverPrioritisedIrqs = If<HasPrioritisedIrqs<Driver>, GetPrioritisedIrqsImpl<Driver>, DefaultPrioritisedIrqs>;
          
          static_assert(SameElements<Flatten<DefaultPrioritisedIrqs>, Flatten<DriverPrioritisedIrqs>>::value, "nested interrupt priority list should include all interrupts handled by the driver");
          template <typename List> using IsNested = All<List, IsList>;
          template <typename List> using IsDoublyNested = All<List, IsNested>;
          template <typename List> using IsIrqList = All<List, IsPrioritisable>;
          template <typename List> using IsNestedIrqList = All<List, IsIrqList>;
          template <typename List> using IsDoublyNestedIrqList = All<List, IsNestedIrqList>;
          static_assert(IsList<DriverPrioritisedIrqs>::value, "invalid nested interrupt priority list");
          static_assert(IsNested<DriverPrioritisedIrqs>::value, "invalid nested interrupt priority list");
          static_assert(IsDoublyNested<DriverPrioritisedIrqs>::value, "invalid nested interrupt priority list");
          static_assert(IsDoublyNestedIrqList<DriverPrioritisedIrqs>::value, "invalid nested interrupt priority list");
          
          using Result = NestedAppend<Memo, DriverPrioritisedIrqs>;
        };
        template <typename Memo, typename Driver> using AddPriorities = Eval<AddPrioritiesImpl<Memo, Driver>>;
        
        using DriversExceptScb = Exclude<Drivers, List<Scb>>;
        using DriverPrioritisedIrqs = NestedUnique<Inject<DriversExceptScb, AddPriorities, List<List<List<>>>>>;
        
        void operator()() {
          // If<Empty<DriverPrioritisedIrqs>, DoNothing, SetPriorities<DriverPrioritisedIrqs, PriorityBits>>()();
          SetPriorities<DriverPrioritisedIrqs, PriorityBits>()();
          EnableMany<HandledPeripheralIrqs>()();
        }
      };
      
      template <typename Kernel, typename Interrupt> struct Handle;
      template <typename Kernel>
      struct Handle<Kernel, Irqs::Reset> {
        static void handle() {
          unsigned const *src = &__data_load;
          for (unsigned *dst = &__data_start; dst < &__data_end; ++dst, ++src)
            *dst = *src;
          for (unsigned *dst = &__bss_start; dst < &__bss_end; ++dst)
            *dst = 0;
          __libc_init_array();
          
          Kernel::run();
        }
      };
    };
  }
}

#endif
