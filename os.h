namespace CppMeta {
  namespace OS {
    template <typename Context, typename RequestedDrivers, typename Machines>
    struct Kernel {
      template <typename Driver> struct GetDependencies { using Result = typename Driver::Dependencies; };
      template <typename DriverList>
      struct ExpandDependencies {
        template <typename Driver> using Expand = Append<typename ExpandDependencies<typename GetDependencies<Driver>::Result>::Result, Driver>;
        using Result = typename Unique<typename Flatten<typename Map<DriverList, Expand>::Result>::Result>::Result;
      };
      using Drivers = typename ExpandDependencies<RequestedDrivers>::Result;
      
      struct Run {
        template <typename Driver> struct Initialise { using Result = typename Driver::template Initialise<Kernel>; };
        void operator ()() {
          Each<Drivers, Initialise>()();
          Scheduler::Initialise<Kernel, Context, Machines>()();
          while(true) ; // TODO;
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
        using DriversAndMachines = typename Concat<typename After<Drivers, Driver>::Result, Machines>::Result;
        template <typename DriverOrMachine> using ConfiguresDriver = CanEval<typename DriverOrMachine::template Configure<Driver, DefaultConfiguration>>;
        using Configurers = typename Select<DriversAndMachines, ConfiguresDriver>::Result;
        template <typename Memo, typename Configurer> using AddConfiguration = typename Configurer::template Configure<Driver, Memo>;
        using Result = typename Inject<Configurers, AddConfiguration, DefaultConfiguration>::Result;
      };
    };
    
    template <typename Kernel, typename Interrupts>
    struct MakeVectorTable {
      template <typename Interrupt>
      struct Handler {
        template <typename Driver> using HandlesInterrupt = HasVoidCallOperator<typename Driver::template Handle<Kernel, Interrupt>>;
        template <typename Driver> struct GetHandler { using Result = typename Driver::template Handle<Kernel, Interrupt>; };
        using HandlingDrivers = typename Select<typename Kernel::Drivers, HandlesInterrupt>::Result;
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
