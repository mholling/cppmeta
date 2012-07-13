namespace CppMeta {
  namespace Tests {
    namespace Basics {
      static_assert(Same<int, int>::Result::value, "failed");
      static_assert(!Same<int, char>::Result::value, "failed");
      
      static_assert(Same<Decrement<Int<10>>::Result, Int<9>>::Result::value, "failed");
      static_assert(Same<Increment<Int<9>>::Result, Int<10>>::Result::value, "failed");
      
      static_assert(Same<Plus<Int<10>, Int<3>>::Result, Int<13>>::Result::value, "failed");
      static_assert(Same<Minus<Int<10>, Int<3>>::Result, Int<7>>::Result::value, "failed");
      
      static_assert(LessThan<Int<4>, Int<5>>::Result::value, "failed");
      static_assert(!LessThan<Int<5>, Int<5>>::Result::value, "failed");
      static_assert(!LessThan<Int<6>, Int<5>>::Result::value, "failed");
      static_assert(!GreaterThan<Int<4>, Int<5>>::Result::value, "failed");
      static_assert(!GreaterThan<Int<5>, Int<5>>::Result::value, "failed");
      static_assert(GreaterThan<Int<6>, Int<5>>::Result::value, "failed");
      
      static_assert(Same<And<Bool<true>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<And<Bool<true>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<true>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<And<Bool<false>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<true>, Bool<false>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<true>>::Result, Bool<true>>::Result::value, "failed");
      static_assert(Same<Or<Bool<false>, Bool<false>>::Result, Bool<false>>::Result::value, "failed");
      
      static_assert(Same<Self<int>::Result, int>::Result::value, "failed");
      static_assert(!Same<Self<bool>::Result, int>::Result::value, "failed");

      struct MakeChar { typedef char Result; };
      template <typename Type> struct MakeType { typedef Type Result; };
      static_assert(!CanEval<int>::Result::value, "failed");
      static_assert(CanEval<MakeChar>::Result::value, "failed");
      static_assert(CanEval<MakeType<long>>::Result::value, "failed");
      static_assert(Same<Eval<int>::Result, int>::Result::value, "failed");
      static_assert(Same<Eval<MakeChar>::Result, char>::Result::value, "failed");
      static_assert(Same<Eval<MakeType<long>>::Result, long>::Result::value, "failed");

      static_assert(Same<If<Bool<true>, int, char>::Result, int>::Result::value, "failed");
      static_assert(Same<If<Bool<false>, int, char>::Result, char>::Result::value, "failed");
      
      static_assert(Same<LesserOf<Int<4>, Int<5>>::Result, Int<4>>::Result::value, "failed");
      static_assert(Same<GreaterOf<Int<4>, Int<5>>::Result, Int<5>>::Result::value, "failed");

      struct Parent { };
      struct Child : Parent { };
      static_assert( IsOrInherits<Child, Parent>::Result::value, "failed");
      static_assert(!IsOrInherits<Parent, Child>::Result::value, "failed");
      static_assert(IsOrInherits<Parent, Parent>::Result::value, "failed");
      static_assert( Inherits<Child, Parent>::Result::value, "failed");
      static_assert(!Inherits<Parent, Child>::Result::value, "failed");
      static_assert(!Inherits<Parent, Parent>::Result::value, "failed");

      struct VoidReturn { void operator ()() { } };
      struct BoolReturn { bool operator ()() { return true; } };
      struct VoidReturnIntCharArgs { void operator ()(int, char) { } };
      static_assert( HasCallOperator<VoidReturn, void>::Result::value, "failed");
      static_assert(!HasCallOperator<BoolReturn, void>::Result::value, "failed");
      static_assert( HasCallOperator<VoidReturnIntCharArgs, void, int, char>::Result::value, "failed");
      static_assert(!HasCallOperator<VoidReturnIntCharArgs, void, char, int>::Result::value, "failed");
      static_assert(!HasCallOperator<VoidReturn,            void, int, char>::Result::value, "failed");
      static_assert(!HasBoolCallOperator<VoidReturn>::Result::value, "failed");
      static_assert( HasBoolCallOperator<BoolReturn>::Result::value, "failed");
    }
    namespace Lists {
      static_assert(IsList<List<int, char>>::Result::value, "failed");
      static_assert(!IsList<int>::Result::value, "failed");

      static_assert(Same<Append<List<char, int>, float>::Result, List<char, int, float>>::Result::value, "failed");
      static_assert(Same<Append<List<>, float>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<char, int>>::Result, List<float, char, int>>::Result::value, "failed");
      static_assert(Same<Prepend<float, List<>>::Result, List<float>>::Result::value, "failed");
      static_assert(Same<Concat<List<char, int>, List<float, bool>>::Result, List<char, int, float, bool>>::Result::value, "failed");

      static_assert(Same<Reverse<List<float, char, bool, int>>::Result, List<int, bool, char, float>>::Result::value, "failed");
      
      template <typename T> using IsFloat = Same<T, float>;
      static_assert(Same<Map<List<int, char, float, bool>, IsFloat>::Result, List<Bool<false>,Bool<false>,Bool<true>,Bool<false>>>::Result::value, "failed");

      template <typename T, typename Index> struct GetIndex { typedef Index Result; };
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetIndex>::Result, List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>>>::Result::value, "failed");
      template <typename T, typename Index> struct GetElement { typedef T Result; };
      static_assert(Same<MapWithIndex<List<int, bool, char, long, float>, GetElement>::Result, List<int, bool, char, long, float>>::Result::value, "failed");

      static_assert(Same<Select<List<int, char, float, bool, float>, IsFloat>::Result, List<float, float>>::Result::value, "failed");
      static_assert(Same<Reject<List<int, char, float, bool, float>, IsFloat>::Result, List<int, char, bool>>::Result::value, "failed");
      
      static_assert(Same<Find<List<bool, char, float, int>, IsFloat>::Result, float>::Result::value, "failed");

      static_assert(Length<List<int, int, float>>::Result::value == 3, "failed");
      static_assert(Length<List<>>::Result::value == 0, "failed");

      static_assert(Any<List<int, char>>::Result::value, "failed");
      static_assert(!Any<List<>>::Result::value, "failed");

      static_assert(!Empty<List<int, char>>::Result::value, "failed");
      static_assert(Empty<List<>>::Result::value, "failed");

      static_assert(Contains<List<int, char, float>, float>::Result::value, "failed");
      static_assert(!Contains<List<int, char, float>, bool>::Result::value, "failed");
      static_assert(!Contains<List<>, char>::Result::value, "failed");

      static_assert(Same<Unique<List<int, char, bool>>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<Unique<List<int, char, bool, char, float>>::Result, List<int, char, bool, float>>::Result::value, "failed");

      static_assert(Same<Index<List<int, char, bool>, int>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Index<List<int, char, bool, float>, bool>::Result, Int<2>>::Result::value, "failed");

      static_assert(Same<First<List<int, char, bool>>::Result, int>::Result::value, "failed");
      static_assert(Same<Last<List<int, char, bool>>::Result, bool>::Result::value, "failed");
      static_assert(Same<At<List<int, float, char, bool>, Int<2>>::Result, char>::Result::value, "failed");

      static_assert(Same<Flatten<List<int, List<char, bool>, float>>::Result, List<int, char, bool, float>>::Result::value, "failed");
      static_assert(Same<Flatten<List<int, List<bool, List<char, long>, List<double, List<float>>>>>::Result, List<int, bool, char, long, double, float>>::Result::value, "failed");

      static_assert(Same<Before<List<int, char, bool, float>, Int<3>>::Result, List<int, char, bool>>::Result::value, "failed");
      static_assert(Same<After<List<int, char, bool, float>, Int<1>>::Result, List<bool, float>>::Result::value, "failed");
      static_assert(Same<UpTo<List<int, char, bool, float>, bool>::Result, List<int, char>>::Result::value, "failed");
      static_assert(Same<UpTo<List<int, char, bool, float>, int>::Result, List<>>::Result::value, "failed");

      typedef List<Int<1>, Int<6>, Int<0>, Int<4>, Int<3>, Int<2>, Int<5>> Unsorted;
      typedef List<Int<0>, Int<1>, Int<2>, Int<3>, Int<4>, Int<5>, Int<6>> Sorted;
      static_assert(Same<Sort<Unsorted, LessThan>::Result, Sorted>::Result::value, "failed");
      
      static_assert(Same<Max<Unsorted>::Result, Int<6>>::Result::value, "failed");
      static_assert(Same<Min<Unsorted>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Max<List<Int<0>>>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Min<List<Int<0>>>::Result, Int<0>>::Result::value, "failed");
      
      int x, y, z;
      struct SetX { void operator()(int v) { x = v; } };
      struct SetY { void operator()(int v) { y = v; } };
      struct SetZ { void operator()(int v) { z = v; } };
      typedef List<SetX, SetY, SetZ> Setters;
      
      int count;
      struct StartFalse { bool operator()() { count++; return false; } };
      struct StillFalse { bool operator()() { count++; return false; } };
      struct NowTrue    { bool operator()() { count++; return true; } };
      struct TrueAgain  { bool operator()() { count++; return true; } };
      struct FinalFalse { bool operator()() { count++; return false; } };
      typedef List<StartFalse, StillFalse, NowTrue, TrueAgain, FinalFalse> SomeSuccesses;
      typedef List<StartFalse, StillFalse, FinalFalse> AllFailures;
      typedef List<NowTrue, TrueAgain, FinalFalse> SucceedTwiceThenFail;
      typedef List<NowTrue, TrueAgain> SucceedTwice;
      
      void test() {
        Each<List<SetX, SetY, SetZ>>()(10);
        assert(x == 10);
        assert(y == 10);
        assert(z == 10);

        count = 0;
        bool result;
        assert(result = Until<SomeSuccesses>()());
        assert(count == 3);

        count = 0;
        assert(result = !Until<AllFailures>()());
        assert(count == 3);

        count = 0;
        assert(result = While<SucceedTwice>()());
        assert(count == 2);

        count = 0;
        assert(result = !While<SucceedTwiceThenFail>()());
        assert(count == 3);
      }
    };
    namespace Trees {
      struct A; struct AA; struct AB; struct AC; struct AD; struct AAA; struct AAB; struct ABA; struct ABB; struct ABC; struct ADA; struct ADB; struct ADAA; struct ADAB;
      
          typedef Tree<AAA> TreeAAA;
          typedef Tree<AAB> TreeAAB;
        typedef Tree<AA, TreeAAA, TreeAAB> TreeAA;
          typedef Tree<ABA> TreeABA;
          typedef Tree<ABB> TreeABB;
          typedef Tree<ABC> TreeABC;
        typedef Tree<AB, TreeABA, TreeABB, TreeABC> TreeAB;
        typedef Tree<AC> TreeAC;
            typedef Tree<ADAA> TreeADAA;
            typedef Tree<ADAB> TreeADAB;
          typedef Tree<ADA, TreeADAA, TreeADAB> TreeADA;
          typedef Tree<ADB> TreeADB;
        typedef Tree<AD, TreeADA, TreeADB> TreeAD;
      typedef Tree<A, TreeAA, TreeAB, TreeAC, TreeAD> TreeA;
      
      static_assert(IsTree<TreeA>::Result::value, "failed");
      static_assert(IsTree<TreeADAA>::Result::value, "failed");
      static_assert(!IsTree<int>::Result::value, "failed");
      
      static_assert(IsLeaf<TreeADAA>::Result::value, "failed");
      static_assert(!IsLeaf<TreeA>::Result::value, "failed");
      
      static_assert(Contains<TreeA, A>::Result::value, "failed");
      static_assert(Contains<TreeA, ADAB>::Result::value, "failed");
      static_assert(!Contains<TreeA, float>::Result::value, "failed");
      
      static_assert(Same<FindBranch<TreeA, A>::Result, TreeA>::Result::value, "failed");
      static_assert(Same<FindBranch<TreeA, AD>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<FindBranch<TreeA, ADAB>::Result, TreeADAB>::Result::value, "failed");
      
      static_assert(Same<Root<TreeA>::Result, A>::Result::value, "failed");
      
      static_assert(Same<Children<TreeA, A>::Result, List<AA, AB, AC, AD>>::Result::value, "failed");
      static_assert(Same<Children<TreeA, AB>::Result, List<ABA, ABB, ABC>>::Result::value, "failed");
      
      static_assert(Same<Ancestors<TreeA, A>::Result, List<>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, AB>::Result, List<A>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, ABC>::Result, List<AB, A>>::Result::value, "failed");
      static_assert(Same<Ancestors<TreeA, ADAB>::Result, List<ADA, AD, A>>::Result::value, "failed");
      
      static_assert(Same<SelfAndAncestors<TreeA, A>::Result, List<A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, AB>::Result, List<AB, A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ABC>::Result, List<ABC, AB, A>>::Result::value, "failed");
      static_assert(Same<SelfAndAncestors<TreeA, ADAB>::Result, List<ADAB, ADA, AD, A>>::Result::value, "failed");
      
      static_assert(Same<Parent<TreeA, AB>::Result, A>::Result::value, "failed");
      static_assert(Same<Parent<TreeA, ABB>::Result, AB>::Result::value, "failed");
      static_assert(Same<Parent<TreeA, ADAB>::Result, ADA>::Result::value, "failed");
      
      static_assert(Same<CommonBranch<TreeA, ADAB, AAB>::Result, TreeA>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAB, ADB>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, ADAB>::Result, TreeADA>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, ADAA, AD>::Result, TreeAD>::Result::value, "failed");
      static_assert(Same<CommonBranch<TreeA, AD, AD>::Result, TreeAD>::Result::value, "failed");
      
      static_assert(Same<CommonAncestor<TreeA, ADAB, AAB>::Result, A>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAB, ADB>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, ADAB>::Result, ADA>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, ADAA, AD>::Result, AD>::Result::value, "failed");
      static_assert(Same<CommonAncestor<TreeA, AD, AD>::Result, AD>::Result::value, "failed");
      
      static_assert(Same<Flatten<TreeA>::Result, List<A, AA, AAA, AAB, AB, ABA, ABB, ABC, AC, AD, ADA, ADAA, ADAB, ADB>>::Result::value, "failed");
      
      static_assert(Same<Index<TreeA, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AAA>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AAB>::Result, Int<3>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, AB>::Result, Int<4>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, ABA>::Result, Int<5>>::Result::value, "failed");
      static_assert(Same<Index<TreeA, ABB>::Result, Int<6>>::Result::value, "failed");
      
      static_assert(Same<Leaves<TreeA>::Result, List<AAA, AAB, ABA, ABB, ABC, AC, ADAA, ADAB, ADB>>::Result::value, "failed");
      
      template <typename Type> using IsADA = Same<ADA, Type>;
      static_assert(Same<Find<TreeA, IsADA>::Result, ADA>::Result::value, "failed");
      
      static_assert(Same<Height<TreeA>::Result, Int<3>>::Result::value, "failed");
      static_assert(Same<Height<TreeAA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Height<TreeAC>::Result, Int<0>>::Result::value, "failed");
      
      static_assert(Same<Depth<TreeA, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Depth<TreeA, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Depth<TreeA, ADAA>::Result, Int<3>>::Result::value, "failed");
      
      static_assert(Same<Distance<TreeA, A, A>::Result, Int<0>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, A, AA>::Result, Int<1>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, A, AAA>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, AA, AB>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, ABA, ABC>::Result, Int<2>>::Result::value, "failed");
      static_assert(Same<Distance<TreeA, ADAB, ABB>::Result, Int<5>>::Result::value, "failed");
    }
    namespace HFSMs {
      using namespace HFSM;
      
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
        
        static_assert(Same<DefaultPath<States>::Result, List<PluggedIn, Active, Stopped>>::Result::value, "failed");
        static_assert(Same<DefaultPath<ActiveTree>::Result, List<Active, Stopped>>::Result::value, "failed");
        static_assert(Same<DefaultPath<PlayingTree>::Result, List<Playing, NormalSpeed>>::Result::value, "failed");
        
        template <typename> static void enter() {}
        template <typename> static void exit() {}
        template <typename, typename, typename> struct Guard;
        template <typename, typename, typename> struct Action;
      };
      
      bool standby_light = false;
      template <> void VCR::enter<VCR::Standby>() { standby_light = true; }
      template <> void VCR::exit<VCR::Standby>() { standby_light = false; }
      
      bool five_minute_timer_started = false;
      template <> void VCR::enter<VCR::Paused>() { five_minute_timer_started = true; }
      template <> void VCR::exit<VCR::Paused>() { five_minute_timer_started = false; }
      
      struct PowerButton; struct PlayButton; struct PauseButton; struct ForwardButton; struct StopButton;
      
      bool battery_is_flat = false;
      bool startup_sound_played = false;
      template<> struct VCR::Action<VCR::Active, PowerButton, VCR::Standby> { void operator()() { } };
      template<> struct VCR::Guard<VCR::Standby, PowerButton, VCR::Active> {
        bool operator()() { return !battery_is_flat; }
      };
      template<> struct VCR::Action<VCR::Standby, PowerButton, VCR::Active> {
        void operator()() { startup_sound_played = true; }
      };
      template <> struct VCR::Action<VCR::Active, PlayButton, VCR::Playing> { void operator()() { } };
      template <> struct VCR::Action<VCR::Playing, PauseButton, VCR::Paused> { void operator()() { } };
      template <> struct VCR::Action<VCR::Paused, PauseButton, VCR::Playing> { void operator()() { } };
      template <> struct VCR::Action<VCR::NormalSpeed, ForwardButton, VCR::DoubleSpeed> { void operator()() { } };
      template <> struct VCR::Action<VCR::DoubleSpeed, ForwardButton, VCR::QuadSpeed> { void operator()() { } };
      template <> struct VCR::Action<VCR::QuadSpeed, ForwardButton, VCR::HighSpeed> { void operator()() { } };
      template <> struct VCR::Action<VCR::HighSpeed, ForwardButton, VCR::DoubleSpeed> { void operator()() { } };
      template <> struct VCR::Action<VCR::Stopped, ForwardButton, VCR::FastForwarding> { void operator()() { } };
      template <> struct VCR::Action<VCR::Playing, StopButton, VCR::Stopped> { void operator()() { } };
      template <> struct VCR::Action<VCR::FastForwarding, StopButton, VCR::Stopped> { void operator()() { } };
      template <> struct VCR::Action<VCR::Paused, StopButton, VCR::Stopped> { void operator()() { } };
      
      static_assert(RespondsTo<VCR, PowerButton>::Result::value, "failed");
      static_assert(RespondsTo<VCR, PlayButton>::Result::value, "failed");
      static_assert(RespondsTo<VCR, PauseButton>::Result::value, "failed");
      static_assert(RespondsTo<VCR, ForwardButton>::Result::value, "failed");
      static_assert(RespondsTo<VCR, StopButton>::Result::value, "failed");
      static_assert(!RespondsTo<VCR, bool>::Result::value, "failed");
      
      void test() {
        bool test;
        
        Initialise<VCR>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(test = !CurrentState<VCR>::Test<VCR::Paused>()());
        
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Standby>()());
        assert(standby_light);
        
        startup_sound_played = false;
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(!standby_light);
        assert(startup_sound_played);
        
        Dispatch<VCR, PlayButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        Dispatch<VCR, ForwardButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::DoubleSpeed>()());
        
        Dispatch<VCR, ForwardButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::QuadSpeed>()());
        
        Dispatch<VCR, ForwardButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::HighSpeed>()());
        
        Dispatch<VCR, ForwardButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::DoubleSpeed>()());
        
        Dispatch<VCR, PlayButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        Dispatch<VCR, PauseButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Paused>()());
        assert(five_minute_timer_started);
        
        Dispatch<VCR, PauseButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        assert(!five_minute_timer_started);
        
        Dispatch<VCR, ForwardButton>()();
        Dispatch<VCR, ForwardButton>()();
        Dispatch<VCR, PlayButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Standby>()());
        assert(standby_light);
        
        startup_sound_played = false;
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(!standby_light);
        assert(startup_sound_played);
        
        Dispatch<VCR, ForwardButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::FastForwarding>()());
        
        Dispatch<VCR, StopButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        
        Dispatch<VCR, PlayButton>()();
        Dispatch<VCR, StopButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        
        Dispatch<VCR, ForwardButton>()();
        Dispatch<VCR, PlayButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::NormalSpeed>()());
        
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Standby>()());
        
        startup_sound_played = false;
        battery_is_flat = true;
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Standby>()());
        assert(!startup_sound_played);
        
        battery_is_flat = false;
        Dispatch<VCR, PowerButton>()();
        assert(test = CurrentState<VCR>::Test<VCR::Stopped>()());
        assert(startup_sound_played);
      }
    }
    namespace Scheduling {
      using namespace Scheduler;
      using namespace HFSM;
      
      struct A; struct AA; struct AB; struct AC;
      struct M1 {
        typedef Tree<A, Tree<AA>, Tree<AB>, Tree<AC>> States;
        template <typename> static void enter() { }
        template <typename> static void exit() { }
        template <typename, typename, typename> struct Guard;
        template <typename, typename, typename> struct Action;
      };
      
      struct X; struct XX; struct XY; struct XZ;
      struct M2 {
        typedef Tree<X, Tree<XX>, Tree<XY>, Tree<XZ>> States;
        template <typename> static void enter() { }
        template <typename> static void exit() { }
        template <typename, typename, typename> struct Guard;
        template <typename, typename, typename> struct Action;
      };
      
      typedef List<M1, M2> Machines;
      
      struct Context {
        static void (*preempt)();
        static void prepare(void (*p)()) { preempt = p; }
        static void push() { preempt(); }
        static void pop() { }
      };
      void (*Context::preempt)();
      
      unsigned int actions;
      void push(int n) { actions *= 10; actions += n; }
      
      struct E1; struct E2; struct E3; struct E4; struct E5;
      
      // template <> struct M1::Action<A, E1, AB> { void operator()(); };
      // template <> struct M1::Action<A, E2, AC> { void operator()(); };
      // template <> struct M1::Action<A, E4, AA> { void operator()(); };
      // template <> struct M2::Action<X, E1, XY> { void operator()(); };
      // template <> struct M2::Action<X, E3, XZ> { void operator()(); };
      // template <> struct M2::Action<X, E5, XX> { void operator()(); };
      //   
      // void M1::Action<A, E1, AB>::operator()() { push(1); }
      // void M1::Action<A, E2, AC>::operator()() { Post<Machines, Context, E3>()(); push(2); }
      // void M1::Action<A, E4, AA>::operator()() { push(3); }
      // void M2::Action<X, E1, XY>::operator()() { push(5); }
      // void M2::Action<X, E3, XZ>::operator()() { push(6); }
      // void M2::Action<X, E5, XX>::operator()() { Post<Machines, Context, E4>()(); push(7); }
      
      // TODO: forward declarations work OK but will be a problem for multiple machines in separate headers...
      // (can we work around this by not full specializing? e.g. dummy template parameter? )
      
      // TODO: change enters and exits into class templates as per above!
      
      template <> struct M1::Action<A, E1, AB> { void operator()() { push(1); } };
      template <> struct M1::Action<A, E2, AC> { void operator()() { Post<Machines, Context, E3>()(); push(2); } };
      template <> struct M1::Action<A, E4, AA> { void operator()() { push(3); } };
      template <> struct M2::Action<X, E1, XY> { void operator()() { push(5); } };
      template <> struct M2::Action<X, E3, XZ> { void operator()() { push(6); } };
      template <> struct M2::Action<X, E5, XX> { void operator()() { Post<Machines, Context, E4>()(); push(7); } };
      
      void test() {
        Scheduler::Initialise<Machines, Context>()();
        
        actions = 0;
        Post<Machines, Context, E1>()();
        assert(actions == 15);
        
        actions = 0;
        Post<Machines, Context, E2>()();
        assert(actions == 26);
        
        actions = 0;
        Post<Machines, Context, E5>()();
        assert(actions == 37);
      }
    }
  }
}
