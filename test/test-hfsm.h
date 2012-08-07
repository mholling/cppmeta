#ifndef CPPMETA_TEST_HFSM_H
#define CPPMETA_TEST_HFSM_H

#include <assert.h>
#include "meta/meta.h"
#include "meta/hfsm.h"

namespace CppMeta {
  namespace Test {
    namespace TestHFSM {
      struct VCR {
        struct PluggedIn;
          struct Active;
            struct Stopped;
            struct Playing;
              struct NormalSpeed;
              struct DoubleSpeed;
              struct QuadSpeed;
              struct HighSpeed;
            struct FastForwarding;
            struct Paused;
          struct Standby;
        
          typedef Tree<Standby> StandbyTree;
            typedef Tree<Stopped> StoppedTree;
              typedef Tree<NormalSpeed> NormalSpeedTree;
              typedef Tree<DoubleSpeed> DoubleSpeedTree;
              typedef Tree<QuadSpeed> QuadSpeedTree;
              typedef Tree<HighSpeed> HighSpeedTree;
            typedef Tree<Playing, NormalSpeedTree, DoubleSpeedTree, QuadSpeedTree, HighSpeedTree> PlayingTree;
            typedef Tree<FastForwarding> FastForwardingTree;
            typedef Tree<Paused> PausedTree;
          typedef Tree<Active, StoppedTree, PlayingTree, FastForwardingTree, PausedTree> ActiveTree;
        typedef Tree<PluggedIn, ActiveTree, StandbyTree> States;
        
        static_assert(Same<HFSM::DefaultPath<States>, List<PluggedIn, Active, Stopped>>::value, "failed");
        static_assert(Same<HFSM::DefaultPath<ActiveTree>, List<Active, Stopped>>::value, "failed");
        static_assert(Same<HFSM::DefaultPath<PlayingTree>, List<Playing, NormalSpeed>>::value, "failed");
        
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
      };
      
      struct EnteringStandby; struct LeavingStandby; struct EnteringPause; struct LeavingPause;
      
      template <typename Kernel> struct VCR::Enter<Kernel, VCR::Standby> { void operator()() { Kernel::template post<EnteringStandby>(); } };
      template <typename Kernel> struct VCR::Exit<Kernel, VCR::Standby> { void operator()() { Kernel::template post<LeavingStandby>(); } };
      
      template <typename Kernel> struct VCR::Enter<Kernel, VCR::Paused> { void operator()() { Kernel::template post<EnteringPause>(); } };
      template <typename Kernel> struct VCR::Exit<Kernel, VCR::Paused> { void operator()() { Kernel::template post<LeavingPause>(); } };
      
      struct PowerButton; struct PlayButton; struct PauseButton; struct ForwardButton; struct StopButton;
      
      struct Activating;
      bool battery_is_flat = false;
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Active, PowerButton, VCR::Standby> { void operator()() { } };
      template <typename Kernel> struct VCR::Guard<Kernel, VCR::Standby, PowerButton, VCR::Active> { bool operator()() { return !battery_is_flat; } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Standby, PowerButton, VCR::Active> { void operator()() { Kernel::template post<Activating>(); } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Active, PlayButton, VCR::Playing> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Playing, PauseButton, VCR::Paused> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Paused, PauseButton, VCR::Playing> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::NormalSpeed, ForwardButton, VCR::DoubleSpeed> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::DoubleSpeed, ForwardButton, VCR::QuadSpeed> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::QuadSpeed, ForwardButton, VCR::HighSpeed> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::HighSpeed, ForwardButton, VCR::DoubleSpeed> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Stopped, ForwardButton, VCR::FastForwarding> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Playing, StopButton, VCR::Stopped> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::FastForwarding, StopButton, VCR::Stopped> { void operator()() { } };
      template <typename Kernel> struct VCR::Action<Kernel, VCR::Paused, StopButton, VCR::Stopped> { void operator()() { } };
      
      static_assert(HFSM::RespondsTo<VCR, PowerButton>::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, PlayButton>::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, PauseButton>::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, ForwardButton>::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, StopButton>::value, "failed");
      static_assert(!HFSM::RespondsTo<VCR, bool>::value, "failed");
      
      struct Kernel { template <typename Event> static void post() { } };
      
      bool entering_standby, leaving_standby, entering_pause, leaving_pause, activating;
      template <> void Kernel::post<EnteringStandby>() { entering_standby = true; }
      template <> void Kernel::post<LeavingStandby>() { leaving_standby = true; }
      template <> void Kernel::post<EnteringPause>() { entering_pause = true; }
      template <> void Kernel::post<LeavingPause>() { leaving_pause = true; }
      template <> void Kernel::post<Activating>() { activating = true; }
      
      struct NullMachine {
        struct S1; struct S11;
        using States = Tree<S1, Tree<S11>>;
      };
      
      void test() {
        bool test;
        
        HFSM::Initialise<Kernel, VCR>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(test = !HFSM::CurrentState<VCR>::Test<VCR::Paused>()());
        
        entering_standby = false;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Standby>()());
        assert(entering_standby);
        
        activating = leaving_standby = false;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(activating && leaving_standby);
        
        HFSM::Dispatch<Kernel, VCR, PlayButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::DoubleSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::QuadSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::HighSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::DoubleSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, PlayButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        entering_pause = false;
        HFSM::Dispatch<Kernel, VCR, PauseButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Paused>()());
        assert(entering_pause);
        
        leaving_pause = false;
        HFSM::Dispatch<Kernel, VCR, PauseButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        assert(leaving_pause);
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        HFSM::Dispatch<Kernel, VCR, PlayButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        entering_standby = false;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Standby>()());
        assert(entering_standby);
        
        leaving_standby = activating = false;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(leaving_standby && activating);
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::FastForwarding>()());
        
        HFSM::Dispatch<Kernel, VCR, StopButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        
        HFSM::Dispatch<Kernel, VCR, PlayButton>()();
        HFSM::Dispatch<Kernel, VCR, StopButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        
        HFSM::Dispatch<Kernel, VCR, ForwardButton>()();
        HFSM::Dispatch<Kernel, VCR, PlayButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Standby>()());
        
        activating = false;
        battery_is_flat = true;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Standby>()());
        assert(!activating);
        
        battery_is_flat = false;
        HFSM::Dispatch<Kernel, VCR, PowerButton>()();
        assert(test = HFSM::CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(activating);
        
        HFSM::Initialise<Kernel, NullMachine>()();
        assert(test = HFSM::CurrentState<NullMachine>::Test<NullMachine::S11>()());
      }
    }
  }
}

#endif
