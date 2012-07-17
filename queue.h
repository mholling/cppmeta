namespace CppMeta {
  namespace Queue {
    template <typename Type>
    struct Node {
      typedef Node *Pointer;
      Type type;
      Pointer next;
      Node(Type type) : type(type), next(nullptr) { }
      
      bool enqueue(Pointer &head) {
        return head == this ? false : head != nullptr ? enqueue(head->next) : (head = this, next = nullptr, true);
      }
      
      static Type dequeue(Pointer &head) {
        Node &node = *head;
        head = head->next;
        node.next = nullptr;
        return node.type;
      }
    };
    
    template <typename Type, typename... Owner>
    struct Head {
      static typename Node<Type>::Pointer head;
      
      template <Type t>
      struct Enqueue {
        static Node<Type> node;
        bool operator()() { return node.enqueue(head); }
      };
      
      static bool any() { return head != nullptr; }
      static Type dequeue() { return Node<Type>::dequeue(head); }
      template <Type t>
      static bool enqueue() { return Enqueue<t>()(); }
    };
    
    template <typename Type, typename... Owner>
    typename Node<Type>::Pointer Head<Type, Owner...>::head = nullptr;
    template <typename Type, typename... Owner> template <Type t>
    Node<Type> Head<Type, Owner...>::Enqueue<t>::node(t);
  }
}
