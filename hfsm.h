namespace Meta {
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
  
    struct NoTransitionAction { static void action() { } };
    struct NoTransitionGuard { static bool guard() { return true; } };
  
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
      template <typename State> struct GetEntry { typedef GetEntry Result; void operator()() { Machine::template enter<State>(); } };
      typedef typename Map<EntryPath, GetEntry>::Result Entries;
      typedef typename Last<EntryPath>::Result FinalState;
      void operator()() {
        Each<Entries>()();
        typename CurrentState<Machine>::template Set<FinalState>()();
      }
    };
  
    template <typename Machine, typename Source, typename Target>
    struct ChangeState {
      typedef typename CommonBranch<typename Machine::States, Source, Target>::Result Substates;
      bool operator()(void (*action)()) {
        ExitStates<Machine, Substates>()();
        action();
        EnterStates<Machine, Substates, Target>()();
        return true;
      }
    };
  
    template <typename Transition>
    struct TransitionIsImplemented {
      struct Yes; struct No;
      template <void (*)()> struct ActionSignature;
      template <typename C> static Yes& test_action(ActionSignature<&C::action> *);
      template <typename>   static No&  test_action(...);
      template <bool (*)()> struct GuardSignature;
      template <typename C> static Yes& test_guard(GuardSignature<&C::guard> *);
      template <typename>   static No&  test_guard(...);
      typedef typename Same<decltype(test_action<Transition>(0)), Yes&>::Result ActionPresent;
      typedef typename Same<decltype(test_guard<Transition>(0)), Yes&>::Result GuardPresent;
      typedef typename And<ActionPresent, GuardPresent>::Result Result;
    };
  
    template <typename Machine, typename Event>
    struct Dispatcher {
      typedef Dispatcher Result;
      typedef typename Machine::States States;
  
      template <typename> struct TryTransition;
      template <typename State, typename Target>
      struct TryTransition<typename Machine::template Transition<State, Event, Target>> {
        typedef TryTransition Result;
        typedef typename Machine::template Transition<State, Event, Target> Trans;
        bool operator()() { return Trans::guard() && ChangeState<Machine, State, Target>()(Trans::action); }
      };
    
      template <typename State>
      struct ClaimEvent {
        typedef ClaimEvent Result;
        template <typename Target> struct GetTransition { typedef typename Machine::template Transition<State, Event, Target> Result; };
        template <typename State1, typename State2> using CloserThan = LessThan<typename Distance<States, State, State1>::Result, typename Distance<States, State, State2>::Result>;
        typedef typename Map<typename Flatten<States>::Result, GetTransition>::Result PossibleTransitions;
        typedef typename Select<PossibleTransitions, TransitionIsImplemented>::Result ImplementedTransitions;
        typedef typename Sort<ImplementedTransitions, CloserThan>::Result SortedTransitions;
        typedef typename Map<SortedTransitions, TryTransition>::Result TransitionSuccessful;
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
  
    template <typename Machines, typename Event>
    struct Dispatch {
      template <typename Machine> using GetDispatcher = Dispatcher<Machine, Event>;
      typedef typename Map<Machines, GetDispatcher>::Result Dispatchers;
      void operator()() { Each<Dispatchers>()(); }
    };
  
    template <typename Machine>
    struct Initialiser {
      typedef Initialiser Result;
      void operator()() { EnterStates<Machine>()(); }
    };
    template <typename Machines> using Initialise = Each<typename Map<Machines, HFSM::Initialiser>::Result>;
  }
}