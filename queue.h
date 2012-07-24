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
    
    template <typename Owner, typename Type>
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
    
    template <typename Owner, typename Type>
    typename Node<Type>::Pointer Head<Owner, Type>::head = nullptr;
    template <typename Owner, typename Type> template <Type t>
    Node<Type> Head<Owner, Type>::Enqueue<t>::node(t);
  }
}
