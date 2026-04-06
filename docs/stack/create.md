# Creating a Stack

To create a stack, we need two things:

- A **node** to store each element on the heap.
- A stack **struct** that holds a pointer to the top and tracks the size.

An empty stack is simply a stack with `top` pointing to nothing and `size` set to zero.

## In C

```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    int size;
} Stack;

// Creates a new empty stack
Stack create_stack() {
    Stack s;
    s.top = NULL;
    s.size = 0;
    return s;
}

// Usage
Stack s = create_stack();
```

`Node` is the same node that we used in our linked list. It has an `int data` field to store the value and a `next` pointer to point to the next node in the stack, which can be thought as the node "below" it.

`Stack` wraps a `top` pointer and a `size` counter.

`create_stack()` returns a stack with `top = NULL` and `size = 0`, representing an empty stack.

::: info Why a linked list?
A stack is just an **abstract idea**. We need an actual data structure to implement this concept. We use a **linked list** because pushing and popping from the top is O(1), since we can just update the `top` pointer. If we used an array, we'd have to worry about resizing when it fills up.
:::

::: tip
`size` is optional. We _could_ remove it and the stack would still work. But keeping track of it lets us get the size of the stack instantly without traversing it.
:::

## In Rust

For these notes, we will not use Rust's `Vec` or `VecDeque` from the standard library, but instead implement our own stack from scratch (cuz it's more fun that way).

```rust
struct Node {
    data: i32,
    next: Option<Box<Node>>,
}

struct Stack {
    top: Option<Box<Node>>,
    size: usize,
}

impl Stack {
    fn new() -> Stack {
        Stack {
            top: None,
            size: 0,
        }
    }
}

// Usage
let mut s = Stack::new();
```

`Stack::new()` returns an empty stack with `top = None` and `size = 0`.

Notice that we will implement methods on `Stack` instead of `Node`, since we will be operating on the **stack**, not the node directly.

::: tip i32 vs usize
We use `usize` instead of `i32` for size because a stack can **never** have a negative number of elements. If we used `i32`, we could have a **negative** size, which doesn't make sense. `usize` is also the standard type for sizes and lengths in Rust, like `Vec::len()` and other standard library methods.
:::

## Key Difference

|             | C                          | Rust                     |
| ----------- | -------------------------- | ------------------------ |
| Empty stack | `Stack s = create_stack()` | `let s = Stack::new()`   |
| Top pointer | `Node *top`                | `top: Option<Box<Node>>` |
| Size field  | `int size`                 | `usize size`             |
