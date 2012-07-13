#include <cstdio>
#include <assert.h>
#include "meta.h"
#include "list.h"
#include "tree.h"
#include "tests.h"
#include "hfsm.h"
#include "scheduler.h"

using namespace Meta;
using namespace HFSM;
using namespace Scheduler;

int x = 0, y = 0, z = 0, count = 0;
void test_list() {
  struct SetX { void operator()(int v) { x = v; } };
  struct SetY { void operator()(int v) { y = v; } };
  struct SetZ { void operator()(int v) { z = v; } };
  typedef List<SetX, SetY, SetZ> Setters;

  Each<List<SetX, SetY, SetZ>>()(10);
  assert(x == 10);
  assert(x == 10);
  assert(x == 10);

  struct StartFalse { bool operator()() { count++; return false; } };
  struct StillFalse { bool operator()() { count++; return false; } };
  struct NowTrue    { bool operator()() { count++; return true; } };
  struct TrueAgain  { bool operator()() { count++; return true; } };
  struct FinalFalse { bool operator()() { count++; return false; } };
  typedef List<StartFalse, StillFalse, NowTrue, TrueAgain, FinalFalse> Success;
  typedef List<StartFalse, StillFalse, FinalFalse> Failure;
  typedef List<NowTrue, TrueAgain, FinalFalse> SucceedTwiceThenFail;
  typedef List<NowTrue, TrueAgain> SucceedTwice;
  
  count = 0;
  bool result = Until<Success>()();
  assert(result);
  assert(count == 3);

  count = 0;
  result = Until<Failure>()();
  assert(!result);
  assert(count == 3);
  
  count = 0;
  result = While<SucceedTwice>()();
  assert(result);
  assert(count == 2);
  
  count = 0;
  result = While<SucceedTwiceThenFail>()();
  assert(!result);
  assert(count == 3);
}

struct Event1; struct Event2; struct Event3; struct Event4;

struct A; struct AA; struct AAA; struct AAB; struct AB; struct AC; struct ACA;
struct Machine1 {
      typedef Tree<AAA> TreeAAA;
      typedef Tree<AAB> TreeAAB;
    typedef Tree<AA, TreeAAA, TreeAAB> TreeAA;
    typedef Tree<AB> TreeAB;
      typedef Tree<ACA> TreeACA;
    typedef Tree<AC, TreeACA> TreeAC;
  typedef Tree<A, TreeAA, TreeAB, TreeAC> States;
  
  template <typename Source, typename Event, typename Target> struct Transition;
  template <typename State> static void enter() { };
  template <typename State> static void exit() { };
};

struct X; struct XX; struct XY;
struct Machine2 {
  typedef Tree<X, Tree<XX>, Tree<XY>> States;
  template <typename Source, typename Event, typename Target> struct Transition;
  template <typename State> static void enter() { };
  template <typename State> static void exit() { };
};

typedef List<Machine1, Machine2> Machines;

bool aa_aab_transition;
template <> struct Machine1::Transition<AA, Event1, AAB> {
  static bool guard() { return true; }
  static void action() { aa_aab_transition = true; printf("AA -> AAB\n"); }
};

bool aab_ac_transition;
template <> struct Machine1::Transition<AA, Event2, AC> {
  static bool guard() { return true; }
  static void action() { aab_ac_transition = true; printf("AA -> AC\n"); }
};

bool a_aab_transition;
template <> struct Machine1::Transition<A, Event3, AAB> {
  static bool guard() { return true; }
  static void action() { a_aab_transition = true; printf("A -> AAB\n"); }
};

static_assert(Same<DefaultPath<Machine1::States>::Result, List<A, AA, AAA>>::Result::value, "failed");
static_assert(RespondsTo<Machine1, Event1>::Result::value, "failed");
static_assert(!RespondsTo<Machine1, Event4>::Result::value, "failed");

bool enter_a, enter_aa, enter_aaa, enter_aab, enter_ab, enter_ac, enter_aca;
bool exit_a, exit_aa, exit_aaa, exit_aab, exit_ab, exit_ac, exit_aca;

template <> void Machine1::enter<A>() { enter_a = true; printf("Entering A\n"); }
template <> void Machine1::enter<AA>() { enter_aa = true; printf("Entering AA\n"); }
template <> void Machine1::enter<AAA>() { enter_aaa = true; printf("Entering AAA\n"); }
template <> void Machine1::enter<AAB>() { enter_aab = true; printf("Entering AAB\n"); }
template <> void Machine1::enter<AB>() { enter_ab = true; printf("Entering AB\n"); }
template <> void Machine1::enter<AC>() { enter_ac = true; printf("Entering AB\n"); }
template <> void Machine1::enter<ACA>() { enter_aca = true; printf("Entering ACA\n"); }
template <> void Machine1::exit<A>() { exit_a = true; printf("Exiting A\n"); }
template <> void Machine1::exit<AA>() { exit_aa = true; printf("Exiting AA\n"); }
template <> void Machine1::exit<AAA>() { exit_aaa = true; printf("Exiting AAA\n"); }
template <> void Machine1::exit<AAB>() { exit_aab = true; printf("Exiting AAB\n"); }
template <> void Machine1::exit<AB>() { exit_ab = true; printf("Exiting AB\n"); }
template <> void Machine1::exit<AC>() { exit_ac = true; printf("Exiting AC\n"); }
template <> void Machine1::exit<ACA>() { exit_aca = true; printf("Exiting ACA\n"); }

template <> struct Machine2::Transition<XX, Event1, XY> {
  static bool guard() { return true; }
  static void action() { printf("XX -> XY\n"); }
};

struct Context {
  static void (*preempt)();
  static void prepare(void (*p)()) { preempt = p; }
  static void push() { preempt(); }
  static void pop() { }
};
void (*Context::preempt)();

template <> struct Machine2::Transition<XY, Event2, XX> {
  static bool guard() { return true; }
  static void action() { printf("XY -> XX, firing event 3\n"); Post<Machines, Event3, Context>()(); }
};

bool enter_x, enter_xx, enter_xy, exit_x, exit_xx, exit_xy;

template <> void Machine2::enter<X>() { enter_x = true; printf("Entering X\n"); }
template <> void Machine2::enter<XX>() { enter_xx = true; printf("Entering XX\n"); }
template <> void Machine2::enter<XY>() { enter_xy = true; printf("Entering XY\n"); }
template <> void Machine2::exit<X>() { exit_x = true; printf("Exiting X\n"); }
template <> void Machine2::exit<XX>() { exit_xx = true; printf("Exiting XX\n"); }
template <> void Machine2::exit<XY>() { exit_xy = true; printf("Exiting XY\n"); }


void test_hfsm() {
  Scheduler::Initialise<Machines, Context>()();
  Post<Machines, Event1, Context>()();
  Post<Machines, Event2, Context>()();
  Post<Machines, Event3, Context>()();
}

int main() {
  test_list();
  test_hfsm();
}
