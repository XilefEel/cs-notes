# Inserting a Node (Singly Linked List)

There are three ways to insert a new node into a singly linked list:

- **Insert at head** — add to the front (O(1))
- **Insert at tail** — add to the end (O(n) without tail pointer, O(1) with tail pointer)
- **Insert at index** — add at a specific position (O(n))

## Insert at Head

Adding a node to the front of the linked list is the simplest and fastest operation. You don't need to traverse the list at all, since the head is always accessible.

### In C

```c
// Insert a new node at the front of the list
void insert_at_head(Node **head, int data) {
    // Create a new node
    Node *new_node = create_node(data);

    // Point the new node to the old head
    new_node->next = *head;

    // Update head to point to the new node
    *head = new_node;
}

// Usage
Node *head = NULL;
insert_at_head(&head, 10);  // HEAD -> [10] -> NULL
insert_at_head(&head, 20);  // HEAD -> [20] -> [10] -> NULL
insert_at_head(&head, 30);  // HEAD -> [30] -> [20] -> [10] -> NULL
```

`new_node->next = *head` makes the new node point to the old head.<br>

`*head = new_node` updates the head pointer to point to the new node.

::: warning
Notice that we pass `Node **head` in the function signature (pointer to a pointer) and `&head` when calling it. This is because we need to modify the original `head` variable. The `&` gives us the address of `head`, and `Node **` receives that address. If you used `Node *head` instead, the function would only modify a local copy, so the caller's head wouldn't change. This is a common C mistake.
:::

### In Rust

Unlike in C where we modify `head` in place, in Rust, we must **consume** the old head and **return** a new node that points to it. This might feel weird at first, but it's how Rust's ownership system works, you can't have two things owning or pointing the same node.

```rust
impl Node {
    // Takes ownership of the old head, returns Some wrapping a new node
    fn insert_at_head(head: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        Some(Box::new(Node {
            data,
            next: head,  // Old head gets moved into the new node's next
        }))
    }
}

// Usage
let mut head = None;
head = Node::insert_at_head(head, 10);  // HEAD -> [10] -> NONE
head = Node::insert_at_head(head, 20);  // HEAD -> [20] -> [10] -> NONE
head = Node::insert_at_head(head, 30);  // HEAD -> [30] -> [20] -> [10] -> NONE
```

`mut head` explicitly makes the head mutable as in Rust, variables are immutable by default. <br>

In the function signature, `head: Option<Box<Node>>` means we're taking ownership (not borrowing with `&`).<br>

When we pass `head` to the function, we **move** it. In Rust, we transfer ownership of `head` to the function, so the `head` variable can't be used anymore.<br>

Since we moved `head`, we **return** a new `Option<Box<Node>>` which we assign back to `head` so that it can be used again.

::: tip
In C you modify `head` in place with a double pointer (`Node **head`).<br>
In Rust you consume the old head and return a new one. It's the same result, just different ownership model.
:::

::: details Alternative: Mutate in place
You can also write this as a void function that modifies `head` directly using a mutable reference:

```rust
impl Node {
    fn insert_at_head(head: &mut Option<Box<Node>>, data: i32) {
        *head = Some(Box::new(Node {
            data,
            next: head.take(),
        }))
    }
}

// Usage
let mut head = None;
Node::insert_at_head(&mut head, 10);
Node::insert_at_head(&mut head, 20);
```

This is closer to how C does it with `Node **head`. Both approaches work, but the consume-and-return pattern is more common in Rust.
:::

### Key Difference

|                    | C                                            | Rust                                                                         |
| ------------------ | -------------------------------------------- | ---------------------------------------------------------------------------- |
| Function signature | `void insert_at_head(Node **head, int data)` | `fn insert_at_head(head: Option<Box<Node>>, data: i32) -> Option<Box<Node>>` |
| Modify head        | `*head = new_node`                           | Return new head, caller assigns it                                           |
| Complexity         | O(1)                                         | O(1)                                                                         |

## Insert at Tail

Adding a node to the end requires traversing the entire list to find the last node. This makes it O(n).

If you track a **tail pointer**, you can make this O(1), but that adds a little more complexity to every other operation.

### In C

