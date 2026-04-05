# Finding the Height of a Binary Tree

The **height** of a binary tree is the number of edges from the root to the deepest leaf. An empty tree has a height of `-1` and a single node has a height of `0`.

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

Height = 2
```

## The Approach

We use recursion to find the height of a tree. The height of a tree can be calculated as `1 + max(height(left), height(right))`. The base case is when we hit `NULL`, which returns `-1`.

```
height([5])
├─ height([3])
│  ├─ height([1])
│  │  ├─ height(NULL) = -1
│  │  └─ height(NULL) = -1
│  │  return 1 + max(-1, -1) = 0
│  ├─ height([4])
│  │  ├─ height(NULL) = -1
│  │  └─ height(NULL) = -1
│  │  return 1 + max(-1, -1) = 0
│  return 1 + max(0, 0) = 1
├─ height([7])
│  ├─ height(NULL) = -1
│  └─ height([10])
│     ├─ height(NULL) = -1
│     └─ height(NULL) = -1
│     return 1 + max(-1, -1) = 0
│  return 1 + max(-1, 0) = 1
return 1 + max(1, 1) = 2

Height = 2
```

## In C

```c
int max(int a, int b) {
    return a > b ? a : b;
}

int height(Node *node) {
    // Base case: empty node
    if (node == NULL) {
        return -1;
    }
    // Recursively find the height of left and right subtrees
    // Return 1 + the height of the taller subtree
    return 1 + max(height(node->left), height(node->right));
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

printf("%d\n", height(bst.root));   // 2
```

`int max(int a, int b)` is a helper function to find the maximum of two integers.

`if (node == NULL) return -1` is the base case, since an empty node has a height of `-1`.

`height(node->left)` recursively finds the height of the left subtree.

`height(node->right)` recursively finds the height of the right subtree.

`1 + max(height(node->left), height(node->right))` returns `1` plus the height of the taller subtree.

::: info Why -1 for empty nodes?
If a tree with only 2 nodes (root and one child) has a height of `1` (only one edge), and a single node has a height of `0` (no edges), then an empty node must have a height of `-1` to maintain this relationship. Since we return `-1` for `NULL` and add `1` when returning, a single leaf node returns `1 + max(-1, -1) = 0`. This is consistent with the definition that height counts edges, not nodes.
:::

## In Rust

```rust
impl BST {
    fn height(&self) -> i32 {
        Self::height_node(self.root.as_ref())
    }

    fn height_node(node: Option<&Box<Node>>) -> i32 {
        match node {
            // Base case: empty node
            None => -1,
            Some(current) => {
                // Recursively find the height of left and right subtrees
                let left_height = Self::height_node(current.left.as_ref());
                let right_height = Self::height_node(current.right.as_ref());

                // Return 1 + the height of the taller subtree
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
bst.insert(10);

println!("{}", bst.height());   // 2
```

`None => -1` is the base case, returning `-1` for an empty node.

`Self::height_node(current.left.as_ref())` recursively finds the height of the left subtree. Same for the right subtree.

`1 + left_height.max(right_height)` returns `1` plus the height of the taller subtree. `.max()` is Rust's built-in method for finding the maximum of two values.

## Complexity

| Operation   | Time | Space |
| ----------- | ---- | ----- |
| Find height | O(n) | O(h)  |

## Key Difference

|             | C                                | Rust         |
| ----------- | -------------------------------- | ------------ |
| Base case   | `return -1`                      | `None => -1` |
| Max height  | Ternary operator `a > b ? a : b` | `.max()`     |
| Return type | `int`                            | `i32`        |
