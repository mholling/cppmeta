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
    
    template <typename Kernel, typename Machine, typename Substates>
    struct ExitStates {
      template <typename Candidate>
      struct ExitIfCurrent {
        template <typename State> using HasExit = HasVoidCallOperator<typename Machine::template Exit<Kernel, State>>;
        template <typename State> struct Exit { typedef Exit Result; void operator()() { typename Machine::template Exit<Kernel, State>()(); } };
        typedef typename SelfAndAncestors<Substates, Candidate>::Result ExitPath;
        typedef ExitIfCurrent Result;
        bool operator()() {
          if (!typename CurrentState<Machine>::template Test<Candidate>()()) return false;
          Each<typename Select<ExitPath, HasExit>::Result, Exit>()();
          return true;
        }
      };
      typedef typename Leaves<Substates>::Result LeafStates;
      typedef typename Map<LeafStates, ExitIfCurrent>::Result ExitFromCurrentState;
      bool operator()() { return Until<ExitFromCurrentState>()(); }
    };
    
    template <typename Kernel, typename Machine, typename Substates = typename Machine::States, typename Target = typename Root<Substates>::Result>
    struct EnterStates {
      template <typename State> using HasEntry = HasVoidCallOperator<typename Machine::template Enter<Kernel, State>>;
      template <typename State> struct Enter { typedef Enter Result; void operator()() { typename Machine::template Enter<Kernel, State>()(); } };
      typedef typename Reverse<typename Ancestors<Substates, Target>::Result>::Result TargetEntryPath;
      typedef typename DefaultPath<typename FindBranch<Substates, Target>::Result>::Result DefaultEntryPath;
      typedef typename Concat<TargetEntryPath, DefaultEntryPath>::Result EntryPath;
      typedef typename Last<EntryPath>::Result FinalState;
      void operator()() {
        Each<typename Select<EntryPath, HasEntry>::Result, Enter>()();
        typename CurrentState<Machine>::template Set<FinalState>()();
      }
    };
    
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasGuard = HasBoolCallOperator<typename Machine::template Guard<Kernel, State, Event, Target>>;
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasAction = HasVoidCallOperator<typename Machine::template Action<Kernel, State, Event, Target>>;
    template <typename Kernel, typename Machine, typename State, typename Event, typename Target>
      using HasTransition = Or<typename HasGuard<Kernel, Machine, State, Event, Target>::Result, typename HasAction<Kernel, Machine, State, Event, Target>::Result>;
    
    struct EmptyAction { void operator()() { } };
    
    template <typename Kernel, typename Machine, typename Source, typename Event, typename Target>
    struct ChangeState {
      typedef typename CommonBranch<typename Machine::States, Source, Target>::Result Substates;
      typedef typename HasAction<Kernel, Machine, Source, Event, Target>::Result ActionExists;
      typedef typename If<ActionExists, typename Machine::template Action<Kernel, Source, Event, Target>, EmptyAction>::Result Action;
      bool operator()() {
        ExitStates<Kernel, Machine, Substates>()();
        Action()();
        EnterStates<Kernel, Machine, Substates, Target>()();
        return true;
      }
    };
    
    template <typename Kernel, typename Machine, typename Event>
    struct Dispatch {
      typedef typename Machine::States States;
      
      template <typename State>
      struct ClaimEvent {
        template <typename Target>
        struct TryTransition {
          typedef TryTransition Result;
          typedef typename HasGuard<Kernel, Machine, State, Event, Target>::Result GuardExists;
          struct EmptyGuard { bool operator()() { return true; } };
          typedef typename If<GuardExists, typename Machine::template Guard<Kernel, State, Event, Target>, EmptyGuard>::Result Guard;
          bool operator()() { return Guard()() && ChangeState<Kernel, Machine, State, Event, Target>()(); }
        };
      
        typedef ClaimEvent Result;
        template <typename State1, typename State2> using CloserThan = LessThan<typename Distance<States, State, State1>::Result, typename Distance<States, State, State2>::Result>;
        template <typename Target> using HasTransitionTo = HasTransition<Kernel, Machine, State, Event, Target>;
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
    };
    
    namespace {
      template <typename Kernel, typename Machine, typename Event>
      void dispatch() { Dispatch<Kernel, Machine, Event>()(); }
    }
    
    template <typename Kernel, typename Machine>
    struct Initialise {
      typedef Initialise Result;
      void operator()() { EnterStates<Kernel, Machine>()(); }
    };
    
    template <typename Machine, typename Event>
    struct RespondsTo {
      typedef typename Flatten<typename Machine::States>::Result States;
      struct DummyKernel { template <typename> static void post() { } };
      template <typename State>
      struct HasEventTransition {
        template <typename Target> using HasTransitionTo = HasTransition<DummyKernel, Machine, State, Event, Target>;
        typedef typename Any<typename Select<States, HasTransitionTo>::Result>::Result Result;
      };
      typedef typename Any<typename Select<States, HasEventTransition>::Result>::Result Result;
    };
  }
}
