# Removing the Nth Node From the End

Another common problem in linked list is removing the nth node from the end of a linked list.

The problem is that we **don't know** the length of the list, and we want to do it in **one pass**.

For example:

```
Before:
HEAD -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL
                              ^
                         remove this (2nd from end)

After:
HEAD -> [1] -> [2] -> [3] -> [5] -> NULL
```

## The Approach

The trick is to maintain two pointers with a **fixed gap of n nodes** between them. When the fast pointer reaches the end, the slow pointer will be exactly before the node we want to remove.

- **Fast pointer** `f` starts at dummy and moves `n` steps ahead first
- **Slow pointer** `s` also starts at dummy and moves after `f` is already `n` steps ahead
- Both pointers move **1 step at a time** until `f` reaches the last node

When `f->next` is `NULL`, `f` is at the last node and `s` is right **before** the nth node from the end, which is exactly what we want.

```
Remove 2nd from end. List has 5 nodes.

Step 1: Create dummy node, both pointers start at dummy
        s,f
DUMMY -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL

Step 2: Move fast n=2 steps ahead
         s              f
DUMMY -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL

Step 3: Move both 1 step at a time until fast reaches the end
                        s             f
DUMMY -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL

Step 4: Remove the node after slow
                        s             f
DUMMY -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL
                               ^
                            to be removed

Result:
DUMMY -> [1] -> [2] -> [3] -> [5] -> NULL
          ^
    Return dummy.next
```

## In C

```c
// Removes the nth node from the end of the list
Node *remove_nth_from_end(Node *head, int n) {
    // Create a dummy node that points to head
    Node dummy;
    dummy.next = head;

    Node *fast = &dummy;
    Node *slow = &dummy;

    // Move fast n steps ahead
    for (int i = 0; i < n; i++) {
        fast = fast->next;
    }

    // Move both until fast reaches the last node
    while (fast->next != NULL) {
        slow = slow->next;
        fast = fast->next;
    }

    // slow is right before the node to remove
    Node *temp = slow->next;
    slow->next = slow->next->next;
    free(temp);

    return dummy.next;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 1);
insert_at_tail(&head, 2);
insert_at_tail(&head, 3);
insert_at_tail(&head, 4);
insert_at_tail(&head, 5);

head = remove_nth_from_end(head, 2);
print_list(head);   // HEAD -> 1 -> 2 -> 3 -> 5 -> NULL
```

`Node dummy` creates a dummy node that acts as the starting point for the merged list.

`for (int i = 0; i < n; i++)` moves `fast` exactly `n` steps ahead.

`slow = slow->next` and `fast = fast->next` moves `slow` and `fast` one step at a time.

`slow->next = slow->next->next` skips over the target node, disconnecting it from the list.

`free(temp)` deallocates the deleted node.

::: tip Why use a dummy node?
Just like merging two sorted linked lists, without a dummy node, we'd need special logic to handle deleting the first node. The dummy node simplifies this by letting us always have a node to attach to, and we just return `dummy.next` at the end.
:::

::: warning
We stop at `fast->next != NULL` rather than `fast != NULL` to ensure that `slow` lands **before** the target node, not on it.
:::

## In Rust

```rust
impl Node {
    // Removes the nth node from the end of the list
    fn remove_nth_from_end(head: Option<Box<Node>>, n: usize) -> Option<Box<Node>> {
        // Create a dummy node that points to head
        let mut dummy = Box::new(Node {
            data: 0,
            next: head,
        });

        let mut fast: *mut Node = &mut *dummy;
        let mut slow: *mut Node = &mut *dummy;

        unsafe {
            // Move fast n steps ahead
            for _ in 0..n {
                fast = (*fast).next.as_mut().map(|n| &mut **n).unwrap();
            }

            // Move both until fast reaches the last node
            while (*fast).next.is_some() {
                slow = (*slow).next.as_mut().map(|n| &mut **n).unwrap();
                fast = (*fast).next.as_mut().map(|n| &mut **n).unwrap();
            }

            // slow is right before the node to remove
            let temp = (*slow).next.take();
            if let Some(mut node) = temp {
                (*slow).next = node.next.take();
            }
        }

        dummy.next
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 1);
head = Node::insert_at_tail(head, 2);
head = Node::insert_at_tail(head, 3);
head = Node::insert_at_tail(head, 4);
head = Node::insert_at_tail(head, 5);

head = Node::remove_nth_from_end(head, 2);
Node::print_list(&head);    // HEAD -> 1 -> 2 -> 3 -> 5 -> NULL
```

