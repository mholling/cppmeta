#ifndef CPPMETA_OS_H
#define CPPMETA_OS_H

#include "meta.h"
#include "list.h"
#include "scheduler.h"

namespace CppMeta {
  namespace OS {
    template <typename DriverOrMachine>
    struct HasDependencies {
      struct Yes; struct No;
      template <typename U> static Yes& test(typename U::Dependencies *);
      template <typename>   static No&  test(...);
      using Result = typename Same<decltype(test<DriverOrMachine>(0)), Yes&>::Result;
    };
    template <typename DriverOrMachine> struct GetDependenciesUnsafely { using Result = typename DriverOrMachine::Dependencies; };
    template <typename DriverOrMachine> using GetDependencies = If<typename HasDependencies<DriverOrMachine>::Result, GetDependenciesUnsafely<DriverOrMachine>, List<>>;
    
    template <typename DriverOrMachine, typename Driver> using DependsOn = Contains<typename GetDependencies<DriverOrMachine>::Result, Driver>;
    
    template <typename Dependents>
    struct ExpandDependencies {
      template <typename Dependent> using Expand = Append<typename ExpandDependencies<typename GetDependencies<Dependent>::Result>::Result, Dependent>;
      using Result = typename Unique<typename Flatten<typename Map<Dependents, Expand>::Result>::Result>::Result;
    };
    
    template <typename Context, typename Machines>
    struct Kernel {
      using DirectDependencies = typename Unique<typename Flatten<typename Map<Machines, GetDependencies>::Result>::Result>::Result;
      using Drivers = typename ExpandDependencies<DirectDependencies>::Result;
      
      struct Run {
        template <typename Driver> struct Initialise { using Result = typename Driver::template Initialise<Kernel>; };
        void operator ()() {
          Each<Drivers, Initialise>()();
          Scheduler::Initialise<Kernel, Context, Machines>()();
          Context::waitloop();
        }
      };
      
      static void run() { Run()(); }
      
      template <typename Event>
      struct Post {
        using Result = Post;
        void operator()() { Scheduler::Post<Kernel, Context, Machines, Event>()(); }
      };
      
      template <typename Event>
      static void post() { Post<Event>()(); }
      
      template <typename Driver>
      struct GetConfiguration {
        template <typename Candidate> using DependsOnDriver = DependsOn<Candidate, Driver>;
        using Candidates = typename Select<typename Concat<Drivers, Machines>::Result, DependsOnDriver>::Result;
        using DefaultConfiguration = typename Driver::DefaultConfiguration;
        template <typename Candidate>
        struct ConfiguresDriver {
          struct Yes; struct No;
          template <typename U> static Yes& test(int(*)[sizeof(typename U::template Configure<Driver, DefaultConfiguration>)]);
          template <typename>   static No&  test(...);
          using Result = typename Same<decltype(test<Candidate>(0)), Yes&>::Result;
        };
        using Configurers = typename Select<Candidates, ConfiguresDriver>::Result;
        template <typename Memo, typename Configurer> struct AddConfiguration { using Result = typename Configurer::template Configure<Driver, Memo>; };
        using Result = typename Inject<Configurers, AddConfiguration, DefaultConfiguration>::Result;
      };
      
      template <typename Driver> using Configuration = typename GetConfiguration<Driver>::Result;
      
      template <typename Interrupts>
      struct GetVectorTable {
        template <typename Interrupt>
        struct GetHandler {
          template <typename Driver>
          struct HandlesInterrupt {
            struct Yes; struct No;
            template <typename U> static Yes& test(int(*)[sizeof(typename U::template Handle<Kernel, Interrupt>)]);
            template <typename>   static No&  test(...);
            using Result = typename Same<decltype(test<Driver>(0)), Yes&>::Result;
          };
          using HandlingDrivers = typename Select<Drivers, HandlesInterrupt>::Result;
          template <typename Driver> struct GetHandle { using Result = typename Driver::template Handle<Kernel, Interrupt>; };
          using Handlers = typename Map<HandlingDrivers, GetHandle>::Result;
          
          template <typename Handler>
          struct UsurpsInterrupt {
            struct Yes; struct No;
            template <void (*)()> struct Signature;
            template <typename U> static Yes& test(Signature<U::handle> *);
            template <typename>   static No&  test(...);
            using Result = typename Same<decltype(test<Handler>(0)), Yes&>::Result;
          };
          using UsurpingHandlers = typename Select<Handlers, UsurpsInterrupt>::Result;
          
          template <typename Handler> using SharesInterrupt = HasVoidCallOperator<Handler>;
          using SharingHandlers = typename Select<Handlers, SharesInterrupt>::Result;
          
          static_assert(Length<UsurpingHandlers>::Result::value < 2, "multiple drivers usurping the same interrupt!");
          static_assert(Empty<UsurpingHandlers>::Result::value || Empty<SharingHandlers>::Result::value, "drivers sharing an interrupt conflict with a driver usurping the same interrupt!");
          
          struct SharedHandler { static void handle() { Each<SharingHandlers>()(); } };
          using Result = typename First<typename Append<UsurpingHandlers, SharedHandler>::Result>::Result;
        };
        
        template <typename Table, typename Interrupt>
        struct AddHandlerToTable : Table {
          using Result = AddHandlerToTable;
          using Handler = typename GetHandler<Interrupt>::Result;
          void (* const vector)();
          constexpr AddHandlerToTable() : Table(), vector(Handler::handle) { }
        };
        
        struct EmptyTable { };
        using Result = typename Inject<Interrupts, AddHandlerToTable, EmptyTable>::Result;
      };
      
      template <typename Interrupts> using VectorTable = typename GetVectorTable<Interrupts>::Result;
    };
  }
}

#endif
