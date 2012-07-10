namespace Meta {
  namespace HFSM {
    template <typename States>
    struct CurrentState {
      static int index;
      template <typename State>
      struct Test { bool operator()() { return Index<States, State>::Result::value == index; } };
      template <typename State>
      struct Set { void operator()() { index = Index<States, State>::Result::value; } };
    };
    template <typename States>
    int CurrentState<States>::index = 0;
  
    template <typename State> struct Enter { typedef Enter Result; void operator()() { } };
    template <typename State> struct  Exit { typedef  Exit Result; void operator()() { } };
  
    template <typename States, typename State, typename Event, typename Target> struct Transition;
  
    struct NoTransitionAction { static void action() { } };
    struct NoTransitionGuard { static bool guard() { return true; } };
  
    template <typename States> struct DefaultPath;
    template <typename State, typename FirstSubstate, typename... OtherSubstates>
    struct DefaultPath<Tree<State, FirstSubstate, OtherSubstates...>> {
      typedef typename Prepend<State, typename DefaultPath<FirstSubstate>::Result>::Result Result;
    };
    template <typename State>
    struct DefaultPath<Tree<State>> { typedef List<State> Result; };
  
    template <typename States, typename Substates>
    struct ExitStates {
      template <typename State>
      struct ExitIfCurrent {
        typedef typename SelfAndAncestors<Substates, State>::Result ExitPath;
        typedef typename Map<ExitPath, Exit>::Result Exits;
        typedef ExitIfCurrent Result;
        bool operator()() {
          if (!typename CurrentState<States>::template Test<State>()()) return false;
          Each<Exits>()();
          return true;
        }
      };
      typedef typename Leaves<Substates>::Result LeafStates;
      typedef typename Map<LeafStates, ExitIfCurrent>::Result ExitFromCurrentState;
      bool operator()() { return Until<ExitFromCurrentState>()(); }
    };

    template <typename States, typename Substates, typename Target>
    struct EnterStates {
      typedef typename Reverse<typename Ancestors<Substates, Target>::Result>::Result TargetEntryPath;
      typedef typename DefaultPath<typename FindBranch<Substates, Target>::Result>::Result DefaultEntryPath;
      typedef typename Concat<TargetEntryPath, DefaultEntryPath>::Result EntryPath;
      typedef typename Map<EntryPath, Enter>::Result Entries;
      typedef typename Last<EntryPath>::Result FinalState;
      void operator()() {
        Each<Entries>()();
        typename CurrentState<States>::template Set<FinalState>()();
      }
    };
  
    template <typename States, typename Source, typename Target>
    struct ChangeState {
      typedef typename CommonBranch<States, Source, Target>::Result Substates;
      bool operator()(void (*action)()) {
        ExitStates<States, Substates>()();
        action();
        EnterStates<States, Substates, Target>()();
        return true;
      }
    };
  
    template <typename Type>
    struct TransitionIsImplemented {
      struct Yes; struct No;
      template <void (*)()> struct ActionSignature;
      template <typename C> static Yes& test_action(ActionSignature<&C::action> *);
      template <typename>   static No&  test_action(...);
      template <bool (*)()> struct GuardSignature;
      template <typename C> static Yes& test_guard(GuardSignature<&C::guard> *);
      template <typename>   static No&  test_guard(...);
      typedef typename Same<decltype(test_action<Type>(0)), Yes&>::Result ActionPresent;
      typedef typename Same<decltype(test_guard<Type>(0)), Yes&>::Result GuardPresent;
      typedef typename And<ActionPresent, GuardPresent>::Result Result;
    };
  
    template <typename> struct TryTransition;
    template <typename States, typename State, typename Event, typename Target>
    struct TryTransition<Transition<States, State, Event, Target>> {
      typedef TryTransition Result;
      typedef Transition<States, State, Event, Target> Trans;
      bool operator()() { return Trans::guard() && ChangeState<States, State, Target>()(Trans::action); }
    };
  
    template <typename States, typename Event>
    struct Dispatcher {
      typedef Dispatcher Result;
    
      template <typename State>
      struct ClaimEvent {
        typedef ClaimEvent Result;
        template <typename Target> struct GetTransition { typedef Transition<States, State, Event, Target> Result; };
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
          if (!typename CurrentState<States>::template Test<State>()()) return false;
          RunEventHandlers<State>()();
          return true;
        }
      };
    
      typedef typename Leaves<States>::Result LeafStates;
      typedef typename Map<LeafStates, DispatchIfCurrent>::Result Dispatched;
      void operator()() { Until<Dispatched>()(); }
    };
  
    template <typename StatesList, typename Event>
    struct Dispatch {
      template <typename States> using GetDispatcher = Dispatcher<States, Event>;
      typedef typename Map<StatesList, GetDispatcher>::Result Dispatchers;
      void operator()() { Each<Dispatchers>()(); }
    };
  
    template <typename States>
    struct Initialiser {
      typedef Initialiser Result;
      void operator()() { EnterStates<States, States, typename Root<States>::Result>()(); }
    };
    template <typename StatesList> using Initialise = Each<typename Map<StatesList, HFSM::Initialiser>::Result>;
  }
}