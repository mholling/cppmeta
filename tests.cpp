#include <assert.h>
#include <cstdio>
#include "meta.h"
#include "list.h"
#include "tree.h"
#include "queue.h"
#include "hfsm.h"
#include "scheduler.h"
#include "os.h"
#include "tests.h"

using namespace CppMeta::Tests;

int main() {
  Basics::test();
  ForLists::test();
  ForQueues::test();
  ForHFSMs::test();
  ForScheduler::test();
  ForOS::test();
}
