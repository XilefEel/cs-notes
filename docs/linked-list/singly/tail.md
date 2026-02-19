# Bonus: Tail Pointer

We've seen that operations like `insert_at_tail` require O(n) time because we have to traverse the entire list to find the last node. But what if we kept track of both the **head** and the **tail**?

## The Trade-off

Adding a tail pointer changes the performance and complexity:

| Operation       | Without Tail | With Tail  | Notes                                  |
| --------------- | ------------ | ---------- | -------------------------------------- |
| Insert at head  | O(1)         | O(1)       | Must update tail if list was empty     |
| Insert at tail  | O(n)         | O(1)       | Direct access to last node!            |
| Delete at head  | O(1)         | O(1)       | Must update tail if list becomes empty |
| Delete at tail  | O(n)         | O(n)       | Still need second-to-last node         |
| Memory overhead | 1 pointer    | 2 pointers | Tracking both head and tail            |

## The Structure

### In C

```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Now we track both head and tail
typedef struct LinkedList {
    Node *head;
    Node *tail;
} LinkedList;

// Initialize an empty list
LinkedList *create_list() {
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}
```

`head` and `tail` start as `NULL` for an empty list.

### In Rust

```rust
struct Node {
    data: i32,
    next: Option<Box<Node>>,
}

// Now we track both head and tail
struct LinkedList {
    head: Option<Box<Node>>,
    tail: *mut Node,  // Raw pointer to avoid ownership issues
}

impl LinkedList {
    fn new() -> Self {
        LinkedList {
            head: None,
            tail: std::ptr::null_mut(),
        }
    }
}
```

The `tail` must be a **raw pointer** (`*mut Node`) because `head` already owns the entire chain through Box and so if `tail` was also `Option<Box<Node>>`, it would try to own the last node too.

::: danger Why raw pointers?
Normally, Rust guarantees memory safety at compile time. But with a tail pointer, we'd have two owners which violates Rust's single-ownership rule!

Raw pointers let us bypass this restriction, but we lose Rust's safety guarantees and have to use `unsafe` blocks. The compiler can't verify we're using the tail pointer correctly, just like how C can't guarantee that we use pointers correctly.

In production Rust code, you'd either:

