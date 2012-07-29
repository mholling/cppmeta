namespace CppMeta {
  namespace Scheduler {
    typedef void (*Dispatcher)();
    template <typename Machine> using Dispatchers = Queue::Node<Machine, Dispatcher>;
    
    template <typename Kernel, typename Context, typename Machines, typename Event>
    struct Post {
      template <typename Machine>
      struct PostEvent {
        typedef typename Dispatchers<Machine>::template Enqueue<HFSM::dispatch<Kernel, Machine, Event>> Result;
      };
    
      template <typename Machine> using RespondsToEvent = HFSM::RespondsTo<Machine, Event>;
      typedef typename Select<Machines, RespondsToEvent>::Result RespondingMachines;
    
      struct PushContext { void operator()() { Context::push(); } };
      typedef typename Any<RespondingMachines>::Result PushContextNeeded;
      typedef typename If<PushContextNeeded, PushContext, DoNothing>::Result PushContextIfNeeded;
    
      void operator()() {
        Each<RespondingMachines, PostEvent>()();
        PushContextIfNeeded()();
      }
    };
    
    template <typename Context, typename Machines, typename MachinesToRun> void run();
    
    template <typename Context, typename Machines, typename MachinesToRun>
    struct Run {
      template <typename Machine>
      struct RunMachine {
        typedef RunMachine Result;
        typedef typename UpTo<Machines, Machine>::Result PreemptingMachines;
        void operator()() {
          Context::prepare(run<Context, Machines, PreemptingMachines>);
          while (Dispatchers<Machine>::any()) { Dispatchers<Machine>::dequeue()(); }
        }
      };
      void operator()() {
        Each<MachinesToRun, RunMachine>()();
        Context::prepare(run<Context, Machines, MachinesToRun>);
        Context::pop();
      }
    };
    
    template <typename Context, typename Machines, typename MachinesToRun>
    void run() { Run<Context, Machines, MachinesToRun>()(); }
    
    template <typename Kernel, typename Context, typename Machines>
    struct Initialise {
      template <typename Machine>
      struct InitialiseMachine {
        typedef InitialiseMachine Result;
        typedef typename UpTo<Machines, Machine>::Result PreemptingMachines;
        void operator()() {
          Context::prepare(run<Context, Machines, PreemptingMachines>);
          HFSM::Initialise<Kernel, Machine>()();
        }
      };
      void operator()() {
        Context::prepare(run<Context, Machines, List<>>);
        Context::enable();
        Each<Machines, InitialiseMachine>()();
        Context::prepare(run<Context, Machines, Machines>);
        Context::push(); // flush out any events which were fired during initialisation
      }
    };
    
    // // Example Kernel class for Cortex M3:
    //
    // struct Kernel {
    //   static void (*preempt)();
    //   static void prepare_context(void (*p)()) { preempt = p; }
    //   static void push_context() { ((SCB_Type *) SCB_BASE)->ICSR = SCB_ICSR_PENDSVSET_Msk; }
    //   static void pop_context() { __asm volatile ("svc 0x01"); }
    //   static void enable_contexts() { __asm volatile ("cpsie i"); }
    // };
    // void (*Kernel::preempt)();
    // 
    // __attribute__ ((naked)) void handler() { // for PendSV_IRQ
    //   register uint32_t xpsr      asm("r1") = 0x01000000;
    //   register void (**preempt)() asm("r2") = &Kernel::Context::preempt;
    //   __asm volatile (
    //     "ldr    r0, [%[preempt]] \n\t"
    //     "push   {r0, %[xpsr]}    \n\t"
    //     "sub    sp, sp, #(6*4)   \n\t"
    //     "bx     lr               \n\t" : : [xpsr] "r" (xpsr), [preempt] "r" (preempt) : "r0"
    //   );
    // };
    // 
    // __attribute__ ((naked)) void handler() { // for SVC_IRQ
    //   __asm volatile (
    //     "add    sp, sp, #(8*4) \n\t"
    //     "bx     lr             \n\t"
    //   );
    // };
  }
}