# Creating a Linked List

To create a linked list, we need two things:

- A way to **create a node** and allocate it on the heap
- A **head pointer** that points to the first node

An empty linked list is simply a head pointer that points to nothing.

## In C

```c
// Function that creates a new node on the heap
Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

// Start with an empty list first, so just a head pointing to NULL
Node *head = NULL;

// Create three nodes
Node *a = create_node(1);
Node *b = create_node(2);
Node *c = create_node(3);

// Link them together manually
a->next = b;
b->next = c;
// c->next is already NULL, so c is the end of list

// Point head at the first node
Node *head = a;
```

After linking, the linked list looks like this:

```
head --> [1 | next] --> [2 | next] --> [3 | NULL]
```

`a->next = b` links node `a` to node `b` by storing `b`'s memory address in `a`'s next pointer. Same goes with node `b` and node `c`. <br>
`head` is just a pointer that points to node `a`, it is not a node itself.

::: warning
`malloc` can fail and return `NULL` if the system is out of memory. In production code you should always check:

```c
if (node == NULL) {
    // handle error
}
```

:::

## In Rust

```rust
// Implement a constructor for Node that returns a heap allocated Box<Node>
impl Node {
    fn new(data: i32) -> Box<Node> {
        Box::new(Node { data, next: None })
    }
}

// Create three nodes
let mut a = Node::new(1);
let mut b = Node::new(2);
let c = Node::new(3);

// Link them together manually
b.next = Some(c);
a.next = Some(b);

// head is just a pointer to the first node
let head = Some(a);
```

After linking, the list looks like this:

```
head --> [1 | Some] --> [2 | Some] --> [3 | None]
```

Notice we link in **reverse order** in Rust (we wrote `b.next = Some(c)` before `a.next = Some(b)`).<br>
This is because of a concept in Rust known as **ownership**. Once you move `b` into `a.next`, you can no longer access `b` directly to set its `next`.

::: tip
This reverse linking pattern is a good first glimpse into how Rust's ownership system affects the way you write code. The compiler won't let you use a value after you've moved it. To learn more about ownership in Rust, head over to the Rust Official Docs.
:::

## Key Difference

|                    | C                       | Rust                                 |
| ------------------ | ----------------------- | ------------------------------------ |
| Empty list         | `Node *head = NULL`     | `let head: Option<Box<Node>> = None` |
| Create node        | `create_node(data)`     | `Node::new(data)`                    |
| Allocate on heap   | `malloc(sizeof(Node))`  | `Box::new(Node { ... })`             |
| Link nodes         | `a->next = b`           | `a.next = Some(b)`                   |
| Allocation failure | Returns `NULL` silently | Panics immediately                   |
| Free nodes         | `free()` each manually  | Automatic                            |
