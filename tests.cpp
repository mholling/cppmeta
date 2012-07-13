#include <assert.h>
#include <cstdio>
#include "meta.h"
#include "list.h"
#include "tree.h"
#include "hfsm.h"
#include "scheduler.h"
#include "tests.h"

using namespace CppMeta::Tests;

int main() {
  Lists::test();
  HFSMs::test();
  Scheduling::test();
}
