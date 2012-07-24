namespace CppMeta {
  namespace OS {
    template <typename _Context, typename Drivers, typename Machines>
    struct Kernel {
      typedef _Context Context;
      
      template <typename Driver> struct GetDependencies { typedef typename Driver::Dependencies Result; };
      template <typename DriverList>
      struct ExpandDependencies {
        template <typename Driver> using Expand = Append<typename ExpandDependencies<typename GetDependencies<Driver>::Result>::Result, Driver>;
        typedef typename Unique<typename Flatten<typename Map<DriverList, Expand>::Result>::Result>::Result Result;
      };
      typedef typename ExpandDependencies<Drivers>::Result RequiredDrivers;
      
      struct Run {
        template <typename Driver> using HasInitialiser = HasVoidCallOperator<typename Driver::Initialise>;
        template <typename Driver> struct Initialise { typedef typename Driver::Initialise Result; };
        typedef typename Select<RequiredDrivers, HasInitialiser>::Result DriversWithInitialiser;
        void operator ()() {
          Each<DriversWithInitialiser, Initialise>()();
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
  }
}