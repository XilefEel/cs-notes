# Merging Two Sorted Linked Lists

Another common problem in linked list is merging.

Given two sorted linked lists, we want to merge them into one sorted list.

For example:

```
List 1: [1] -> [3] -> [5] -> NULL
List 2: [2] -> [4] -> [6] -> NULL

Merged: [1] -> [2] -> [3] -> [4] -> [5] -> [6] -> NULL
```

## The Approach

We use two pointers to traverse both lists at the same time, always choosing the smaller value to add to the merged list. We essentially splice the two lists together into a new merged list.

```
Step 1: Compare heads
list1: [1] -> [3] -> [5] -> NULL
        ^
list2: [2] -> [4] -> [6] -> NULL
        ^
merged: dummy -> NULL

Choose 1 (smaller), attach to merged


Step 2: Move list1 pointer, compare again
list1: [1] -> [3] -> [5] -> NULL
               ^
list2: [2] -> [4] -> [6] -> NULL
        ^
merged: dummy -> [1] -> NULL

Choose 2 (smaller), attach to merged


Step 3: Continue comparing
list1: [1] -> [3] -> [5] -> NULL
               ^
list2: [2] -> [4] -> [6] -> NULL
               ^
merged: dummy -> [1] -> [2] -> NULL

Choose 3 (smaller), attach to merged


Step 4: Result
merged: dummy -> [1] -> [2] -> [3] -> [4] -> [5] -> [6] -> NULL
                  ^
                Return dummy.next
```

## In C

```c
// Merge two sorted linked lists
Node *merge_sorted(Node *list1, Node *list2) {
    // Create a dummy node to simplify the merge
    Node dummy;
    dummy.next = NULL;
    Node *tail = &dummy;

    // While both lists have nodes
    while (list1 != NULL && list2 != NULL) {
        if (list1->data <= list2->data) {
            // Attach list1's node
            tail->next = list1;
            list1 = list1->next;
        } else {
            // Attach list2's node
            tail->next = list2;
            list2 = list2->next;
        }
        // Move tail forward
        tail = tail->next;
    }

    // Attach remaining nodes (one list is exhausted)
    if (list1 != NULL) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    // Return the merged list (skip dummy node)
    return dummy.next;
}

// Usage
Node *list1 = NULL;
insert_at_tail(&list1, 1);
insert_at_tail(&list1, 3);
insert_at_tail(&list1, 5);

Node *list2 = NULL;
insert_at_tail(&list2, 2);
insert_at_tail(&list2, 4);
insert_at_tail(&list2, 6);

Node *merged = merge_sorted(list1, list2);
print_list(merged);  // 1 -> 2 -> 3 -> 4 -> 5 -> 6
```

`Node dummy` creates a dummy node that acts as the starting point for the merged list.

`Node *tail = &dummy` keeps track of the last node in the merged list.

`if (list1->data <= list2->data)` compares the current nodes and chooses the smaller one.

`tail->next = list1` attaches the chosen node to the merged list.

`tail = tail->next` moves the tail pointer forward.

After the loop, one list must be empty. We attach whichever list has remaining nodes with `tail->next = list1` or `tail->next = list2`.

`return dummy.next` returns the actual head of the merged list (skipping the dummy).

::: tip Why use a dummy node?
`Node dummy` creates a temporary node on the stack (not the heap). We use `&dummy` to get its address and use it as a starting point for building the merged list.

Without a dummy node, we'd need special logic to handle the first node of the merged list. The dummy simplifies this by letting us always have a node to attach to, and we just return `dummy.next` at the end.
:::

## In Rust

For this example, we will use a recursive algorithm.

```rust
impl Node {
    // Merge two sorted linked lists
    fn merge_sorted(list1: Option<Box<Node>>, list2: Option<Box<Node>>) -> Option<Box<Node>> {
        match (list1, list2) {
            // If one list is empty, return the other
            (None, None) => None,
            (Some(node), None) | (None, Some(node)) => Some(node),

            // Both lists have nodes
            (Some(mut node1), Some(mut node2)) => {
                if node1.data <= node2.data {
                    // Take node1, recursively merge the rest
                    node1.next = Node::merge_sorted(node1.next, Some(node2));
                    Some(node1)
                } else {
                    // Take node2, recursively merge the rest
                    node2.next = Node::merge_sorted(Some(node1), node2.next);
                    Some(node2)
                }
            }
        }
    }
}

// Usage
let mut list1 = None;
list1 = Node::insert_at_tail(list1, 1);
list1 = Node::insert_at_tail(list1, 3);
list1 = Node::insert_at_tail(list1, 5);

let mut list2 = None;
list2 = Node::insert_at_tail(list2, 2);
list2 = Node::insert_at_tail(list2, 4);
list2 = Node::insert_at_tail(list2, 6);

let merged = Node::merge_sorted(list1, list2);
Node::print_list(&merged);  // 1 -> 2 -> 3 -> 4 -> 5 -> 6
```

`match (list1, list2)` handles all four cases: both empty, one of them empty, or both have nodes.

If `node1` is smaller, we attach it and recursively merge `node1.next` with `list2` using `node1.next = Node::merge_sorted(node1.next, Some(node2))`, and vice versa.

**Call stack visualization:**

```
list1: [1] -> [3] -> NULL
list2: [2] -> [4] -> NULL

merge([1,3], [2,4])
├─ 1 <= 2, so take 1
├─ 1.next = merge([3], [2,4])  ← recursive call
│  ├─ 3 > 2, so take 2
│  ├─ 2.next = merge([3], [4])  ← recursive call
│  │  ├─ 3 <= 4, so take 3
│  │  ├─ 3.next = merge(None, [4])  ← recursive call
│  │  │  └─ returns [4]  ← base case
│  │  └─ returns [3] -> [4]
│  └─ returns [2] -> [3] -> [4]
└─ returns [1] -> [2] -> [3] -> [4]
```

::: info Why recursion in Rust?
The C version uses iteration with a dummy node, but in Rust, we use recursion because it's cleaner with ownership, since we don't have to deal with mutable references to build the list.

You could write an iterative version in Rust, but it would require more `.take()` and tons of mutable reference gymnastics.
:::

## Complexity

| Operation   | Time     | Space                                         |
| ----------- | -------- | --------------------------------------------- |
| Merge lists | O(n + m) | O(1) in C, O(n + m) in Rust (recursion stack) |

We visit each node once, where `n` and `m` are the lengths of the two lists.

## Key Difference

|                | C                                 | Rust                                      |
| -------------- | --------------------------------- | ----------------------------------------- |
| Approach       | Iterative with dummy node         | Recursive                                 |
| Dummy node     | `Node dummy`                      | Not needed (recursion handles it)         |
| Comparison     | `if (list1->data <= list2->data)` | `if node1.data <= node2.data`             |
| Attach node    | `tail->next = list1`              | `node1.next = Node::merge_sorted(...)`    |
| Remaining list | `tail->next = list1` or `list2`   | Handled by base case `(Some(node), None)` |
| Space          | O(1)                              | O(n + m) due to recursion stack           |
