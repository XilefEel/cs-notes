# Checking if a Tree is a Valid BST

A valid BST must satisfy the BST property for the **entire subtree**, not just immediate children.

```
Invalid BST:
      [5]
     /   \
   [3]   [7]
  /   \
[1]   [9]    <- 9 > 5, violates BST!

Even though each node looks valid locally, the node [9] breaks the global BST property since it's larger than [5] but it's in the left subtree of [5].
```

## The Approach

We recursively pass down a **min** and **max** range that each node must fall within. At each node:

- If we go **left**, the current node's value becomes the new **max**
- If we go **right**, the current node's value becomes the new **min**

```
is_valid([5], min=-∞, max=+∞)
├─ 5 is in (-∞, +∞) ✓
├─ is_valid([3], min=-∞, max=5)
│  ├─ 3 is in (-∞, 5) ✓
│  ├─ is_valid([1], min=-∞, max=3)
│  │  ├─ 1 is in (-∞, 3) ✓
│  │  └─ leaf node, return true
│  └─ is_valid([9], min=3, max=5)
│     ├─ 9 is NOT in (3, 5) ✗
│     └─ return false
└─ return false
```

## In C

```c
int is_valid_bst(Node *node, long min, long max) {
    // Base case: empty node is valid
    if (node == NULL) {
        return 1;
    }

    // If current node's value is out of range, return false
    if (node->data <= min || node->data >= max) {
        return 0;
    }

    // Recurse into left and right subtrees with updated ranges
    return is_valid_bst(node->left, min, node->data) &&
           is_valid_bst(node->right, node->data, max);
}

int is_valid(BST *bst) {
    return is_valid_bst(bst->root, LONG_MIN, LONG_MAX);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);

printf("%d\n", is_valid(&bst));     // 1 (true)

// Manually create an invalid BST
Node *root = create_node(5);
root->left = create_node(3);
root->left->right = create_node(9);  // 9 > 5, invalid!

BST invalid;
invalid.root = root;

printf("%d\n", is_valid(&invalid));  // 0 (false)
```

`if (node->data <= min || node->data >= max)` returns `0` early if the current node is out of the valid range.

`is_valid_bst(node->left, min, node->data)` recurses left with the current node's value as the new max.

Similarly, `is_valid_bst(node->right, node->data, max)` recurses right with the current node's value as the new min.

`LONG_MIN` and `LONG_MAX` represent `-∞` and `+∞`. We use `long` instead of `int` since `long` has a wider range, avoiding any edge cases where the node's value equals `INT_MIN` or `INT_MAX`.

::: warning
Notice that we use strict inequalities (`<=` and `>=`) since a valid BST does not allow duplicate values. If your BST allows duplicates, you'd need to adjust the comparison accordingly.
:::

## In Rust

```rust
impl BST {
    fn is_valid(&self) -> bool {
        Self::is_valid_bst(self.root.as_ref(), i64::MIN, i64::MAX)
    }

    fn is_valid_bst(node: Option<&Box<Node>>, min: i64, max: i64) -> bool {
        match node {
            // Base case: empty node is valid
            None => true,
            Some(current) => {
                let val = current.data as i64;

                // If current node's value is out of range, return false
                if val <= min || val >= max {
                    return false;
                }

                // Recurse into left and right subtrees with updated ranges
                Self::is_valid_bst(current.left.as_ref(), min, val) &&
                Self::is_valid_bst(current.right.as_ref(), val, max)
            }
        }
    }
}

// Usage
let mut bst = BST::new();
bst.insert(5);
bst.insert(3);
bst.insert(7);
bst.insert(1);
bst.insert(4);

println!("{}", bst.is_valid());     // true

// Manually create an invalid BST
let mut invalid = BST::new();
invalid.root = Some(Box::new(Node {
    data: 5,
    left: Some(Box::new(Node {
        data: 3,
        left: None,
        right: Some(Box::new(Node {
            data: 9,    // 9 > 5, invalid!
            left: None,
            right: None,
        })),
    })),
    right: None,
}));

println!("{}", invalid.is_valid()); // false
```

`i64` is Rust's version of `long` in C. `as i64` casts the current node's `i32` value into `i64` so we can compare with `i64::MIN` and `i64::MAX`.

`if val <= min || val >= max` returns `false` if the current node violates the range.

`Self::is_valid_bst(current.left.as_ref(), min, val)` recurses left with the current value as the new max, just like in C.

`Self::is_valid_bst(current.right.as_ref(), val, max)` recurses right with the current value as the new min.

## Complexity

| Operation    | Time | Space |
| ------------ | ---- | ----- |
| Is valid BST | O(n) | O(h)  |

## Key Difference

|                | C                          | Rust                   |
| -------------- | -------------------------- | ---------------------- |
| Initial bounds | `LONG_MIN`, `LONG_MAX`     | `i64::MIN`, `i64::MAX` |
| Range check    | `node->data <= min`        | `val <= min`           |
| Cast value     | Implicit `long` comparison | `current.data as i64`  |
| Base case      | `return 1`                 | `true`                 |