`let mut fast: *mut Node = &mut *dummy` gets a raw mutable pointer to the dummy node.

`for _ in 0..n` moves `fast` exactly `n` steps ahead, just like in C.

`(*fast).next.as_mut().map(|n| &mut **n).unwrap()` does a few things at once:

- `*fast` dereferences `fast` to get the node it points to.

- `.next` accesses the node next to `fast`, which is an `Option<Box<Node>>`.

- `.as_mut()` makes a mutable reference, which converts `Option<Box<Node>>` to `Option<&mut Box<Node>>`. We want a mutable reference since we want to borrow the next node to move our pointer.

- `.map(|n| &mut **n)` converts `Option<&mut Box<Node>>` to `Option<&mut Node>`.
  <br> The `n` in the closure is `&mut Box<Node>`.
  <br> `*n` dereferences the mutable reference `&mut` to get `Box<Node>`.
  <br> `**n` dereferences the `Box` to get the inner `Node`.
  <br> `&mut` then reborrows it and makes a mutable reference to it as `&mut Node`.

- `.unwrap()` extracts the `&mut Node` from the `Option`, which Rust will then automatically coerces into `*mut Node` to assign back to `fast`.

`(*slow).next.take()` removes the target node from the list by taking ownership of it, which Rust will then drop it automatically at the end of the block.

::: danger Why unsafe?
Rust's borrow checker doesn't allow two mutable references into the **same** data structure at the same time. Since both slow and fast point into the same list, and we need to mutate `slow->next` at the end, the borrow checker would **reject** this, even though we know the two pointers never actually conflict.

The alternative would be to **count** the list length first, then do a **second** pass to find the node, which would let us write fully safe Rust. But that defeats the purpose of the challenge, which was to do it in **one** pass.
:::

::: info What is .map()?
`.map()` is a method on `Option` that transforms the value inside it by applying a **closure**, without **unwrapping** it. If the `Option` is `Some`, it applies the closure and returns `Some(result)`. If it's `None`, it does nothing and returns `None`.

```rust
let x: Option<i32> = Some(5);
let y = x.map(|n| n * 2);   // Some(10)

let z: Option<i32> = None;
let w = z.map(|n| n * 2);   // None (closure never runs)
```

In our case, we use it to convert `Option<&mut Box<Node>>` to `Option<&mut Node>`, punching through the `Box` layer to get directly to the `Node` inside. Without `.map()`, we'd have to **unwrap** first, **dereference** manually, then **wrap** back into an `Option`.
:::

::: warning
`unwrap()` will panic if `n` is larger than the list length. In production code you'd want to handle this gracefully.
:::

## Complexity

| Operation           | Time | Space |
| ------------------- | ---- | ----- |
| Remove nth from end | O(n) | O(1)  |

## Key Differences

|             | C                                       | Rust                                             |
| ----------- | --------------------------------------- | ------------------------------------------------ |
| Dummy node  | `Node dummy`                            | `Box<Node>`                                      |
| Pointers    | ` Node  *fast`                          | `let mut fast: *mut Node`                        |
| Traversal   | `fast = fast->next`                     | `(*fast).next.as_mut().map(n &mut **n).unwrap()` |
| Remove node | `slow->next = slow->next->next; free()` | `(*slow).next.take()` (auto-dropped)             |
