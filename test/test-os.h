#ifndef CPPMETA_TEST_OS_H
#define CPPMETA_TEST_OS_H

#include <assert.h>
#include <cstdio>
#include "meta/meta.h"
#include "meta/os.h"

namespace CppMeta {
  namespace Test {
    namespace TestOS {
      unsigned int init_order;
      void init(int n) { init_order *= 10; init_order += n; }
      
      struct Context {
        static void (*preempt)();
        static void prepare(void (*p)()) { preempt = p; }
        static void push() { preempt(); }
        static void pop() { }
        static void enable() { }
        static void waitloop() { }
      };
      void (*Context::preempt)();
      
      struct Irq1; struct Irq2; struct Irq3;
      bool d1_irq3_called, d2_irq2_called, d2_irq3_called, d3_irq1_called;
      
      struct D1 {
        template <typename Kernel, typename Interrupt> struct Handle;
        struct DefaultConfiguration { static constexpr bool defaultflag = true; static constexpr int id = 1; };
        template <typename Kernel> struct Initialise { void operator()() { init(Kernel::template Configuration<D1>::id); } };
      };
      template <typename Kernel>
      struct D1::Handle<Kernel, Irq3> { void operator()() { d1_irq3_called = true; } };
      
      struct D2 {
        struct SingleOwner;
        template <typename Kernel, typename Interrupt> struct Handle;
        using Dependencies = List<D1>;
        struct DefaultConfiguration { static constexpr int id = 2; };
        template <typename Kernel> struct Initialise { void operator()() { init(Kernel::template Configuration<D2>::id); } };
      };
      template <typename Kernel>
      struct D2::Handle<Kernel, Irq2> { static void handle() { d2_irq2_called = true; } };
      template <typename Kernel>
      struct D2::Handle<Kernel, Irq3> { void operator()() { d2_irq3_called = true; } };
      
      struct D3 {
        template <typename Kernel, typename Interrupt> struct Handle;
        using Dependencies = List<D1>;
        struct DefaultConfiguration { static constexpr int bitfield = 1; static constexpr int id = 3; };
        template <typename Kernel> struct Initialise { void operator()() { init(Kernel::template Configuration<D3>::id); } };
      };
      template <typename Kernel>
      struct D3::Handle<Kernel, Irq1> { void operator()() { d3_irq1_called = true; } };
      
      struct D4 {
        using Dependencies = List<D2>;
        struct DefaultConfiguration { using Types = List<int>; static constexpr int id = 4; };
        template <typename Kernel> struct Initialise { void operator()() { init(Kernel::template Configuration<D4>::id); } };
      };
      
      struct M1 {
        struct S;
        using States = Tree<S>;
        using Dependencies = List<D3, D1>;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
        template <typename, typename Config> struct Configure;
      };
      template <typename Config> struct M1::Configure<D3, Config> : Config { static constexpr int bitfield = Config::bitfield | 2; };
      
      struct M2 {
        struct S;
        using States = Tree<S>;
        using Dependencies = List<D4>;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
      };
      
      struct M3 {
        struct S;
        using States = Tree<S>;
        using Dependencies = List<D3, D4>;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
        template <typename, typename Config> struct Configure;
      };
      template <typename Config> struct M3::Configure<D3, Config> : Config { static constexpr int bitfield = Config::bitfield | 4; };
      template <typename Config> struct M3::Configure<D4, Config> : Config { static constexpr bool m3flag = true; using Types = Append<typename Config::Types, bool>; };
      
      using Machines = List<M1, M2, M3>;
      using Kernel = OS::Kernel<Context, Machines>;
      
      static_assert(Same<Kernel::Drivers, List<D1, D3, D2, D4>>::value, "failed");
      
      static_assert(Same<Kernel::Dependants<D1>, List<D3, D2, M1>>::value, "failed");
      static_assert(Same<Kernel::Dependants<D2>, List<D4>>::value, "failed");
      static_assert(Same<Kernel::Dependants<D3>, List<M1, M3>>::value, "failed");
      static_assert(Same<Kernel::Dependants<D4>, List<M2, M3>>::value, "failed");
      
      using D1Config = Kernel::Configuration<D1>;
      using D3Config = Kernel::Configuration<D3>;
      using D4Config = Kernel::Configuration<D4>;
      
      static_assert(D1Config::defaultflag == true, "failed");
      static_assert(D3Config::bitfield == 7, "failed");
      static_assert(D4Config::m3flag == true, "failed");
      static_assert(Same<D4Config::Types, List<int, bool>>::value, "failed");
      
      using Interrupts = List<Irq1, Irq2, Irq3>;
      using VectorTable = OS::VectorTable<Kernel, Interrupts>;
      
      VectorTable vector_table;
      void (** vectors)() = reinterpret_cast<void (**)()>(&vector_table);

      void test() {
        d3_irq1_called = false; d1_irq3_called = false; d3_irq1_called = false; d2_irq2_called = false;
        vectors[0]();
        assert(d3_irq1_called && !d2_irq2_called && !d1_irq3_called && !d2_irq3_called);
        
        d3_irq1_called = false; d1_irq3_called = false; d3_irq1_called = false; d2_irq2_called = false;
        vectors[1]();
        assert(!d3_irq1_called && d2_irq2_called && !d1_irq3_called && !d2_irq3_called);
        
        d3_irq1_called = false; d1_irq3_called = false; d3_irq1_called = false; d2_irq2_called = false;
        vectors[2]();
        assert(!d3_irq1_called && !d2_irq2_called && d1_irq3_called && d2_irq3_called);
        
        init_order = 0;
        Kernel::run();
        assert(init_order == 1324);
      }
    }
  }
}

#endif