```c
// Insert a new node at the end of the list
void insert_at_tail(Node **head, int data) {
    Node *new_node = create_node(data);

    // If list is empty, the new node becomes the head
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    // Traverse to the last node
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Point the last node to the new node (new node will already point to NULL)
    current->next = new_node;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);  // HEAD -> [10] -> NULL
insert_at_tail(&head, 20);  // HEAD -> [10] -> [20] -> NULL
insert_at_tail(&head, 30);  // HEAD -> [10] -> [20] -> [30] -> NULL
```

Here, we use `while (current->next != NULL)` to walk to the last node (the one whose `next` is `NULL`).<br>

Once we hit the last node, `current->next = new_node` attaches the new node to the end. <br>

::: warning
Inserting at the tail n times in a row is O(n²) total, since each insert walks the entire list. If you're building a list from scratch, inserting at the head and reversing later is often faster.
:::

### In Rust

Unlike in C where we can modify current however we want, in Rust `current` is **immutable** by default. So we have to make sure to use `mut` to make `current` mutable.

```rust
impl Node {
    // Insert a new node at the end of the list
    fn insert_at_tail(head: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match head {
            None => Some(Node::new(data)),  // Empty list, the new node becomes head
            Some(mut node) => {
                let mut current = &mut node;

                // Traverse to the last node
                while current.next.is_some() {
                    current = current.next.as_mut().unwrap();
                }

                // Point the last node to the new node
                current.next = Some(Node::new(data));

                // Return the original head
                Some(node)
            }
        }
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 10);  // HEAD -> [10] -> NONE
head = Node::insert_at_tail(head, 20);  // HEAD -> [10] -> [20] -> NONE
head = Node::insert_at_tail(head, 30);  // HEAD -> [10] -> [20] -> [30] -> NONE
```

Since head is a type `Option<Box<Node>>`, Rust forces us to handle both cases explicitly. <br>
`match head` is used to handle if the list is empty (if `head` is `None`) and if the list is not empty (if `head` is `Some`).<br>

`while current.next.is_some()` is Rust's way of writing `while (current->next != NULL)` in C. It loops so long as the node next to current has a value.<br>

`current.next.as_mut().unwrap()` gives us a mutable reference to the next node so we can keep traversing.

::: info What is .unwrap()?
`.unwrap()` extracts the value from an `Option` or `Result`. If the value is `Some(x)`, it returns `x`. If it's `None`, the program panics!. We use it here because we've already checked that the value exists. If it doesn't, we want the program to crash loudly rather than continue with invalid data.

In production code, you'd typically handle `None` properly with `match` or `if let` instead of using `.unwrap()`.
:::

::: info What is panic!?
`panic!` is Rust's way of saying "something went so wrong that we can't continue safely." When a panic happens, the program immediately stops and prints an error message. This is better than continuing with invalid data, which could cause crashes or security bugs later.

In production code, you'd typically return a `Result` instead so the caller can decide how to handle the error gracefully.
:::

::: tip
Notice the difference: in C we modify through pointers. In Rust we borrow mutably (`&mut`) and traverse using `.as_mut()`. Same logic, different syntax.
:::

### Key Difference

|                    | C                               | Rust                                       |
| ------------------ | ------------------------------- | ------------------------------------------ |
| Traverse condition | `while (current->next != NULL)` | `while current.next.is_some()`             |
| Move to next       | `current = current->next`       | `current = current.next.as_mut().unwrap()` |
| Empty list check   | `if (*head == NULL)`            | Handle with `Option`                       |
| Complexity         | O(n)                            | O(n)                                       |

## Insert at Index

Inserting at a specific index is simmilar to inserting at the tail. But this time, we need to traverse to the node just **before** the target position, then rewiring the pointers so that the node before points to the new node and the new node points to the node after it.

If the index is 0, this is the same as inserting at the **head**.

### In C

