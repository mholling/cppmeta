namespace CppMeta {
  namespace Scheduler {
    template <typename Type>
    struct Node {
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
  
    template <typename Machine>
    struct Queue {
      typedef Node<void (*const)()> NodeType;
      typedef typename NodeType::NodePtr NodePtr;
      static NodePtr head;
    
      template <typename Event>
      struct Dispatch {
        static NodeType dispatch;
        static void enqueue() { dispatch.enqueue(head); }
        void operator()() { dispatch(); }
      };
    
      static bool any() { return head != 0; }
      static void dispatch() { NodeType::dequeue(head)()(); }
      template <typename Event>
      static void enqueue() { Dispatch<Event>::enqueue(); }
    };
    template <typename Machine>
    typename Queue<Machine>::NodePtr Queue<Machine>::head;
    template <typename Machine> template <typename Event>
    typename Queue<Machine>::NodeType Queue<Machine>::Dispatch<Event>::dispatch = HFSM::Dispatch<Machine, Event>::dispatch;
  
    template <typename Machines, typename Context, typename MachinesToRun> struct Run {
      template <typename Machine>
      struct RunMachine {
        typedef RunMachine Result;
        typedef typename UpTo<Machines, Machine>::Result PreemptingMachines;
        void operator()() {
          Context::prepare(Run<Machines, Context, PreemptingMachines>::run);
          while (Queue<Machine>::any()) { Queue<Machine>::dispatch(); }
        }
      };
      typedef typename Map<MachinesToRun, RunMachine>::Result MachineRunners;
      static void run() {
        Each<MachineRunners>()();
        Context::prepare(Run<Machines, Context, MachinesToRun>::run);
        Context::pop();
      }
    };
  
    template <typename Machines, typename Context>
    struct Initialise {
      void operator()() {
        Context::prepare(Run<Machines, Context, Machines>::run);
        Each<typename Map<Machines, HFSM::Initialise>::Result>()();
      }
    };
  
    template <typename Machines, typename Context, typename Event>
    struct Post {
      template <typename Machine> using RespondsToEvent = HFSM::RespondsTo<Machine, Event>;
      typedef typename Select<Machines, RespondsToEvent>::Result RespondingMachines;
    
      template <typename Machine>
      struct PostEvent {
        typedef PostEvent Result;
        void operator()() { Queue<Machine>::template enqueue<Event>(); }
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