# Lowest Common Ancestor (LCA)

The **Lowest Common Ancestor** of two nodes `p` and `q` is the deepest node in the tree that has both `p` and `q` as descendants. A node can be a descendant of itself.

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

LCA(1, 4)  = 3    (both are in [3]'s children)
LCA(1, 7)  = 5    (they split at the root)
LCA(3, 10) = 5    (they split at the root)
LCA(3, 4)  = 3    (a node can be its own ancestor)
```

## The Approach

The BST property makes this elegant. At each node:

- If both `p` and `q` are **less than** the current node, the LCA is in the **left** subtree
- If both `p` and `q` are **greater than** the current node, the LCA is in the **right** subtree
- Otherwise, the current node **is** the LCA, because `p` and `q` split here

```
LCA(1, 4) in:
        [5]
       /   \
     [3]   [7]
    /   \
  [1]   [4]

1 < 5 and 4 < 5, go left
1 < 3 and 4 > 3, they split! LCA = [3]
```

## In C

```c
// Returns the LCA of nodes with values p and q
Node *lca(Node *node, int p, int q) {
    // Base case: empty node
    if (node == NULL) {
        return NULL;
    }

    // Both values are in the left subtree
    if (p < node->data && q < node->data) {
        return lca(node->left, p, q);
    }

    // Both values are in the right subtree
    if (p > node->data && q > node->data) {
        return lca(node->right, p, q);
    }

    // If they split, the current node is the LCA
    return node;
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

Node *result = lca(bst.root, 1, 4);
printf("%d\n", result->data);   // 3

result = lca(bst.root, 1, 7);
printf("%d\n", result->data);   // 5

result = lca(bst.root, 3, 4);
printf("%d\n", result->data);   // 3
```

`if (p < node->data && q < node->data)` checks if both values are smaller than the current node. If so, we recurse left.

Similarly, `if (p > node->data && q > node->data)` checks if both values are larger and recurses right.

We `return node` if neither condition is true. Since the values split here, the current node must bethe LCA.

## In Rust

```rust
impl BST {
    fn lca(&self, p: i32, q: i32) -> Option<&Node> {
        Self::lca_node(self.root.as_ref(), p, q)
    }

    fn lca_node(node: Option<&Box<Node>>, p: i32, q: i32) -> Option<&Node> {
        match node {
            // Base case: empty node
            None => None,
            Some(current) => {
                // Both values are in the left subtree
                if p < current.data && q < current.data {
                    return Self::lca_node(current.left.as_ref(), p, q);
                }

                // Both values are in the right subtree
                if p > current.data && q > current.data {
                    return Self::lca_node(current.right.as_ref(), p, q);
                }

                // If they split, the current node is the LCA
                Some(current)
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

if let Some(node) = bst.lca(1, 4) {
    println!("{}", node.data);  // 3
}

if let Some(node) = bst.lca(1, 7) {
    println!("{}", node.data);  // 5
}

if let Some(node) = bst.lca(3, 4) {
    println!("{}", node.data);  // 3
}
```

`if p < current.data && q < current.data` checks if both values are smaller and recurses left, just like in C.

Similarly, `if p > current.data && q > current.data` checks if both values are larger and recurses right.

`Some(current)` returns the current node as the LCA when the values split.

## Complexity

| Operation | Time     | Worst case |
| --------- | -------- | ---------- |
| LCA       | O(log n) | O(n)       |

## Key Difference

|             | C                 | Rust                  |
| ----------- | ----------------- | --------------------- |
| Return type | `Node *`          | `Option<&Node>`       |
| Not found   | `return NULL`     | `None`                |
| Found LCA   | `return node`     | `Some(current)`       |
| Recurse     | `return lca(...)` | `Self::lca_node(...)` |
