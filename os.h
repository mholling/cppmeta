namespace CppMeta {
  namespace OS {
    template <typename _Context, typename RequestedDrivers, typename Machines>
    struct Kernel {
      typedef _Context Context;
      
      template <typename Driver> struct GetDependencies { typedef typename Driver::Dependencies Result; };
      template <typename DriverList>
      struct ExpandDependencies {
        template <typename Driver> using Expand = Append<typename ExpandDependencies<typename GetDependencies<Driver>::Result>::Result, Driver>;
        typedef typename Unique<typename Flatten<typename Map<DriverList, Expand>::Result>::Result>::Result Result;
      };
      typedef typename ExpandDependencies<RequestedDrivers>::Result Drivers;
      
      struct Run {
        template <typename Driver> struct Initialise { typedef typename Driver::template Initialise<Kernel> Result; };
        void operator ()() {
          Each<Drivers, Initialise>()();
          Scheduler::Initialise<Kernel, Machines>()();
          while(true) ; // TODO;
        }
      };
      
      static void run() { Run()(); }
      
      template <typename Event>
      struct Post {
        typedef Post Result;
        void operator()() { Scheduler::Post<Context, Machines, Event>()(); }
      };
      
      template <typename Event>
      static void post() { Post<Event>()(); }
    };
    
    template <typename Kernel, typename Interrupts>
    struct MakeVectorTable {
      template <typename Interrupt>
      struct Handler {
        template <typename Driver> using HandlesInterrupt = HasVoidCallOperator<typename Driver::template Handle<Kernel, Interrupt>>;
        template <typename Driver> struct GetHandler { typedef typename Driver::template Handle<Kernel, Interrupt> Result; };
        typedef typename Select<typename Kernel::Drivers, HandlesInterrupt>::Result HandlingDrivers;
        void operator()() { Each<HandlingDrivers, GetHandler>()(); }
      };
      
      template <typename Interrupt>
      static void handler() { Handler<Interrupt>()(); }
      
      template <typename Table, typename Interrupt>
      struct AddHandlerToTable : Table {
        void (* const vector)();
        constexpr AddHandlerToTable() : Table(), vector(handler<Interrupt>) { }
        typedef AddHandlerToTable Result;
      };
      
      struct EmptyTable { };
      typedef typename Inject<Interrupts, AddHandlerToTable, EmptyTable>::Result Result;
    };
    
    template <typename Kernel, typename Interrupts> using VectorTable = typename MakeVectorTable<Kernel, Interrupts>::Result;
  }
}
