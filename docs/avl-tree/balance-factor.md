# What is a Balance Factor?

The **balance factor** of a node is the difference between the height of its **left subtree** and the height of its **right subtree**. We've already explored the concept of height in a binary tree in the [Finding the Height of a Binary Tree](../binary-tree/problems/height.md) section. The balance factor is calculated as:

```
balance factor = height(left) - height(right)
```

An AVL tree maintains the property that every node has a balance factor of **-1, 0, or 1**. If a node's balance factor is outside this range, the tree is considered **unbalanced** and needs to be rotated. 

## Calculating the Balance Factor

```
        [4]
       /   \
     [2]   [5]
    /   \
  [1]   [3]

height(left of [4])  = 2
height(right of [4]) = 1
balance factor of [4] = 2 - 1 = 1 ✓

height(left of [2])  = 1
height(right of [2]) = 1
balance factor of [2] = 1 - 1 = 0 ✓

height(left of [5])  = 0
height(right of [5]) = 0
balance factor of [5] = 0 - 0 = 0 ✓
```

Every node has a balance factor of -1, 0, or 1 so the tree is balanced. A leaf will always have a balance factor of 0 since both left and right heights are -1 (empty).

## When the Tree Becomes Unbalanced

A balance factor outside **-1, 0, or 1** means the tree needs to be rotated. There are four cases:

**Left Heavy (balance factor > 1):**
```
        [3]     <- bf = 2, unbalanced!
       /
     [2]        <- bf = 1
    /
  [1]           <- bf = 0
```

**Right Heavy (balance factor < -1):**
```
  [1]           <- bf = -2, unbalanced!
     \
     [2]        <- bf = -1
        \
        [3]     <- bf = 0
```

**Left-Right (left child is right heavy):**
```
     [3]        <- bf = 2, unbalanced!
    /
  [1]           <- bf = -1
     \
     [2]        <- bf = 0
```

**Right-Left (right child is left heavy):**
```
  [1]           <- bf = -2, unbalanced!
     \
     [3]        <- bf = 1
    /
  [2]           <- bf = 0
```

Each of these four cases requires a different **rotation** to restore balance, which we will cover in the [Rotations](./rotations/left-right.md) section later.

## Updating the Height

Since the balance factor depends on height, we need to update the height of a node after every insertion or deletion. The height of a node is:

```
height = 1 + max(height(left), height(right))
```

But calculating height from scratch every time would be inefficient, so we can store the height of each node directly in the node structure. After any change to the tree, we can update the height of affected nodes using the stored heights of their children.

```
        [4]
       /   \
     [2]   [5]
    /   \
  [1]   [3]

height([1]) = 0
height([3]) = 0
height([5]) = 0
height([2]) = 1 + max(0, 0) = 1
height([4]) = 1 + max(1, 0) = 2
```

## In C

```c
int height(Node *node) {
    if (node == NULL) return -1;
    return node->height;
}

int get_balance_factor(Node *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

void update_height(Node *node) {
    int left_height = height(node->left);
    int right_height = height(node->right);
    node->height = 1 + (left_height > right_height ? left_height : right_height);
}
```

`height(node)` returns `-1` for `NULL` nodes since the height of an empty tree is defined as `-1`. If it's not `NULL`, we just return the stored height of the node.

We return `height(left) - height(right)` to get the balance factor of a node.

`update_height(node)` recalculates the height of a node based on its children's heights using the ternary operator `(left_height > right_height ? left_height : right_height)` to find the maximum.

## In Rust

```rust
impl AVLTree {
    fn height(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => -1,
            Some(n) => n.height,
        }
    }

    fn get_balance_factor(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => 0,
            Some(n) => {
                Self::height(n.left.as_ref()) - Self::height(n.right.as_ref())
            }
        }
    }

    fn update_height(node: &mut Box<Node>) {
        let left_height = Self::height(node.left.as_ref());
        let right_height = Self::height(node.right.as_ref());
        node.height = 1 + left_height.max(right_height);
    }
}
```
`height` takes an `Option<&Box<Node>>` because we want to handle the case where the node might be `None` (empty) and we only want to read the height without taking ownership of the node.

`match node { None => -1, Some(n) => n.height }` returns `-1` for empty nodes and the stored height otherwise.

We use `as_ref()` in `Self::height(n.left.as_ref()) - Self::height(n.right.as_ref())` because `n.left` and `n.right` are `Option<Box<Node>>`, and we need to convert them to `Option<&Box<Node>>` to pass to the `height` function.

`left_height.max(right_height)` returns the maximum of the two heights. We could alternatively use `std::cmp::max(left_height, right_height)` to achieve the same result.

## Key Difference

|                  | C                                      | Rust                                   |
| ---------------- | -------------------------------------- | -------------------------------------- |
| NULL height      | `if (node == NULL) return -1`          | `None => -1`                           |
| Balance factor   | `height(left) - height(right)`         | `Self::height(left) - Self::height(right)` |
| Update height    | Direct field assignment                | `&mut Box<Node>`                       |
