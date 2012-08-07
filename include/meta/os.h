#ifndef CPPMETA_META_OS_H
#define CPPMETA_META_OS_H

#include "meta.h"
#include "list.h"
#include "scheduler.h"

namespace CppMeta {
  namespace OS {
    template <typename DriverOrMachine>
    struct HasDependenciesImpl {
      template <typename U> static Bool<true>  test(typename U::Dependencies *);
      template <typename>   static Bool<false> test(...);
      using Result = decltype(test<DriverOrMachine>(0));
    };
    template <typename DriverOrMachine> using HasDependencies = typename HasDependenciesImpl<DriverOrMachine>::Result;
    
    template <typename DriverOrMachine> struct DependenciesImpl { using Result = typename DriverOrMachine::Dependencies; };
    template <typename DriverOrMachine> using Dependencies = If<HasDependencies<DriverOrMachine>, DependenciesImpl<DriverOrMachine>, List<>>;
    
    template <typename DriverOrMachine, typename Driver> using DependsOn = Contains<Dependencies<DriverOrMachine>, Driver>;
    
    template <typename Dependents> struct ExpandDependenciesImpl;
    template <typename Dependents> using ExpandDependencies = typename ExpandDependenciesImpl<Dependents>::Result;
    template <typename Dependents> struct ExpandDependenciesImpl { using Result = Unique<Concat<Flatten<Map<Dependents, Dependencies>>, Dependents>>; };
    
    template <typename Context, typename Machines>
    struct Kernel {
      using DirectDependencies = Unique<Flatten<Map<Machines, Dependencies>>>;
      using Drivers = ExpandDependencies<DirectDependencies>;
      
      template <typename Event>
      struct Post { void operator()() { Scheduler::Post<Kernel, Context, Machines, Event>()(); } };
      
      template <typename Event>
      static void post() { Post<Event>()(); }
      
      template <typename Driver>
      struct DependantsImpl {
        template <typename Candidate> using DependsOnDriver = DependsOn<Candidate, Driver>;
        using Result = Select<Concat<Drivers, Machines>, DependsOnDriver>;
      };
      template <typename Driver> using Dependants = typename DependantsImpl<Driver>::Result;
      
      template <typename Driver>
      struct ConfigurationImpl {
        template <typename U> static Bool<true>  test(typename U::SingleOwner *);
        template <typename>   static Bool<false> test(...);
        using SingleOwner = decltype(test<Driver>(0));
        static_assert(Not<And<SingleOwner, Many<Dependants<Driver>>>>::value, "driver requires single owner");
        
        using DefaultConfiguration = typename Driver::DefaultConfiguration;
        
        template <typename Candidate>
        struct ConfiguresDriverImpl {
          template <typename U> static Bool<true>  test(int(*)[sizeof(typename U::template Configure<Driver, DefaultConfiguration>)]);
          template <typename>   static Bool<false> test(...);
          using Result = decltype(test<Candidate>(0));
        };
        template <typename Candidate> using ConfiguresDriver = typename ConfiguresDriverImpl<Candidate>::Result;
        
        using Configurers = Select<Dependants<Driver>, ConfiguresDriver>;
        
        template <typename Memo, typename Configurer> using AddConfiguration = typename Configurer::template Configure<Driver, Memo>;
        using Result = Inject<Configurers, AddConfiguration, DefaultConfiguration>;
      };
      template <typename Driver> using Configuration = typename ConfigurationImpl<Driver>::Result;
      
      struct Run {
        template <typename Driver> using Initialise = typename Driver::template Initialise<Kernel, Configuration<Driver>>;
        void operator ()() {
          Each<Drivers, Initialise>()();
          Scheduler::Initialise<Kernel, Context, Machines>()();
          Context::waitloop();
        }
      };
      
      static void run() { Run()(); }
      
      template <typename Interrupts>
      struct VectorTableImpl {
        template <typename Interrupt>
        struct HandlerImpl {
          template <typename Driver>
          struct HandlesInterruptImpl {
            template <typename U> static Bool<true>  test(int(*)[sizeof(typename U::template Handle<Kernel, Interrupt>)]);
            template <typename>   static Bool<false> test(...);
            using Result = decltype(test<Driver>(0));
          };
          template <typename Driver> using HandlesInterrupt = typename HandlesInterruptImpl<Driver>::Result;
          using HandlingDrivers = Select<Drivers, HandlesInterrupt>;
          
          template <typename Driver> using GetHandle = typename Driver::template Handle<Kernel, Interrupt>;
          using Handlers = Map<HandlingDrivers, GetHandle>;
          
          template <typename Handler>
          struct UsurpsInterruptImpl {
            template <void (*)()> struct Signature;
            template <typename U> static Bool<true>  test(Signature<U::handle> *);
            template <typename>   static Bool<false> test(...);
            using Result = decltype(test<Handler>(0));
          };
          template <typename Handler> using UsurpsInterrupt = typename UsurpsInterruptImpl<Handler>::Result;
          
          using UsurpingHandlers = Select<Handlers, UsurpsInterrupt>;
          
          template <typename Handler> using SharesInterrupt = HasVoidCallOperator<Handler>;
          using SharingHandlers = Select<Handlers, SharesInterrupt>;
          
          static_assert(Length<UsurpingHandlers>::value < 2, "multiple drivers usurping the same interrupt!");
          static_assert(Empty<UsurpingHandlers>::value || Empty<SharingHandlers>::value, "drivers sharing an interrupt conflict with a driver usurping the same interrupt!");
          
          struct SharedHandler { static void handle() { Each<SharingHandlers>()(); } };
          using Result = First<Append<UsurpingHandlers, SharedHandler>>;
        };
        template <typename Driver> using Handler = typename HandlerImpl<Driver>::Result;
        
        template <typename Table, typename Interrupt>
        struct AddHandlerToTable : Table {
          void (* const vector)();
          constexpr AddHandlerToTable() : Table(), vector(Handler<Interrupt>::handle) { }
        };
        
        struct EmptyTable { };
        using Result = Inject<Interrupts, AddHandlerToTable, EmptyTable>;
      };
      
      template <typename Interrupts> using VectorTable = typename VectorTableImpl<Interrupts>::Result;
    };
  }
}

#endif
