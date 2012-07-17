namespace CppMeta {
  namespace Scheduler {
    typedef void (*Dispatcher)();
    template <typename Context, typename Machines, typename Machine> using Dispatchers = Queue::Head<Dispatcher, Context, Machines, Machine>;
    
    template <typename Context, typename Machines, typename Event>
    struct Post {
      template <typename OtherEvent> using Poster = Post<Context, Machines, OtherEvent>;
      
      template <typename Machine>
      struct PostEvent {
        typedef Dispatchers<Context, Machines, Machine> Events;
        typedef typename Events::template Enqueue<HFSM::dispatch<Poster, Machine, Event>> Result;
      };
    
      template <typename Machine> using RespondsToEvent = HFSM::RespondsTo<Machine, Event>;
      typedef typename Select<Machines, RespondsToEvent>::Result RespondingMachines;
    
      struct DoNothing { void operator()() { } };
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
          while (Dispatchers<Context, Machines, Machine>::any()) { Dispatchers<Context, Machines, Machine>::dequeue()(); }
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
    
    template <typename Context, typename Machines>
    struct Initialise {
      template <typename Event> using Post = Scheduler::Post<Context, Machines, Event>;
      template <typename Machine> using RunInitialiser = HFSM::Initialise<Post, Machine>;
      void operator()() {
        Context::prepare(run<Context, Machines, Machines>);
        Each<Machines, RunInitialiser>()();
      }
    };
    
    // // Example Context class for Cortex M3:
    //
    // struct Context {
    //   static void (*preempt);
    //   static void prepare(void (*p)) { preempt = p; }
    //   static void push() { ((SCB_Type *) SCB_BASE)->ICSR = SCB_ICSR_PENDSVSET_Msk; }
    //   static void pop() { __asm volatile ("svc 0x01"); }
    // };
    // void (*Context::preempt)();
    // 
    // __attribute__ ((naked)) void handler() { // for PendSV_IRQ
    //   register uint32_t xpsr      asm("r1") = 0x01000000;
    //   register void (**preempt)() asm("r2") = &Context::preempt;
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