- Use `Rc<RefCell<Node>>` for shared ownership (adds runtime overhead)
- Redesign with a `VecDeque` or ring buffer (what Rust's stdlib does)

For this section, raw pointers are useful because they show where Rust's safety model conflicts with certain data structure patterns.
:::

## Insert at Head

When inserting at the head, we now need to update `tail` if the list was previously empty.

### In C

```c
void insert_at_head(LinkedList *list, int data) {
    Node *new_node = create_node(data);
    new_node->next = list->head;
    list->head = new_node;

    // If list was empty, tail should point to the new node too
    if (list->tail == NULL) {
        list->tail = new_node;
    }
}
```

Before, we just simply inserted a new node at the head, but now we have to check `if (list->tail == NULL)`, which handles the empty list case.

### In Rust

```rust
impl LinkedList {
    fn insert_at_head(&mut self, data: i32) {
        let mut new_node = Box::new(Node { data, next: self.head.take() });
        let raw_node: *mut Node = &mut *new_node;

        self.head = Some(new_node);

        // If list was empty, tail should point to the new node too
        if self.tail.is_null() {
            self.tail = raw_node;
        }
    }
}
```

::: info What is self?
`self` is similar to `this` in other languages, like C++ and Java. `self` refers to the current instance of the struct. But Rust's ownership system makes it more explicit:

**`&self`** — borrows the instance (read-only)

```rust
fn print(&self) { ... }  // Just looking, not modifying
```

**`&mut self`** — borrows the instance mutably (can modify)

```rust
fn insert(&mut self, data: i32) { ... }  // Modifying the list
```

**`self`** — takes ownership (consumes the instance)

```rust
fn consume(self) { ... }  // Caller can't use it after this
```

In our linked list methods, we use `&mut self` so that we want to modify the list, but we still want the caller to own it. If we used just `self`, the list would be consumed and unusable after calling the method!
:::

## Insert at Tail

This is where the tail pointer shines, since we can now insert directly at the end without traversing.

### In C

```c
void insert_at_tail(LinkedList *list, int data) {
    Node *new_node = create_node(data);

    // If list is empty, both head and tail point to new node
    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    // Otherwise, link the current tail to the new node
    list->tail->next = new_node;
    list->tail = new_node;
}
```

No traversal needed! We directly access `list->tail` and link the new node, so the complexity becomes **O(1) instead of O(n).**

### In Rust

```rust
impl LinkedList {
    fn insert_at_tail(&mut self, data: i32) {
        let mut new_node = Box::new(Node { data, next: None });
        let raw_node: *mut Node = &mut *new_node;

        if self.tail.is_null() {
            // List is empty
            self.head = Some(new_node);
            self.tail = raw_node;
        } else {
            // Link current tail to new node
            unsafe {
                (*self.tail).next = Some(new_node);
            }
            self.tail = raw_node;
        }
    }
}
```

We need `unsafe` here because we're dereferencing the raw tail pointer.

::: info What is unsafe?
`unsafe` is Rust's escape hatch that says "I'm taking responsibility for memory safety here, compiler can't help me." Inside an `unsafe` block, you can:

- Dereference raw pointers
- Call unsafe functions
- Access mutable static variables
- Implement unsafe traits

In C, The whole language is a giant `unsafe` block, since the compiler trusts you completely. In Rust, unsafe is explicit and localized to specific blocks, making it easier to audit where things could go wrong.
:::

## Delete at Head

Deleting the head also requires updating the tail if we're removing the last node.

### In C

```c
void delete_at_head(LinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node *temp = list->head;
    list->head = list->head->next;
    free(temp);

    // If list is now empty, reset tail
    if (list->head == NULL) {
        list->tail = NULL;
    }
}
```

`if (list->head == NULL)` resets the tail when deleting the last node.

### In Rust

```rust
impl LinkedList {
    fn delete_at_head(&mut self) {
        if let Some(node) = self.head.take() {
            self.head = node.next;

            // If list is now empty, reset tail
            if self.head.is_none() {
                self.tail = std::ptr::null_mut();
            }
        } else {
            println!("List is empty");
        }
    }
}
```

::: info What is std::ptr::null_mut()?
`std::ptr::null_mut()` creates a null mutable raw pointer. It's like `NULL` in C, but specifically for mutable raw pointers (`*mut T`).

You can check if a raw pointer is null with `.is_null()`:

```rust
if self.tail.is_null() {
    // tail points to nothing
}
```

:::

## Delete at Tail

Even with a tail pointer, deleting the last node still requires traversing to the **second-to-last** node because we need to update its `next` pointer and make it the new tail.

### In C

```c
void delete_at_tail(LinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    // If there's only one node
    if (list->head == list->tail) {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return;
    }

    // Traverse to second-to-last node
    Node *current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }

    // Delete the tail
    free(list->tail);
    current->next = NULL;
    list->tail = current;  // Update tail to second-to-last
}
```

We still have to traverse with `while (current->next != list->tail)`, so this remains **O(n)**.

### In Rust

```rust
impl LinkedList {
    fn delete_at_tail(&mut self) {
        if self.head.is_none() {
            println!("List is empty");
            return;
        }

        // If there's only one node
        if self.head.as_ref().unwrap().next.is_none() {
            self.head = None;
            self.tail = std::ptr::null_mut();
            return;
        }

        // Traverse to second-to-last node
        let mut current = self.head.as_mut().unwrap();
        while current.next.as_ref().unwrap().next.is_some() {
            current = current.next.as_mut().unwrap();
        }

        // Delete the tail and update pointer
        current.next = None;
        self.tail = current as *mut Node;
    }
}
```

## Summary

✅ **Use a tail pointer when:**

- You frequently insert at the end (queue behavior)
- O(1) tail insertion is worth the extra bookkeeping
- You're implementing a queue or circular buffer

❌ **Skip the tail pointer when:**

- You rarely insert at the tail
- The extra complexity isn't worth it
- You mostly insert/delete at the head (stack behavior)

In practice, if you need both fast head and tail operations, you might want to use a **doubly linked list** instead. Doubly linked lists make tail operations easier because you can traverse backwards.
