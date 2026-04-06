# Nodes in Doubly Linked List

A node in a doubly linked list is similar to a node in a singly linked list, but instead of only having one pointer `next`, it has **two** pointers: `next` (a pointer to the next node) and `prev` (a pointer to the previous node).

## In C

```c
typedef struct Node {
    int data;
    struct Node *next;  // Pointer to the next node
    struct Node *prev;  // Pointer to the previous node
} Node;

// Creates a new node on the heap
Node* create_node(int data) {
    // Allocate memory for a new node
    Node *node = (Node *)malloc(sizeof(Node));

    // Set the data
    node->data = data;

    // Initially, the next and prev doesn't point to anything
    node->next = NULL;
    node->prev = NULL;

    return node;
}
```

The `prev` pointer lets us traverse backwards through the list, which is the main advantage of doubly linked lists.

We now also have to do `node->prev = NULL` to initialize the `prev` pointer to `NULL`.

## In Rust?

Yeah imma be honest, I tried to implement this in Rust, but then John Rust (Ferris) broke into my house at 3 AM and told me to stop cuz I was violating every rule known in Rust.

So... for doubly linked lists, **no Rust version**.

::: tip Why Doubly Linked Lists Break Rust
Each node needs both `next` and `prev` pointers. But if Node A points to Node B, and Node B points to Node A, we'd have **circular ownership**.

Rust's entire safety model is built on **single ownership**. Doubly linked lists fundamentally violate this. It _is_ possible to make them work with `Rc<RefCell<Node>>` or by spamming raw pointers and `unsafe` code, but it's very painful and un-idiomatic.
:::
