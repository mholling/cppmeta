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
    namespace ForLists {
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
      
      bool a[4] = { false };
      template <typename I>
      struct SetA {
        typedef SetA Result;
        void operator()() { a[I::value] = true; }
      };
      
      template <typename I>
      struct GetA {
        typedef GetA Result;
        bool operator()() { return a[I::value]; }
      };
      
      
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
        assert(x == 10 && y == 10 && z == 10);
        
        Each<List<Int<1>, Int<3>>, SetA>()();
        assert(!a[0] && a[1] && !a[2] && a[3]);
        
        count = 0;
        bool result;
        assert(result = Until<SomeSuccesses>()());
        assert(count == 3);
        
        count = 0;
        assert(result = !Until<AllFailures>()());
        assert(count == 3);
        
        result = Until<List<Int<0>, Int<1>, Int<2>, Int<3>>, GetA>()();
        assert(result);
        
        result = !Until<List<Int<0>, Int<2>>, GetA>()();
        assert(result);
        
        count = 0;
        assert(result = While<SucceedTwice>()());
        assert(count == 2);
        
        count = 0;
        assert(result = !While<SucceedTwiceThenFail>()());
        assert(count == 3);
        
        result = While<List<Int<1>, Int<3>>, GetA>()();
        assert(result);
        
        result = !While<List<Int<1>, Int<2>>, GetA>()();
        assert(result);
      }
    };
    namespace ForTrees {
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
    namespace ForQueues {
      struct Owner; struct Owner2;
      typedef Queue::Node<Owner, int> Ints;
      typedef Queue::Node<Owner, int, char> IntsChars;
      typedef Queue::Node<Owner2, int> Ints2;
      
      void test() {
        bool result;
        
        assert(result = !Ints::any());
        assert(result = Ints::Enqueue<5>()());
        assert(result = Ints::Enqueue<1>()());
        assert(result = Ints::Enqueue<3>()());
        assert(result = Ints::any());
        
        assert(result = Ints::dequeue() == 5);
        assert(result = Ints::dequeue() == 1);
        assert(result = Ints::Enqueue<5>()());
        assert(result = Ints::dequeue() == 3);
        assert(result = Ints::dequeue() == 5);
        assert(result = !Ints::any());
        
        assert(result = Ints::Enqueue<2>()());
        assert(result = !Ints::Enqueue<2>()());
        
        assert(result = Ints::any());
        assert(result = !Ints2::any());
        
        IntsChars::Enqueue<10, 'a'>()();
        IntsChars::Enqueue<11, 'b'>()();
        IntsChars::Enqueue<12, 'c'>()();
        
        int i = 0;
        char c = 0;
        IntsChars::dequeue(i, c);
        assert(i == 10 && c == 'a');
        IntsChars::dequeue(i, c);
        assert(i == 11 && c == 'b');
        IntsChars::dequeue(i, c);
        assert(i == 12 && c == 'c');
      }
    }
    namespace ForHFSMs {
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
        
        static_assert(Same<HFSM::DefaultPath<States>::Result, List<PluggedIn, Active, Stopped>>::Result::value, "failed");
        static_assert(Same<HFSM::DefaultPath<ActiveTree>::Result, List<Active, Stopped>>::Result::value, "failed");
        static_assert(Same<HFSM::DefaultPath<PlayingTree>::Result, List<Playing, NormalSpeed>>::Result::value, "failed");
        
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
      
      static_assert(HFSM::RespondsTo<VCR, PowerButton>::Result::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, PlayButton>::Result::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, PauseButton>::Result::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, ForwardButton>::Result::value, "failed");
      static_assert(HFSM::RespondsTo<VCR, StopButton>::Result::value, "failed");
      static_assert(!HFSM::RespondsTo<VCR, bool>::Result::value, "failed");
      
      struct Kernel { template <typename Event> static void post() { } };
      
      bool entering_standby, leaving_standby, entering_pause, leaving_pause, activating;
      template <> void Kernel::post<EnteringStandby>() { entering_standby = true; }
      template <> void Kernel::post<LeavingStandby>() { leaving_standby = true; }
      template <> void Kernel::post<EnteringPause>() { entering_pause = true; }
      template <> void Kernel::post<LeavingPause>() { leaving_pause = true; }
      template <> void Kernel::post<Activating>() { activating = true; }
      
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
      }
    }
    namespace ForScheduler {
      unsigned int actions;
      void action(int n) { actions *= 10; actions += n; }
      
      struct E1; struct E2; struct E3; struct E4; struct E5;
      
      struct A; struct AA; struct AB; struct AC;
      struct M1{
        typedef Tree<A, Tree<AA>, Tree<AB>, Tree<AC>> States;
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
        typedef Tree<X, Tree<XX>, Tree<XY>, Tree<XZ>> States;
        template <typename, typename, typename, typename> struct Guard;
        template <typename, typename, typename, typename> struct Action;
        template <typename, typename> struct Enter;
        template <typename, typename> struct Exit;
      };
      
      template <typename Kernel> struct M2::Action<Kernel, X, E1, XY> { void operator()() { action(5); } };
      template <typename Kernel> struct M2::Action<Kernel, X, E3, XZ> { void operator()() { action(6); } };
      template <typename Kernel> struct M2::Action<Kernel, X, E5, XX> { void operator()() { Kernel::template post<E4>(); action(7); } };
      
      typedef List<M1, M2> Machines;
      
      struct Kernel {
        struct Context {
          static void (*preempt)();
          static void prepare(void (*p)()) { preempt = p; }
          static void push() { preempt(); }
          static void pop() { }
          static void enable() { }
        };
        template <typename Event> static void post() { Scheduler::Post<Kernel, Machines, Event>()(); }
      };
      void (*Kernel::Context::preempt)();
      
      void test() {
        Scheduler::Initialise<Kernel, Machines>()();
        
        actions = 0;
        Kernel::post<E1>();
        assert(actions == 15);
        
        actions = 0;
        Kernel::post<E2>();
        assert(actions == 26);
        
        actions = 0;
        Kernel::post<E5>();
        assert(actions == 37);
      }
    }
    namespace ForOS {
      struct Context {
        static void (*preempt)();
        static void prepare(void (*p)()) { preempt = p; }
        static void push() { preempt(); }
        static void pop() { }
        static void enable() { }
      };
      void (*Context::preempt)();
      
      struct Irq1; struct Irq2; struct Irq3;
      
      struct D1 {
        template <typename Kernel, typename Interrupt> struct Handle;
        typedef List<> Dependencies;
        template <typename Kernel> struct Initialise { void operator()() { } };
      };
      template <typename Kernel>
      struct D1::Handle<Kernel, Irq3> {
        void operator()() { volatile int x = 666; ++x; }
      };
      
      struct D2 {
        template <typename Kernel, typename Interrupt> struct Handle;
        typedef List<D1> Dependencies;
        template <typename Kernel> struct Initialise { void operator()() { } };
      };
      template <typename Kernel>
      struct D2::Handle<Kernel, Irq3> {
        void operator()() { volatile int x = 222; ++x; }
      };
      
      typedef List<D2> Drivers;
      typedef List<> Machines;
      typedef List<Irq1, Irq2, Irq3> Interrupts;
      
      typedef OS::Kernel<Context, Drivers, Machines> Kernel;
      typedef OS::VectorTable<Kernel, Interrupts> VectorTable;
      
      __attribute__ ((used, section (".isr_vector")))
      const VectorTable vt;
      
      void test() {
      }
    }
  }
}
