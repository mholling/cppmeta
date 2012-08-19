#ifndef CPPMETA_TEST_H
#define CPPMETA_TEST_H

#include "test-value.h"
#include "test-meta.h"
#include "test-list.h"
#include "test-many.h"
#include "test-tree.h"
#include "test-queue.h"
#include "test-hfsm.h"
#include "test-scheduler.h"
#include "test-os.h"

namespace CppMeta {
  namespace Test {
    void test() {
      TestMeta::test();
      TestList::test();
      TestQueue::test();
      TestHFSM::test();
      TestScheduler::test();
      TestOS::test();
    }
  }
}

#endif
