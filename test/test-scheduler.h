#ifndef CPPMETA_TEST_SCHEDULER_H
#define CPPMETA_TEST_SCHEDULER_H

#include <assert.h>
#include "meta/meta.h"
#include "meta/scheduler.h"

namespace CppMeta {
  namespace Test {
    namespace TestScheduler {
      unsigned int action_order;
      void action(int n) { action_order *= 10; action_order += n; }
      
      struct E1; struct E2; struct E3; struct E4; struct E5;
      
      struct A; struct AA; struct AB; struct AC;
      struct M1{
        using States = Tree<A, Tree<AA>, Tree<AB>, Tree<AC>>;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
      };
      
      template <typename Kernel> struct M1::Action<Kernel, A, E1, AB> { void operator()() { action(1); } };
      template <typename Kernel> struct M1::Action<Kernel, A, E2, AC> { void operator()() { Kernel::template post<E3>(); action(2); } };
      template <typename Kernel> struct M1::Action<Kernel, A, E4, AA> { void operator()() { action(3); } };
      
      struct X; struct XX; struct XY; struct XZ;
      struct M2 {
        using States = Tree<X, Tree<XX>, Tree<XY>, Tree<XZ>>;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
      };
      
      template <typename Kernel> struct M2::Action<Kernel, X, E1, XY> { void operator()() { action(5); } };
      template <typename Kernel> struct M2::Action<Kernel, X, E3, XZ> { void operator()() { action(6); } };
      template <typename Kernel> struct M2::Action<Kernel, X, E5, XX> { void operator()() { Kernel::template post<E4>(); action(7); } };
      
      using Machines = List<M1, M2>;
      
      struct Context {
        static void (*preempt)();
        static void prepare(void (*p)()) { preempt = p; }
        static void push() { preempt(); }
        static void pop() { }
        static void enable() { }
      };
      
      void (*Context::preempt)();
      struct Kernel {
        template <typename Event> static void post() { Scheduler::Post<Kernel, Context, Machines, Event>()(); }
      };
      
      void test() {
        Scheduler::Initialise<Kernel, Context, Machines>()();
        
        action_order = 0;
        Kernel::post<E1>();
        assert(action_order == 15);
        
        action_order = 0;
        Kernel::post<E2>();
        assert(action_order == 26);
        
        action_order = 0;
        Kernel::post<E5>();
        assert(action_order == 37);
      }
    }
  }
}

#endif
