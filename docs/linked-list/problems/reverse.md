# Reversing a Linked List

A common problem in linked list is when we want to **reverse** them in place.

Given a singly linked list, we need to **reverse** it so that the **last** node becomes the **first**, and the **first** becomes the **last**.

For example:

```
Before:
HEAD -> [1] -> [2] -> [3] -> NULL

After:
HEAD -> [3] -> [2] -> [1] -> NULL
```

## The Approach

To reverse a linked list in place, we need to flip the direction of each `next` pointer. To do this, we need **three pointers** to avoid losing track of nodes as we rewire them.

```
Before: HEAD -> [1] -> [2] -> [3] -> NULL

Step 1: Start with three pointers
prev = NULL
curr = [1] -> [2] -> [3] -> NULL
next = (to be set)

Step 2: Save next, flip curr->next to prev
NULL <- [1]    [2] -> [3] -> NULL
  ^      ^      ^
 prev   curr   next

Step 3: Move all pointers forward
NULL <- [1] <- [2]    [3] -> NULL
         ^      ^      ^
        prev   curr   next

Step 4: Repeat until curr is NULL
NULL <- [1] <- [2] <- [3]
                       ^
                      prev
```

## In C

```c
// Reverse a singly linked list
Node *reverse_list(Node *head) {
    Node *prev = NULL;
    Node *curr = head;
    Node *next = NULL;

    // Traverse and reverse each pointer
    while (curr != NULL) {
        // Save the next node
        next = curr->next;

        // Reverse the current node's pointer
        curr->next = prev;

        // Move prev and curr one step forward
        prev = curr;
        curr = next;
    }

    // prev is now the new head
    return prev;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 1);
insert_at_tail(&head, 2);
insert_at_tail(&head, 3);
print_list(head);  // 1 -> 2 -> 3

head = reverse_list(head);
print_list(head);  // 3 -> 2 -> 1
```

`next = curr->next` saves the next node before we lose access to it.

`curr->next = prev` flips the pointer to point backward.

`prev = curr` moves prev forward, and `curr = next` moves curr forward.

When the loop ends, `curr` is NULL and `prev` now points to what was the last node (now the new head).

## In Rust

```rust
impl Node {
    // Reverse a singly linked list
    fn reverse(head: Option<Box<Node>>) -> Option<Box<Node>> {
        let mut prev = None;
        let mut curr = head;

        // Traverse and reverse each pointer
        while let Some(mut node) = curr {
            // Save the next node
            let next = node.next.take();

            // Reverse the current node's pointer
            node.next = prev;

            // Move prev and curr one step forward
            prev = Some(node);
            curr = next;
        }

        // prev is now the new head
        prev
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 1);
head = Node::insert_at_tail(head, 2);
head = Node::insert_at_tail(head, 3);
Node::print_list(&head);  // 1 -> 2 -> 3

head = Node::reverse(head);
Node::print_list(&head);  // 3 -> 2 -> 1
```

`while let Some(mut node) = curr` unwraps each node as we traverse.

`node.next.take()` removes the next node from `node.next` and gives us ownership of it.

`node.next = prev` flips the pointer backward.

`prev = Some(node)` and `curr = next` moves both pointers one step forward.

::: info Understanding ownership in the loop
The `while let Some(mut node) = curr` unwraps the `Option`, giving us `node` with type `Box<Node>`. Since `prev` is declared as `Option<Box<Node>>`, we need to wrap `node` back in `Some()` when assigning to it.

We can directly assign `next` to `curr` because `next` is already `Option<Box<Node>>` (the same type as `curr`). This transfers ownership from `next` to `curr`.
:::

## Complexity

| Operation    | Time | Space |
| ------------ | ---- | ----- |
| Reverse list | O(n) | O(1)  |

## Key Difference

|                | C                          | Rust                                  |
| -------------- | -------------------------- | ------------------------------------- |
| Three pointers | `Node *prev, *curr, *next` | `prev`, `curr` as `Option<Box<Node>>` |
| Save next      | `next = curr->next`        | `next = node.next.take()`             |
| Flip pointer   | `curr->next = prev`        | `node.next = prev`                    |
| Move forward   | `prev = curr; curr = next` | `prev = Some(node); curr = next`      |
