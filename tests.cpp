#include <cstdio>
#include <assert.h>
#include "meta.h"
#include "list.h"
#include "tree.h"
#include "tests.h"
#include "hfsm.h"

using namespace Meta;
using namespace HFSM;

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

struct Event1; struct Event2; struct Event3;

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

bool aa_aab_transition;
template <> struct Machine1::Transition<AA, Event1, AAB> {
  static bool guard() { return true; }
  static void action() { aa_aab_transition = true; }
};

bool aab_ac_transition;
template <> struct Machine1::Transition<AA, Event2, AC> {
  static bool guard() { return true; }
  static void action() { aab_ac_transition = true; }
};

bool a_aab_transition;
template <> struct Machine1::Transition<A, Event2, AAB> {
  static bool guard() { return false; }
  static void action() { a_aab_transition = true; }
};

bool enter_a, enter_aa, enter_aaa, enter_aab, enter_ab, enter_ac, enter_aca;
bool exit_a, exit_aa, exit_aaa, exit_aab, exit_ab, exit_ac, exit_aca;

template <> void Machine1::enter<A>() { enter_a = true; }
template <> void Machine1::enter<AA>() { enter_aa = true; }
template <> void Machine1::enter<AAA>() { enter_aaa = true; }
template <> void Machine1::enter<AAB>() { enter_aab = true; }
template <> void Machine1::enter<AB>() { enter_ab = true; }
template <> void Machine1::enter<AC>() { enter_ac = true; }
template <> void Machine1::enter<ACA>() { enter_aca = true; }
template <> void Machine1::exit<A>() { exit_a = true; }
template <> void Machine1::exit<AA>() { exit_aa = true; }
template <> void Machine1::exit<AAA>() { exit_aaa = true; }
template <> void Machine1::exit<AAB>() { exit_aab = true; }
template <> void Machine1::exit<AB>() { exit_ab = true; }
template <> void Machine1::exit<AC>() { exit_ac = true; }
template <> void Machine1::exit<ACA>() { exit_aca = true; }

struct X; struct XX; struct XY;
struct Machine2 {
  typedef Tree<X, Tree<XX>, Tree<XY>> States;
  template <typename Source, typename Event, typename Target> struct Transition;
  template <typename State> static void enter() { };
  template <typename State> static void exit() { };
};

typedef List<Machine1, Machine2> Machines;

void test_hfsm() {
  bool test;
  
  Initialise<Machines>()();
  assert(test = CurrentState<Machine1>::Test<AAA>()());
  assert(test = !CurrentState<Machine1>::Test<A>()());
  assert(test = CurrentState<Machine2>::Test<XX>()());
  
  enter_a = enter_aa = enter_aaa = enter_aab = enter_ab = enter_ac = enter_aca = false;
  exit_a = exit_aa = exit_aaa = exit_aab = exit_ab = exit_ac = exit_aca = false;
  Dispatch<Machines, Event1>()();
  assert(test = CurrentState<Machine1>::Test<AAB>()());
  assert(aa_aab_transition);
  assert(exit_aaa && exit_aa && enter_aa && enter_aab);
  
  enter_a = enter_aa = enter_aaa = enter_ab = enter_ac = enter_aca = false;
  exit_a = exit_aa = exit_aaa = exit_ab = exit_ac = exit_aca = false;
  Dispatch<Machines, Event2>()();
  assert(test = CurrentState<Machine1>::Test<ACA>()());
  assert(aab_ac_transition);
  assert(exit_aab && exit_aa && exit_a && enter_a && enter_ac && enter_aca);
  
  enter_a = enter_aa = enter_aaa = enter_ab = enter_ac = enter_aca = false;
  exit_a = exit_aa = exit_aaa = exit_ab = exit_ac = exit_aca = false;
  Dispatch<Machines, Event3>()();
  assert(test = CurrentState<Machine1>::Test<ACA>()());
  assert(!a_aab_transition);
}

int main() {
  test_list();
  test_hfsm();
}
