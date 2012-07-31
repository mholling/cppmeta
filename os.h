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
      struct Configuration {
        using DefaultConfiguration = typename Driver::DefaultConfiguration;
        template <typename DriverOrMachine>
        struct HasConfigureTemplate {
          struct Yes; struct No;
          template <typename U> static Yes& test(typename U::template Configure<Driver, DefaultConfiguration> *);
          template <typename>   static No&  test(...);
          using Result = typename Same<decltype(test<DriverOrMachine>(0)), Yes&>::Result;
        };
        using Candidates = typename Select<typename Concat<Drivers, Machines>::Result, HasConfigureTemplate>::Result;
        template <typename DriverOrMachine> using DependsOnDriver = DependsOn<DriverOrMachine, Driver>;
        using Dependents = typename Select<Candidates, DependsOnDriver>::Result;
        template <typename DriverOrMachine> using ConfiguresDriver = CanEval<typename DriverOrMachine::template Configure<Driver, DefaultConfiguration>>;
        using Configurers = typename Select<Dependents, ConfiguresDriver>::Result;
        template <typename Memo, typename Configurer> using AddConfiguration = typename Configurer::template Configure<Driver, Memo>;
        using Result = typename Inject<Configurers, AddConfiguration, DefaultConfiguration>::Result;
      };
    };
    
    template <typename Kernel, typename Interrupts>
    struct MakeVectorTable {
      template <typename Interrupt>
      struct Handler {
        template <typename Driver>
        struct HasHandleTemplate {
          struct Yes; struct No;
          template <typename U> static Yes& test(typename U::template Handle<Kernel, Interrupt> *);
          template <typename>   static No&  test(...);
          using Result = typename Same<decltype(test<Driver>(0)), Yes&>::Result;
        };
        using CandidateDrivers = typename Select<typename Kernel::Drivers, HasHandleTemplate>::Result;
        template <typename Driver> using HandlesInterrupt = HasVoidCallOperator<typename Driver::template Handle<Kernel, Interrupt>>;
        using HandlingDrivers = typename Select<CandidateDrivers, HandlesInterrupt>::Result;
        template <typename Driver> struct GetHandler { using Result = typename Driver::template Handle<Kernel, Interrupt>; };
        void operator()() { Each<HandlingDrivers, GetHandler>()(); }
      };
      
      template <typename Interrupt>
      static void handler() { Handler<Interrupt>()(); }
      
      template <typename Table, typename Interrupt>
      struct AddHandlerToTable : Table {
        void (* const vector)();
        constexpr AddHandlerToTable() : Table(), vector(handler<Interrupt>) { }
        using Result = AddHandlerToTable;
      };
      
      struct EmptyTable { };
      using Result = typename Inject<Interrupts, AddHandlerToTable, EmptyTable>::Result;
    };
    
    template <typename Kernel, typename Interrupts> using VectorTable = typename MakeVectorTable<Kernel, Interrupts>::Result;
  }
}
