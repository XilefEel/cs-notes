# Checking if a Tree is Balanced

A binary tree is **balanced** if for every node, the height difference between its left and right subtrees is at most **1**.

```
Balanced:                   Not balanced:
        [5]                       [5]
       /   \                     /
     [3]   [7]                 [3]
    /   \                     /
  [1]   [4]                 [1]

Height diff at [5] = 0      Height diff at [5] = 2 
```

## The Approach

The naive approach is to call `height()` on every node and check the difference. But this is O(n²) since we recompute heights repeatedly.

The better approach is to check balance and compute height **at the same time** in a single pass. We return `-1` as a sentinel value to signal that a subtree is already unbalanced, so we can stop early.

```
is_balanced([5])
├─ is_balanced([3])
│  ├─ is_balanced([1])
│  │  ├─ is_balanced(NULL) = 0
│  │  └─ is_balanced(NULL) = 0
│  │  diff = |0 - 0| = 0, return height = 1
│  ├─ is_balanced([4])
│  │  ├─ is_balanced(NULL) = 0
│  │  └─ is_balanced(NULL) = 0
│  │  diff = |0 - 0| = 0, return height = 1
│  diff = |1 - 1| = 0, return height = 2
├─ is_balanced([7])
│  ├─ is_balanced(NULL) = 0
│  └─ is_balanced(NULL) = 0
│  diff = |0 - 0| = 0, return height = 1
diff = |2 - 1| = 1, return height = 3

3 != -1,
Tree is balanced!
```

## In C

```c
int check_balanced(Node *node) {
    // Base case: empty node
    if (node == NULL) {
        return 0;
    }

    // Recursively check left and right subtrees
    int left_height = check_balanced(node->left);
    int right_height = check_balanced(node->right);

    // If either subtree is unbalanced, propagate -1 up
    if (left_height == -1 || right_height == -1) {
        return -1;
    }

    // If the height difference is more than 1, it's unbalanced
    if (abs(left_height - right_height) > 1) {
        return -1;
    }

    // Return the height of this subtree
    return 1 + max(left_height, right_height);
}

int is_balanced(BST *bst) {
    return check_balanced(bst->root) != -1;
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);

printf("%d\n", is_balanced(&bst));  // 1 (true)

BST unbalanced = create_bst();
insert(&unbalanced, 5);
insert(&unbalanced, 3);
insert(&unbalanced, 1);

printf("%d\n", is_balanced(&unbalanced));   // 0 (false)
```

`if (left_height == -1 || right_height == -1)` checks if either subtree is already unbalanced and propagates `-1` up the call stack immediately.

`abs(left_height - right_height) > 1` checks if the height difference exceeds 1 at the current node.

`return 1 + max(left_height, right_height)` returns the height of the current subtree if it's balanced.

`check_balanced(bst->root) != -1` returns `1` if the tree is balanced, `0` otherwise.

::: warning
We reuse `-1` as a sentinel value to mean "unbalanced", which is why the base case returns `0` instead of `-1`. If we returned `-1` for empty nodes, we'd confuse "empty" with "unbalanced". This shift of 1 doesn't affect the balance check since we only care about the difference between subtrees, not the actual height values.
:::

## In Rust

```rust
impl BST {
    fn is_balanced(&self) -> bool {
        Self::check_balanced(self.root.as_ref()) != -1
    }

    fn check_balanced(node: Option<&Box<Node>>) -> i32 {
        match node {
            // Base case: empty node
            None => 0,
            Some(current) => {
                // Recursively check left and right subtrees
                let left_height = Self::check_balanced(current.left.as_ref());
                let right_height = Self::check_balanced(current.right.as_ref());

                // If either subtree is unbalanced, propagate -1 up
                if left_height == -1 || right_height == -1 {
                    return -1;
                }

                // If the height difference is more than 1, it's unbalanced
                if (left_height - right_height).abs() > 1 {
                    return -1;
                }

                // Return the height of this subtree
                1 + left_height.max(right_height)
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

println!("{}", bst.is_balanced());  // true

let mut unbalanced = BST::new();
unbalanced.insert(5);
unbalanced.insert(3);
unbalanced.insert(1);

println!("{}", unbalanced.is_balanced());  // false
```

`Self::check_balanced(self.root.as_ref()) != -1` returns `true` if the tree is balanced, `false` otherwise.

`if left_height == -1 || right_height == -1` propagates `-1` up if either subtree is unbalanced.

`(left_height - right_height).abs() > 1` checks if the height difference exceeds 1 at the current node, just like in C.

`1 + left_height.max(right_height)` returns the height of the current subtree if balanced.

## Complexity

| Operation   | Time | Space |
| ----------- | ---- | ----- |
| Is balanced | O(n) | O(h)  |

## Key Difference

|                   | C                                  | Rust                                    |
| ----------------- | ---------------------------------- | --------------------------------------- |
| Absolute value    | `abs(left_height - right_height)`  | `(left_height - right_height).abs()`    |
| Return balanced   | `check_balanced(...) != -1`        | `Self::check_balanced(...) != -1`       |
| Sentinel value    | `-1`                               | `-1`                                    |
