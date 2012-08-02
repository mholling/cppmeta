#ifndef CPPMETA_HFSM_H
#define CPPMETA_HFSM_H

#include "meta.h"
#include "list.h"
#include "tree.h"

namespace CppMeta {
  namespace HFSM {
    template <typename Machine>
    struct CurrentState {
      static int index;
      template <typename State>
      struct Test { bool operator()() { return Index<typename Machine::States, State>::Result::value == index; } };
      template <typename State>
      struct Set { void operator()() { index = Index<typename Machine::States, State>::Result::value; } };
    };
    template <typename Machine>
    int CurrentState<Machine>::index = 0;
    
    template <typename States> struct DefaultPath;
    template <typename State, typename FirstSubstate, typename... OtherSubstates>
    struct DefaultPath<Tree<State, FirstSubstate, OtherSubstates...>> {
      using Result = typename Prepend<State, typename DefaultPath<FirstSubstate>::Result>::Result;
    };
    template <typename State>
    struct DefaultPath<Tree<State>> { using Result = List<State>; };
    
    template <typename Kernel, typename Machine, typename Substates>
    struct ExitStates {
      template <typename Candidate>
      struct ExitIfCurrent {
        template <typename State>
        struct HasExit {
          struct Yes; struct No;
          template <typename U, void (U::*)()> struct Signature;
          template <typename U> static Yes& test(Signature<typename U::template Exit<Kernel, State>, &U::template Exit<Kernel, State>::operator()> *);
          template <typename>   static No&  test(...);
          using Result = typename Same<decltype(test<Machine>(0)), Yes&>::Result;
        };
        template <typename State> struct Exit { using Result = Exit; void operator()() { typename Machine::template Exit<Kernel, State>()(); } };
        using ExitPath = typename SelfAndAncestors<Substates, Candidate>::Result;
        using Result = ExitIfCurrent;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<Candidate>()()) return false;
          Each<typename Select<ExitPath, HasExit>::Result, Exit>()();
          return true;
        }
      };
      using LeafStates = typename Leaves<Substates>::Result;
      using ExitFromCurrentState = typename Map<LeafStates, ExitIfCurrent>::Result;
      bool operator()() { return Until<ExitFromCurrentState>()(); }
    };
    
    template <typename Kernel, typename Machine, typename Substates = typename Machine::States, typename Target = typename Root<Substates>::Result>
    struct EnterStates {
        template <typename State>
        struct HasEntry {
          struct Yes; struct No;
          template <typename U, void (U::*)()> struct Signature;
          template <typename U> static Yes& test(Signature<typename U::template Enter<Kernel, State>, &U::template Enter<Kernel, State>::operator()> *);
          template <typename>   static No&  test(...);
          using Result = typename Same<decltype(test<Machine>(0)), Yes&>::Result;
        };
      template <typename State> struct Enter { using Result = Enter; void operator()() { typename Machine::template Enter<Kernel, State>()(); } };
      using TargetEntryPath = typename Reverse<typename Ancestors<Substates, Target>::Result>::Result;
      using DefaultEntryPath = typename DefaultPath<typename FindBranch<Substates, Target>::Result>::Result;
      using EntryPath = typename Concat<TargetEntryPath, DefaultEntryPath>::Result;
      using FinalState = typename Last<EntryPath>::Result;
      void operator()() {
        Each<typename Select<EntryPath, HasEntry>::Result, Enter>()();
        typename CurrentState<Machine>::template Set<FinalState>()();
      }
    };
    
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
    struct HasGuard {
      struct Yes; struct No;
      template <typename U, bool (U::*)()> struct Signature;
      template <typename U> static Yes& test(Signature<typename U::template Guard<Kernel, State, Event, Target>, &U::template Guard<Kernel, State, Event, Target>::operator()> *);
      template <typename>   static No&  test(...);
      using Result = typename Same<decltype(test<Machine>(0)), Yes&>::Result;
    };
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
    struct HasAction {
      struct Yes; struct No;
      template <typename U, void (U::*)()> struct Signature;
      template <typename U> static Yes& test(Signature<typename U::template Action<Kernel, State, Event, Target>, &U::template Action<Kernel, State, Event, Target>::operator()> *);
      template <typename>   static No&  test(...);
      using Result = typename Same<decltype(test<Machine>(0)), Yes&>::Result;
    };
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasTransition = Or<typename HasGuard<Kernel, Machine, State, Event, Target>::Result, typename HasAction<Kernel, Machine, State, Event, Target>::Result>;
    
    struct EmptyAction { void operator()() { } };
    struct EmptyGuard { bool operator()() { return true; } };
    
    template <typename Kernel, typename Machine, typename Source, typename Event, typename Target>
    struct ChangeState {
      using Substates = typename CommonBranch<typename Machine::States, Source, Target>::Result;
      using ActionExists = typename HasAction<Kernel, Machine, Source, Event, Target>::Result;
      using Action = typename If<ActionExists, typename Machine::template Action<Kernel, Source, Event, Target>, EmptyAction>::Result;
      bool operator()() {
        ExitStates<Kernel, Machine, Substates>()();
        Action()();
        EnterStates<Kernel, Machine, Substates, Target>()();
        return true;
      }
    };
    
    template <typename Kernel, typename Machine, typename Event>
    struct Dispatch {
      using States = typename Machine::States;
      
      template <typename State>
      struct ClaimEvent {
        template <typename Target>
        struct TryTransition {
          using Result = TryTransition;
          using GuardExists = typename HasGuard<Kernel, Machine, State, Event, Target>::Result;
          using Guard = typename If<GuardExists, typename Machine::template Guard<Kernel, State, Event, Target>, EmptyGuard>::Result;
          bool operator()() { return Guard()() && ChangeState<Kernel, Machine, State, Event, Target>()(); }
        };
      
        using Result = ClaimEvent;
        template <typename State1, typename State2> using CloserThan = LessThan<typename Distance<States, State, State1>::Result, typename Distance<States, State, State2>::Result>;
        template <typename Target> using HasTransitionTo = HasTransition<Kernel, Machine, State, Event, Target>;
        using Targets = typename Select<typename Flatten<States>::Result, HasTransitionTo>::Result;
        using SortedTargets = typename Sort<Targets, CloserThan>::Result;
        using TransitionSuccessful = typename Map<SortedTargets, TryTransition>::Result;
        bool operator()() { return Until<TransitionSuccessful>()(); }
      };
    
      template <typename State>
      struct RunEventHandlers {
        using HandlingStates = typename SelfAndAncestors<States, State>::Result;
        using EventClaimed = typename Map<HandlingStates, ClaimEvent>::Result;
        bool operator()() { return Until<EventClaimed>()(); }
      };
  
      template <typename State>
      struct DispatchIfCurrent {
        using Result = DispatchIfCurrent;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<State>()()) return false;
          RunEventHandlers<State>()();
          return true;
        }
      };
    
      using LeafStates = typename Leaves<States>::Result;
      using Dispatched = typename Map<LeafStates, DispatchIfCurrent>::Result;
      void operator()() { Until<Dispatched>()(); }
    };
    
    namespace {
      template <typename Kernel, typename Machine, typename Event>
      void dispatch() { Dispatch<Kernel, Machine, Event>()(); }
    }
    
    template <typename Kernel, typename Machine>
    struct Initialise {
      using Result = Initialise;
      void operator()() { EnterStates<Kernel, Machine>()(); }
    };
    
    template <typename Machine, typename Event>
    struct RespondsTo {
      using States = typename Flatten<typename Machine::States>::Result;
      struct DummyKernel { template <typename> static void post() { } };
      template <typename State>
      struct HasEventTransition {
        template <typename Target> using HasTransitionTo = HasTransition<DummyKernel, Machine, State, Event, Target>;
        using Result = typename Any<typename Select<States, HasTransitionTo>::Result>::Result;
      };
      using Result = typename Any<typename Select<States, HasEventTransition>::Result>::Result;
    };
  }
}

#endif
