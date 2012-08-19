#ifndef CPPMETA_TEST_QUEUE_H
#define CPPMETA_TEST_QUEUE_H

#include <assert.h>

#include "meta/queue.h"

namespace CppMeta {
  namespace Test {
    namespace TestQueue {
      struct Owner; struct Owner2;
      using Ints = Queue::Node<Owner, int>;
      using IntsChars = Queue::Node<Owner, int, char>;
      using Ints2 = Queue::Node<Owner2, int>;
      
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
  }
}

#endif
