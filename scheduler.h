namespace CppMeta {
  namespace Scheduler {
    template <typename Type>
    struct Node { // TODO: extract Queue/Node into stand-alone template class
      typedef Node *NodePtr;
      Type type;
      NodePtr next;
    
      Node(Type type) : type(type), next(0) { }

      bool enqueue(NodePtr &head) {
        return head == this ? false : head != 0 ? enqueue(head->next) : (head = this, next = 0, true);
      }

      static Node &dequeue(NodePtr &head) {
        Node &result = *head;
        head = head->next;
        result.next = 0;
        return result;
      }
    
      Type operator()() { return type; }
    };
    
    template <typename Context, typename Machines, typename Event> struct Post;
    
    template <typename Context, typename Machines, typename Machine>
    struct Queue {
      typedef Node<void (*const)()> NodeType;
      typedef typename NodeType::NodePtr NodePtr;
      static NodePtr head;
      
      template <typename Event> using Post = Scheduler::Post<Context, Machines, Event>;
    
      template <typename Event>
      struct Dispatch {
        static NodeType dispatch;
        static void enqueue() { dispatch.enqueue(head); }
      };
    
      static bool any() { return head != 0; }
      static void dispatch() { NodeType::dequeue(head)()(); }
      template <typename Event>
      static void enqueue() { Dispatch<Event>::enqueue(); }
    };
    template <typename Context, typename Machines, typename Machine>
    typename Queue<Context, Machines, Machine>::NodePtr Queue<Context, Machines, Machine>::head;
    template <typename Context, typename Machines, typename Machine> template <typename Event>
    typename Queue<Context, Machines, Machine>::NodeType Queue<Context, Machines, Machine>::Dispatch<Event>::dispatch = HFSM::Dispatch<Queue<Context, Machines, Machine>::template Post, Machine, Event>::dispatch;
  
    template <typename Context, typename Machines, typename Event>
    struct Post {
      template <typename Machine> using RespondsToEvent = HFSM::RespondsTo<Machine, Event>;
      typedef typename Select<Machines, RespondsToEvent>::Result RespondingMachines;
      
      template <typename Machine>
      struct PostEvent {
        typedef PostEvent Result;
        void operator()() { Queue<Context, Machines, Machine>::template enqueue<Event>(); }
      };
    
      typedef typename Map<RespondingMachines, PostEvent>::Result PostsToMachines;
    
      struct DoNothing { void operator()() { } };
      struct PushContext { void operator()() { Context::push(); } };
      typedef typename Any<RespondingMachines>::Result PushContextNeeded;
      typedef typename If<PushContextNeeded, PushContext, DoNothing>::Result PushContextIfNeeded;
    
      void operator()() {
        Each<PostsToMachines>()();
        PushContextIfNeeded()();
      }
    };
    
    template <typename Context, typename Machines, typename MachinesToRun>
    struct Run {
      template <typename Machine>
      struct RunMachine {
        typedef RunMachine Result;
        typedef typename UpTo<Machines, Machine>::Result PreemptingMachines;
        void operator()() {
          Context::prepare(Run<Context, Machines, PreemptingMachines>::run);
          while (Queue<Context, Machines, Machine>::any()) { Queue<Context, Machines, Machine>::dispatch(); }
        }
      };
      typedef typename Map<MachinesToRun, RunMachine>::Result MachineRunners;
      static void run() {
        Each<MachineRunners>()();
        Context::prepare(Run<Context, Machines, MachinesToRun>::run);
        Context::pop();
      }
    };
    
    template <typename Context, typename Machines>
    struct Initialise {
      template <typename Event> using Post = Scheduler::Post<Context, Machines, Event>;
      template <typename Machine> using GetInitialiser = HFSM::Initialise<Post, Machine>;
      typedef typename Map<Machines, GetInitialiser>::Result Initialisers;
      void operator()() {
        Context::prepare(Run<Context, Machines, Machines>::run);
        Each<Initialisers>()();
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