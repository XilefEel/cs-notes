# Detecting a Cycle

A linked list can have a **cycle**.

A cycle in a linked list occurs when a node's `next` pointer points back to a **previous** node, creating a **loop**. This means you can keep following `next` pointers forever without reaching `NULL`.

Our job is to detect wheter a given linked list contains a cycle without using extra space (like a hash table to track visited nodes).

For example:

```
No cycle:
HEAD -> [1] -> [2] -> [3] -> NULL

With cycle:
HEAD -> [1] -> [2] -> [3] -> [4]
                ^             v
               [7] <- [6] <- [5]
```

## The Approach: Floyd's Cycle Detection

The solution is to use two pointers that move at different speeds:

- **Slow pointer** `s` that moves 1 step at a time
- **Fast pointer** `f` that moves 2 steps at a time

If there's a cycle, the `f` will eventually **catch up** to `s` from behind (like a faster runner lapping a slower one on a track). If there's no cycle, the fast pointer will reach `NULL`.

```
Step 1: Both start at head
        s,f
HEAD -> [1] -> [2] -> [3] -> [4]
                ^             v
               [7] <- [6] <- [5]

Step 2: Slow moves 1 step, fast moves 2 steps
                s      f
HEAD -> [1] -> [2] -> [3] -> [4]
                ^             v
               [7] <- [6] <- [5]

Step 3: Continue moving the pointers
                       s
HEAD -> [1] -> [2] -> [3] -> [4]
                ^             v
               [7] <- [6] <- [5] f

Step 4: Eventually they meet.
               s,f
HEAD -> [1] -> [2] -> [3] -> [4]
                ^             v
               [7] <- [6] <- [5]
```

## In C

```c
// Returns true if the list has a cycle, false otherwise
int has_cycle(Node *head) {
    // Empty list or single node can't have a cycle
    if (head == NULL || head->next == NULL) {
        return 0;
    }

    Node *slow = head;
    Node *fast = head;

    // Move pointers at different speeds
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;        // Move 1 step
        fast = fast->next->next;  // Move 2 steps

        // If they meet, there's a cycle
        if (slow == fast) {
            return 1;
        }
    }

    // Fast reached the end, no cycle
    return 0;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 1);
insert_at_tail(&head, 2);
insert_at_tail(&head, 3);
insert_at_tail(&head, 4);

// Create a cycle, make node 4 point back to node 2
Node *second = head->next;
Node *last = head;
while (last->next != NULL) {
    last = last->next;
}
last->next = second;  // 4 -> 2 (cycle!)

if (has_cycle(head)) {
    printf("Cycle detected!\n");  // Prints this
} else {
    printf("No cycle\n");
}
```

`while (fast != NULL && fast->next != NULL)` checks both fast and fast->next to avoid dereferencing NULL when moving 2 steps.

`slow = slow->next` moves slow 1 step forward.

`fast = fast->next->next` moves fast 2 steps forward.

`if (slow == fast)` checks if they've met. If so, then there's a cycle.

::: warning
We must check `fast->next != NULL` because `fast->next->next` can dereference `NULL`. If we only checked `fast != NULL`, we'd crash when trying to access `next->next` on the last node.
:::

## In Rust

```rust
impl Node {
    // Returns true if the list has a cycle, false otherwise
    fn has_cycle(head: &Option<Box<Node>>) -> bool {
        if head.is_none() {
            return false;
        }

        let mut slow = head.as_ref();
        let mut fast = head.as_ref();

        // Move pointers at different speeds
        while fast.is_some() && fast.unwrap().next.is_some() {
            slow = slow.unwrap().next.as_ref();           // Move 1 step
            fast = fast.unwrap().next.as_ref()            // Move 2 steps
                       .unwrap().next.as_ref();

            // If they meet, there's a cycle
            if let (Some(s), Some(f)) = (slow, fast) {
                if std::ptr::eq(s.as_ref(), f.as_ref()) {
                    return true;
                }
            }
        }

        // Fast reached the end, no cycle
        false
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 1);
head = Node::insert_at_tail(head, 2);
head = Node::insert_at_tail(head, 3);
head = Node::insert_at_tail(head, 4);

if Node::has_cycle(&head) {
    println!("Cycle detected!");
} else {
    println!("No cycle");  // Prints this
}

// Note: Creating cycles in safe Rust requires Rc<RefCell<T>>
// which is beyond the scope of these notes
```

`let mut slow = head.as_ref()` gets a reference to the node without taking ownership.

`while fast.is_some() && fast.unwrap().next.is_some()` checks both conditions, just like in C.

`slow.unwrap().next.as_ref()` moves slow 1 step forward.

`fast.unwrap().next.as_ref().unwrap().next.as_ref()` moves fast 2 steps forward.

`std::ptr::eq(s.as_ref(), f.as_ref())` compares the memory addresses to see if they point to the same node.

::: info What is std::ptr::eq?
`std::ptr::eq` is used to check if two references point to the **exact same memory location**. In C, comparing `slow == fast` compares pointer addresses directly. In Rust, `Box` uses `==` to compare the **contents** of nodes, not their addresses, so we have to use `std::ptr::eq`.
:::

## Complexity

| Operation    | Time | Space |
| ------------ | ---- | ----- |
| Detect cycle | O(n) | O(1)  |

## Key Difference

|                  | C                                    | Rust                                                         |
| ---------------- | ------------------------------------ | ------------------------------------------------------------ |
| Pointer type     | `Node *slow, *fast`                  | `slow`, `fast` as `Option<&Box<Node>>`                       |
| Move slow        | `slow = slow->next`                  | `slow = slow.unwrap().next.as_ref()`                         |
| Move fast        | `fast = fast->next->next`            | `fast = fast.unwrap().next.as_ref() .unwrap().next.as_ref()` |
| Compare pointers | `if (slow == fast)`                  | `std::ptr::eq(s.as_ref(), f.as_ref())`                       |
| Check NULL       | `fast != NULL && fast->next != NULL` | `fast.is_some() && fast.unwrap().next.is_some()`             |
