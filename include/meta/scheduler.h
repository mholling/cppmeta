#ifndef CPPMETA_META_SCHEDULER_H
#define CPPMETA_META_SCHEDULER_H

#include "meta.h"
#include "list.h"
#include "hfsm.h"
#include "queue.h"

namespace CppMeta {
  namespace Scheduler {
    using Dispatcher = void (*)();
    template <typename Machine> using Dispatchers = Queue::Node<Machine, Dispatcher>;
    
    template <typename Kernel, typename Context, typename Machines, typename Event>
    struct Post {
      template <typename Machine> using PostEvent = typename Dispatchers<Machine>::template Enqueue<HFSM::dispatch<Kernel, Machine, Event>>;
      
      template <typename Machine> using RespondsToEvent = HFSM::RespondsTo<Machine, Event>;
      using RespondingMachines = Select<Machines, RespondsToEvent>;
    
      struct PushContext { void operator()() { Context::push(); } };
      using PushContextNeeded = NotEmpty<RespondingMachines>;
      using PushContextIfNeeded = If<PushContextNeeded, PushContext, DoNothing>;
    
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
        using PreemptingMachines = Before<Machines, Machine>;
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
        using PreemptingMachines = Before<Machines, Machine>;
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
  }
}

// // Example Context driver for Cortex M3:
// 
// struct Context {
//   static void (*preempt)();
//   static void prepare(void (*p)()) { preempt = p; }
//   static void push() { ((SCB_Type *) SCB_BASE)->ICSR = SCB_ICSR_PENDSVSET_Msk; }
//   static void pop() { __asm volatile ("svc 0x01"); }
//   static void enable() { __asm volatile ("cpsie i"); }
//   __attribute__ ((noreturn))
//   static void waitloop() { while (true) ; } // or: { while (true) __asm volatile ("wfi"); }
//   template <typename Kernel> struct Initialise { void operator()() { } };
//   template <typename Kernel, typename Interrupt> struct Handle;
// };
// void (*Context::preempt)();
// 
// template <typename Kernel>
// struct Context::Handle<Kernel, PendSV_IRQ> {
//   __attribute__ ((naked))
//   static void handle() {
//     register uint32_t xpsr      asm("r1") = 0x01000000;
//     register void (**preempt)() asm("r2") = &preempt;
//     __asm volatile (
//       "ldr    r0, [%[preempt]] \n\t"
//       "push   {r0, %[xpsr]}    \n\t"
//       "sub    sp, sp, #(6*4)   \n\t"
//       "bx     lr               \n\t" : : [xpsr] "r" (xpsr), [preempt] "r" (preempt) : "r0"
//     );
//   }
// };
// 
// template <typename Kernel>
// struct Context::Handle<Kernel, SVC_IRQ> {
//   __attribute__ ((naked))
//   static void handle() {
//   __asm volatile (
//     "add    sp, sp, #(8*4) \n\t"
//     "bx     lr             \n\t"
//   );
// };

#endif
