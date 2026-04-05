# Check if Palindrome

A linked list is a **palindrome** if it reads the same forwards and backwards.

For example:

```
Palindrome:
HEAD -> [1] -> [2] -> [3] -> [2] -> [1] -> NULL

Not a palindrome:
HEAD -> [1] -> [2] -> [3] -> [4] -> [5] -> NULL
```

## The Approach

Since we can't traverse a singly linked list backwards, the idea is to:

1. **Find the middle** of the list using slow/fast pointers
2. **Reverse the second half** of the list
3. **Compare** both halves node by node

If all values match, then it's a **palindrome**.

```
Step 1: Find the middle using fast and slow pointers
                       s             f
HEAD -> [1] -> [2] -> [3] -> [2] -> [1] -> NULL
                       ^
                     middle

Step 2: Reverse the second half
HEAD -> [1] -> [2] -> [3]    [1] -> [2] -> NULL
                              ^
                        second half reversed

Step 3: Compare both halves
HEAD -> [1] -> [2] -> [3]    [1] -> [2] -> NULL
         ^                    ^
        left                right

Result: 1 == 1, 2 == 2, it's a Palindrome!
```

## In C

We already know how to reverse a linked list! So all that's left is to find the middle and split it in half.

```c
// Returns true if the list is a palindrome
int is_palindrome(Node *head) {
    if (head == NULL || head->next == NULL) {
        return 1;
    }

    // Find the middle
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Reverse the second half
    Node *second = reverse_list(slow);
    Node *copy = second;  // Keep a copy to restore later

    // Compare both halves
    Node *left = head;
    Node *right = second;

    int result = 1;

    // Check until the right half is exhausted
    while (right != NULL) {
        if (left->data != right->data) {
            result = 0;
            break;
        }
        left = left->next;
        right = right->next;
    }

    // Restore the list
    reverse_list(copy);

    return result;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 1);
insert_at_tail(&head, 2);
insert_at_tail(&head, 3);
insert_at_tail(&head, 2);
insert_at_tail(&head, 1);

if (is_palindrome(head)) {
    printf("Palindrome!\n");    // Prints this
} else {
    printf("Not a palindrome\n");
}
```

`Node *slow, *fast` finds the middle using a slow and fast pointer (the same trick from cycle detection).

`reverse_list(slow)` reverses the second half of the list in place.

`while (right != NULL)` compares both halves node by node, stopping when the right half is exhausted.

`reverse_list(copy)` restores the list to its original order after the check.

::: tip Why restore the list?
Reversing the second half mutates the list. If the caller still needs the list intact after calling `is_palindrome`, leaving it modified would be surprising. Restoring it is not strictly necessary, but it's good practice to avoid side effects when possible.
:::

::: warning
We only iterate until `right != NULL` rather than checking both `left` and `right`, because the second half is always the **same** length or **shorter** than the first half, so `right` always runs out first.
:::

## In Rust

Checking for a palindrome in Rust is harder than in C, since we can't have **two mutable reference**s to the same list at the same time, which makes it tricky to reverse the second half while still having access to the first half for comparison. So we must do a slightly different approach.

```rust
impl Node {
    fn is_palindrome(head: Option<Box<Node>>) -> bool {
        if head.is_none() {
            return true;
        }

        // Count the length
        let mut len = 0;
        let mut curr = head.as_ref();
        while let Some(node) = curr {
            len += 1;
            curr = node.next.as_ref();
        }

        // Walk to the middle and split
        let mut left = head;
        let mut walker = &mut left;
        for _ in 0..(len + 1) / 2 {
            walker = &mut walker.as_mut().unwrap().next;
        }
        let second = walker.take();

        // Reverse the second half
        let right = Node::reverse(second);

        // Compare both halves
        let mut l = left.as_ref();
        let mut r = right.as_ref();

        // Check until the right half is exhausted
        while r.is_some() {
            if l.unwrap().data != r.unwrap().data {
                return false;
            }
            l = l.unwrap().next.as_ref();
            r = r.unwrap().next.as_ref();
        }

        true
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 1);
head = Node::insert_at_tail(head, 2);
head = Node::insert_at_tail(head, 3);
head = Node::insert_at_tail(head, 2);
head = Node::insert_at_tail(head, 1);

if Node::is_palindrome(head) {
    println!("Palindrome!");    // Prints this
} else {
    println!("Not a palindrome");
}
```

`Node::reverse(head)` takes ownership of the list and returns the reversed version.

`walker.take()` splits the list at the middle, `left` owns the first half, `second` owns the second half.

`while r.is_some()` compares both halves, stopping when the right half is exhausted.

`for _ in 0..(len + 1) / 2` walks to the split point. The `len + 1` ensures that for odd-length lists, the first half gets the **middle** node, so the second half is always the **shorter** one.

::: info Why two passes?
We count the length first to know where to split. If we tried to do this in **one** pass, we'd need `unsafe`. Two passes keeps the code clean and safe, and it's still **O(n)**.
:::

::: info Why no restore?
`is_palindrome` takes ownership of the list, so the caller gives it up entirely. If you need the list after the call, you need to **clone** it before passing it in.
:::

## Complexity

| Operation        | Time | Space |
| ---------------- | ---- | ----- |
| Check palindrome | O(n) | O(1)  |

## Key Difference

|                    | C                   | Rust                   |
| ------------------ | ------------------- | ---------------------- |
| Mutates input list | Yes, restores after | No, consumes ownership |
| Passes             | 1                   | 2 (count + compare)    |
