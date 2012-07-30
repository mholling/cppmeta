namespace CppMeta {
  namespace Queue {
    template <typename...> struct Tuple;
    template <typename Type, typename... Types>
    struct Tuple<Type, Types...> {
      Type value;
      Tuple<Types...> values;
      Tuple(Type value, Types... values) : value(value), values(values...) { }
      void get(Type &val, Types&... vals) { val = value; values.get(vals...); }
    };
    template <> struct Tuple<> { void get() { } };
    
    template <typename Owner, typename... Types>
    class Node {
      Tuple<Types...> contents;
      
      using Pointer = Node *;
      static Pointer head;
      Pointer next;
      
      Node(Types... contents) : contents(contents...), next(nullptr) { }
      
      bool push(Pointer &after = head) {
        return after == this ? false : after != nullptr ? push(after->next) : (after = this, next = nullptr, true);
      }
      
      static Node &pop() {
        Node &node = *head;
        head = node.next;
        node.next = nullptr;
        return node;
      }
      
    public:
      template <Types...>
      struct Enqueue {
        static Node node;
        bool operator()() { return node.push(); }
      };
      
      template <Types... values>
      static bool enqueue() { return Enqueue<values...>()(); }
      
      static void dequeue(Types&... vals) { pop().contents.get(vals...); }
      static auto dequeue() -> decltype(contents.value) { return pop().contents.value; }
      
      static bool any() { return head != nullptr; }
    };
    
    template <typename Owner, typename... Types>
    typename Node<Owner, Types...>::Pointer Node<Owner, Types...>::head = nullptr;
    template <typename Owner, typename... Types> template <Types... vals>
    Node<Owner, Types...> Node<Owner, Types...>::Enqueue<vals...>::node(vals...);
  }
}
