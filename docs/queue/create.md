# Creating a Queue

To create a queue, we need three things:

- A **node** to store each element on the heap
- A **front pointer** that points to the front of the queue
- A **back pointer** that points to the back of the queue

An empty queue is simply a queue with both `front` and `back` pointing to nothing and `size` set to zero.

## In C

```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *back;
    int size;
} Queue;

// Creates a new empty queue
Queue create_queue() {
    Queue q;
    q.front = NULL;
    q.back = NULL;
    q.size = 0;
    return q;
}

// Usage
Queue q = create_queue();
```

`front` points to the first node in the queue. This is where we dequeue from.

`back` points to the last node in the queue. This is where we enqueue to.

`create_queue()` returns an empty queue with `front = NULL`, `back = NULL` and `size = 0`.

::: tip
Just like with our stack, `size` is optional but lets us check `isEmpty()` and get the size of the queue in O(1) without traversing it.
:::

## In Rust

Same with our stack implementation, we will not use Rust's `Vec` or `VecDeque` from the standard library, but instead implement our own queue from scratch.

But this is where things get interesting. Rust's ownership model means we can't have two pointers owning the same data. So we need to use a combination of `Option<Box<Node>>` for the front and a **raw pointer** for the back. Luckily, this isnt as _bad_ as [doubly linked list](../linked-list/doubly/node.md) because we only need to point to the last node, not each node pointing the previous node.

```rust
struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

struct Queue<T> {
    front: Option<Box<Node<T>>>,
    back: *mut Node<T>,
    size: usize,
}

impl<T> Queue<T> {
    fn new() -> Queue<T> {
        Queue {
            front: None,
            back: std::ptr::null_mut(),
            size: 0,
        }
    }
}

// Usage
let mut q: Queue<i32> = Queue::new();
```

`front` is an `Option<Box<Node>>`, it owns the entire list from front to back.

`back` is a `*mut Node` raw pointer, it points directly to the last node so we can enqueue in O(1) without traversing.

::: info Why do we need a raw pointer for the back?
`front` owns the linked list through `Box<Node>`, with each node owns the next one through `Option<Box<Node>>`. If `back` also tried to own the last node, we'd have two owners for the same node, which Rust doesn't allow.

Instead, `back` is just a raw pointer that points to the last node without owning it. `front` owns the whole queue and `back` is just a shortcut to the end.
:::

::: warning
Because `back` is a raw pointer, we need to be careful to keep it in sync with `front`. When the queue is empty, `back` must be `null_mut()`. When we enqueue or dequeue, we must update both pointers correctly.
:::

## Key Difference

|                 | C              | Rust                      |
| --------------- | -------------- | ------------------------- |
| Empty queue     | `front = NULL` | `front = None`            |
| Front pointer   | `Node *front`  | `Option<Box<Node>>`       |
| Back pointer    | `Node *back`   | `*mut Node` (raw pointer) |
| NULL equivalent | `NULL`         | `std::ptr::null_mut()`    |
| Size field      | `int size`     | `usize size`              |
