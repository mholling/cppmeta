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
      typedef typename Prepend<State, typename DefaultPath<FirstSubstate>::Result>::Result Result;
    };
    template <typename State>
    struct DefaultPath<Tree<State>> { typedef List<State> Result; };
  
    template <typename Machine, typename Substates>
    struct ExitStates {
      template <typename State>
      struct ExitIfCurrent {
        typedef typename SelfAndAncestors<Substates, State>::Result ExitPath;
        template <typename OtherState> struct GetExit { typedef GetExit Result; void operator()() { Machine::template exit<OtherState>(); } };
        typedef typename Map<ExitPath, GetExit>::Result Exits;
        typedef ExitIfCurrent Result;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<State>()()) return false;
          Each<Exits>()();
          return true;
        }
      };
      typedef typename Leaves<Substates>::Result LeafStates;
      typedef typename Map<LeafStates, ExitIfCurrent>::Result ExitFromCurrentState;
      bool operator()() { return Until<ExitFromCurrentState>()(); }
    };
    
    template <typename Machine, typename Substates = typename Machine::States, typename Target = typename Root<Substates>::Result>
    struct EnterStates {
      typedef typename Reverse<typename Ancestors<Substates, Target>::Result>::Result TargetEntryPath;
      typedef typename DefaultPath<typename FindBranch<Substates, Target>::Result>::Result DefaultEntryPath;
      typedef typename Concat<TargetEntryPath, DefaultEntryPath>::Result EntryPath;
      // TODO: change template <> enter to template class with operator()() ? (also for exits?)
      template <typename State> struct GetEntry { typedef GetEntry Result; void operator()() { Machine::template enter<State>(); } };
      typedef typename Map<EntryPath, GetEntry>::Result Entries;
      typedef typename Last<EntryPath>::Result FinalState;
      void operator()() {
        Each<Entries>()();
        typename CurrentState<Machine>::template Set<FinalState>()();
      }
    };
    
    template <typename Machine, typename State, typename Event, typename Target> using HasGuard = HasBoolCallOperator<typename Machine::template Guard<State, Event, Target>>;
    template <typename Machine, typename State, typename Event, typename Target> using HasAction = HasVoidCallOperator<typename Machine::template Action<State, Event, Target>>;
    template <typename Machine, typename State, typename Event, typename Target> using HasTransition = Or<typename HasGuard<Machine, State, Event, Target>::Result, typename HasAction<Machine, State, Event, Target>::Result>;
    
    struct EmptyAction { void operator()() { } };
    
    template <typename Machine, typename Source, typename Event, typename Target>
    struct ChangeState {
      typedef typename CommonBranch<typename Machine::States, Source, Target>::Result Substates;
      typedef typename HasAction<Machine, Source, Event, Target>::Result Actioned;
      typedef typename If<Actioned, typename Machine::template Action<Source, Event, Target>, EmptyAction>::Result Action;
      bool operator()() {
        ExitStates<Machine, Substates>()();
        Action()();
        EnterStates<Machine, Substates, Target>()();
        return true;
      }
    };
    
    template <typename Machine, typename Event>
    struct Dispatch {
      typedef typename Machine::States States;
  
      template <typename State>
      struct ClaimEvent {
        template <typename Target>
        struct TryTransition {
          typedef TryTransition Result;
          typedef typename HasGuard<Machine, State, Event, Target>::Result Guarded;
          struct NoGuard { bool operator()() { return true; } };
          typedef typename If<Guarded, typename Machine::template Guard<State, Event, Target>, NoGuard>::Result Guard;
          bool operator()() { return Guard()() && ChangeState<Machine, State, Event, Target>()(); }
        };
      
        typedef ClaimEvent Result;
        template <typename State1, typename State2> using CloserThan = LessThan<typename Distance<States, State, State1>::Result, typename Distance<States, State, State2>::Result>;
        template <typename Target> using HasTransitionTo = HasTransition<Machine, State, Event, Target>;
        typedef typename Select<typename Flatten<States>::Result, HasTransitionTo>::Result Targets;
        typedef typename Sort<Targets, CloserThan>::Result SortedTargets;
        typedef typename Map<SortedTargets, TryTransition>::Result TransitionSuccessful;
        bool operator()() { return Until<TransitionSuccessful>()(); }
      };
    
      template <typename State>
      struct RunEventHandlers {
        typedef typename SelfAndAncestors<States, State>::Result HandlingStates;
        typedef typename Map<HandlingStates, ClaimEvent>::Result EventClaimed;
        bool operator()() { return Until<EventClaimed>()(); }
      };
  
      template <typename State>
      struct DispatchIfCurrent {
        typedef DispatchIfCurrent Result;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<State>()()) return false;
          RunEventHandlers<State>()();
          return true;
        }
      };
    
      typedef typename Leaves<States>::Result LeafStates;
      typedef typename Map<LeafStates, DispatchIfCurrent>::Result Dispatched;
      void operator()() { Until<Dispatched>()(); }
      static void dispatch() { Dispatch()(); }
    };
  
    template <typename Machine>
    struct Initialise {
      typedef Initialise Result;
      void operator()() { EnterStates<Machine>()(); }
    };
    
    template <typename Machine, typename Event>
    struct RespondsTo {
      typedef typename Flatten<typename Machine::States>::Result States;
      template <typename State>
      struct HasEventTransition {
        template <typename Target> using HasTransitionTo = HasTransition<Machine, State, Event, Target>;
        typedef typename Any<typename Select<States, HasTransitionTo>::Result>::Result Result;
      };
      typedef typename Any<typename Select<States, HasEventTransition>::Result>::Result Result;
    };
  }
}