```c
// Insert a new node at a specific index
void insert_at_index(Node **head, int data, int index) {
    // If index is 0, insert at head
    if (index == 0) {
        insert_at_head(head, data);
        return;
    }

    // Traverse to the node just before the target index
    Node *current = *head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    // If current is NULL, index is out of bounds
    if (current == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    // Create a new node
    Node *new_node = create_node(data);

    // Point new node to what current was pointing to
    new_node->next = current->next;

    // Point current to the new node
    current->next = new_node;
}

// Usage
Node *head = NULL;
insert_at_head(&head, 10);      // HEAD -> [10] -> NULL
insert_at_head(&head, 20);      // HEAD -> [20] -> [10] -> NULL
insert_at_index(&head, 30, 1);  // HEAD -> [20] -> [30] -> [10] -> NULL
```

We stop at `index - 1` because we need access to the node **before** the insertion point to rewire its `next` pointer.<br>
`new_node->next = current->next` makes the new node point to the node that was originally at the target index.<br>
`current->next = new_node` inserts the new node into the list.

::: warning
If the index is out of bounds, this function just prints an error and returns. In production code you'd want to handle it properly.
:::

### In Rust

```rust
impl Node {
    // Insert a new node at a specific index
    fn insert_at_index(head: Option<Box<Node>>, data: i32, index: usize) -> Option<Box<Node>> {
        match head {
            // Empty list
            None => {
                // If index is 0, insert at head
                if index == 0 {
                    Some(Node::new(data))
                } else {
                    panic!("Index out of bounds");
                }
            }
            Some(mut node) => {
                // If index is 0, create new node pointing to current head
                if index == 0 {
                    // Can't use Node::new() here because we need next to point to the existing node
                    return Some(Box::new(Node {
                        data,
                        next: Some(node),
                    }));
                }

                // Traverse to the node just before the target index
                let mut current = &mut node;
                for _ in 0..index - 1 {
                    // If current.next is None, index is out of bounds
                    if current.next.is_none() {
                        panic!("Index out of bounds");
                    }
                    // Move to the next node
                    current = current.next.as_mut().unwrap();
                }

                // Create a new node and make it point to whatever current was pointing to
                let new_node = Box::new(Node {
                    data,
                    next: current.next.take(),  // take() steals current's next, leaving None
                });

                // Point current to the new node
                current.next = Some(new_node);

                // Return the original head
                Some(node)
            }
        }
    }
}

// Usage
let mut head = None;
head = Node::insert_at_index(head, 10, 0);  // HEAD -> [10] -> NONE
head = Node::insert_at_index(head, 20, 0);  // HEAD -> [20] -> [10] -> NONE
head = Node::insert_at_index(head, 30, 1);  // HEAD -> [20] -> [30] -> [10] -> NONE
```

Just like before, we use `match head` to handles both cases explicitly. If the list is empty (`None`), the new node becomes the head. If the list has nodes (`Some`), we traverse to the end.<br>

`.take()` moves the value out of `current.next` and leaves `None` behind. We need to do this because Rust doesn't allow two things to own or point the same node. By taking ownership from `current.next`, we can transfer it to `new_node.next` without violating ownership rules.<br>

::: tip
In C you check `current != NULL` to detect out of bounds. In Rust you check `current.next.is_none()` — same concept, type-safe.
:::

::: warning
If the index is out of bounds, this function panics!. In production code you'd want to return a `Result` instead so the program doesn't stop.
:::

::: info Why not use Node::new() everywhere?
`Node::new(data)` always creates a node with `next: None`. When inserting in the middle of a list, we need to manually construct the node with `Box::new(Node { data, next: Some(...) })` so we can link it to existing nodes. In C, this wouldn't be a problem since we can change the pointer manually.
:::

### Key Difference

|                | C                                  | Rust                                   |
| -------------- | ---------------------------------- | -------------------------------------- |
| Stop condition | `i < index - 1 && current != NULL` | `for _ in 0..index - 1` + manual check |
| Rewire next    | `new_node->next = current->next`   | `next: current.next.take()`            |
| Out of bounds  | Print error or return code         | `panic!` or return `Result`            |
| Complexity     | O(n)                               | O(n)                                   |

## Summary

| Operation       | C Complexity | Rust Complexity | When to Use                                |
| --------------- | ------------ | --------------- | ------------------------------------------ |
| Insert at head  | O(1)         | O(1)            | Building a list, stack operations          |
| Insert at tail  | O(n)         | O(n)            | Queue behavior (better with tail pointer)  |
| Insert at index | O(n)         | O(n)            | Rare, usually better data structures exist |
