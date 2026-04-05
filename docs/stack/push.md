# Pushing an Element

Pushing adds a new element to the **top** of the stack. Pushing an element is very similar to inserting a node at the head of a linked list.

```
Before: TOP -> [10] -> [20] -> NULL

Step 1: Create a new node
        [30]

Step 2: Point the new node to the old top
        [30] -> [10] -> [20] -> NULL
                 ^
                TOP

Step 3: Update the top to point to the new node
TOP -> [30] -> [10] -> [20] -> NULL
```

## In C

```c
// Push a new node at the top of the list
void push(Stack *s, int data) {
    // Create a new node
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;

    // Point the new node to the old top
    node->next = s->top;

    // Update top to point to the new node
    s->top = node;

    // Increment the size
    s->size++;
}

// Usage
Stack s = create_stack();
push(&s, 10);    // TOP -> [10] -> NULL
push(&s, 20);    // TOP -> [20] -> [10] -> NULL
push(&s, 30);    // TOP -> [30] -> [20] -> [10] -> NULL
```

`Node *node = (Node *)malloc(sizeof(Node))` allocates a new node on the heap, just like we did in our linked list.

`node->next = s->top` makes the new node point to the current top, placing it "above" the existing stack.

`s->top = node` updates the top pointer to the new node.

`s->size++` increments the size counter.

::: warning
`malloc` can fail and return `NULL` if the system is out of memory. In production code we should always check if `node` is `NULL` before using it to avoid dereferencing a `NULL` pointer, which would crash the program.
:::

## In Rust

```rust
impl Stack {
    fn push(&mut self, data: i32) {
        // Create a new node
        let node = Box::new(Node {
            data,
            next: self.top.take(), // Take ownership of the current top and set it as the next node
        });

        // Update top to point to the new node
        self.top = Some(node);

        // Increment the size
        self.size += 1;
    }
}

// Usage
let mut s = Stack::new();
s.push(10);  // TOP -> [10] -> NONE
s.push(20);  // TOP -> [20] -> [10] -> NONE
s.push(30);  // TOP -> [30] -> [20] -> [10] -> NONE
```

`&mut self` gives the method mutable access to the stack so we can update `top` and `size`.

`self.top.take()` takes **ownership** of the current top and moves it into the new node's `next` field, leaving `self.top` as `None`. This is the same as writing `node->next = s->top` in C. The difference is that instead of copying a pointer, we're transferring ownership.

`self.top = Some(node)` updates the top pointer to the new node.

`self.size += 1` increments the size counter.

## Key Difference

|                    | C                       | Rust                     |
| ------------------ | ----------------------- | ------------------------ |
| Allocate node      | `malloc(sizeof(Node))`  | `Box::new(Node { ... })` |
| Link to old top    | `node->next = s->top`   | `next: self.top.take()`  |
| Update top         | `s->top = node`         | `self.top = Some(node)`  |
| Increment size     | `s->size++`             | `self.size += 1`         |
| Allocation failure | Returns `NULL` silently | Panics immediately       |
