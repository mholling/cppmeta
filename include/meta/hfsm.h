#ifndef CPPMETA_META_HFSM_H
#define CPPMETA_META_HFSM_H

#include "meta.h"
#include "list.h"
#include "tree.h"

namespace CppMeta {
  namespace HFSM {
    template <typename Machine>
    struct CurrentState {
      static int index;
      template <typename State>
      struct Test { bool operator()() { return Index<typename Machine::States, State>::value == index; } };
      template <typename State>
      struct Set { void operator()() { index = Index<typename Machine::States, State>::value; } };
    };
    template <typename Machine>
    int CurrentState<Machine>::index = 0;
    
    template <typename States> struct DefaultPathImpl;
    template <typename States> using DefaultPath = typename DefaultPathImpl<States>::Result;
    template <typename State, typename FirstSubstate, typename... OtherSubstates>
    struct DefaultPathImpl<Tree<State, FirstSubstate, OtherSubstates...>> { using Result = Prepend<State, DefaultPath<FirstSubstate>>; };
    template <typename State>
    struct DefaultPathImpl<Tree<State>> { using Result = List<State>; };
    
    template <typename Kernel, typename Machine, typename Substates>
    struct ExitStates {
      template <typename Candidate>
      struct ExitIfCurrent {
        template <typename State>
        struct HasExitImpl {
          template <typename U, void (U::*)()> struct Signature;
          template <typename U> static Bool<true>  test(Signature<typename U::template Exit<Kernel, State>, &U::template Exit<Kernel, State>::operator()> *);
          template <typename>   static Bool<false> test(...);
          using Result = decltype(test<Machine>(0));
        };
        template <typename State> using HasExit = typename HasExitImpl<State>::Result;
        
        template <typename State> struct Exit { void operator()() { typename Machine::template Exit<Kernel, State>()(); } };
        using ExitPath = SelfAndAncestors<Substates, Candidate>;
        using Result = ExitIfCurrent;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<Candidate>()()) return false;
          Each<Select<ExitPath, HasExit>, Exit>()();
          return true;
        }
      };
      
      bool operator()() { return Until<Leaves<Substates>, ExitIfCurrent>()(); }
    };
    
    template <typename Kernel, typename Machine, typename Substates = typename Machine::States, typename Target = Root<Substates>>
    struct EnterStates {
      template <typename State>
      struct HasEntryImpl {
        template <typename U, void (U::*)()> struct Signature;
        template <typename U> static Bool<true>  test(Signature<typename U::template Enter<Kernel, State>, &U::template Enter<Kernel, State>::operator()> *);
        template <typename>   static Bool<false> test(...);
        using Result = decltype(test<Machine>(0));
      };
      template <typename State> using HasEntry = typename HasEntryImpl<State>::Result;
      
      template <typename State> struct Enter { void operator()() { typename Machine::template Enter<Kernel, State>()(); } };
      using TargetEntryPath = Reverse<Ancestors<Substates, Target>>;
      using DefaultEntryPath = DefaultPath<FindBranch<Substates, Target>>;
      using EntryPath = Concat<TargetEntryPath, DefaultEntryPath>;
      using FinalState = Last<EntryPath>;
      
      void operator()() {
        Each<Select<EntryPath, HasEntry>, Enter>()();
        typename CurrentState<Machine>::template Set<FinalState>()();
      }
    };
    
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
    struct HasGuardImpl {
      template <typename U, bool (U::*)()> struct Signature;
      template <typename U> static Bool<true>  test(Signature<typename U::template Guard<Kernel, State, Event, Target>, &U::template Guard<Kernel, State, Event, Target>::operator()> *);
      template <typename>   static Bool<false> test(...);
      using Result = decltype(test<Machine>(0));
    };
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasGuard = typename HasGuardImpl<Kernel, Machine, State, Event, Target>::Result;
    
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
    struct HasActionImpl {
      template <typename U, void (U::*)()> struct Signature;
      template <typename U> static Bool<true>  test(Signature<typename U::template Action<Kernel, State, Event, Target>, &U::template Action<Kernel, State, Event, Target>::operator()> *);
      template <typename>   static Bool<false> test(...);
      using Result = decltype(test<Machine>(0));
    };
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasAction = typename HasActionImpl<Kernel, Machine, State, Event, Target>::Result;
    
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasTransition = Or<HasGuard<Kernel, Machine, State, Event, Target>, HasAction<Kernel, Machine, State, Event, Target>>;
    
    struct EmptyAction { void operator()() { } };
    struct EmptyGuard { bool operator()() { return true; } };
    
    template <typename Kernel, typename Machine, typename Source, typename Event, typename Target>
    struct ChangeState {
      using Substates = CommonBranch<typename Machine::States, Source, Target>;
      using ActionExists = HasAction<Kernel, Machine, Source, Event, Target>;
      struct GetAction { using Result = typename Machine::template Action<Kernel, Source, Event, Target>; };
      using Action = If<ActionExists, GetAction, EmptyAction>;
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
          using GuardExists = HasGuard<Kernel, Machine, State, Event, Target>;
          struct GetGuard { using Result = typename Machine::template Guard<Kernel, State, Event, Target>; };
          using Guard = If<GuardExists, GetGuard, EmptyGuard>;
          bool operator()() { return Guard()() && ChangeState<Kernel, Machine, State, Event, Target>()(); }
        };
      
        template <typename State1, typename State2> using CloserThan = LessThan<Distance<States, State, State1>, Distance<States, State, State2>>;
        template <typename Target> using HasTransitionTo = HasTransition<Kernel, Machine, State, Event, Target>;
        using Targets = Select<Flatten<States>, HasTransitionTo>;
        using SortedTargets = Sort<Targets, CloserThan>;
        bool operator()() { return Until<SortedTargets, TryTransition>()(); }
      };
    
      template <typename State>
      struct RunEventHandlers {
        using HandlingStates = SelfAndAncestors<States, State>;
        bool operator()() { return Until<HandlingStates, ClaimEvent>()(); }
      };
  
      template <typename State>
      struct DispatchIfCurrent {
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<State>()()) return false;
          RunEventHandlers<State>()();
          return true;
        }
      };
    
      void operator()() { Until<Leaves<States>, DispatchIfCurrent>()(); }
    };
    
    namespace {
      template <typename Kernel, typename Machine, typename Event>
      void dispatch() { Dispatch<Kernel, Machine, Event>()(); }
    }
    
    template <typename Kernel, typename Machine> using Initialise = EnterStates<Kernel, Machine>;
    
    template <typename Machine, typename Event>
    struct RespondsToImpl {
      using States = Flatten<typename Machine::States>;
      struct DummyKernel { template <typename> static void post() { } };
      template <typename State>
      struct HasEventTransitionImpl {
        template <typename Target> using HasTransitionTo = HasTransition<DummyKernel, Machine, State, Event, Target>;
        using Result = Any<States, HasTransitionTo>;
      };
      template <typename State> using HasEventTransition = typename HasEventTransitionImpl<State>::Result;
      
      using Result = Any<States, HasEventTransition>;
    };
    template <typename Machine, typename Event> using RespondsTo = typename RespondsToImpl<Machine, Event>::Result;
  }
}

#endif
