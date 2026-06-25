# Inserting a Node

Inserting a node into an AVL tree is similar to [inserting into a BST](../binary-tree/operations/insert.md), but after every insertion we **update the height** and **rebalance** the node if needed. This ensures the tree stays balanced at all times.

```
Insert 1, 2, 3 into an AVL tree:

      [1]                 [2]
         \               /   \
         [2]           [1]   [3]
            \
            [3]
     unbalanced!     after rebalance
```

## The Approach

- We recursively traverse the tree to find the right place to insert, just like in a BST
- When we reach an empty spot, we create a new node there
- On the way back up the call stack, we **update the height** of each node
- If its balance factor falls outside **-1, 0, or 1**, we **rebalance** it

## Rebalance

The `rebalance()` is a helper function that checks the balance factor of the node and performs rotations if necessary. This helper function will be called after every **insertion** (and deletion) to ensure the tree remains balanced.

### In C

```c
Node *rebalance(Node *n) {
    // Update the height of the node
    update_height(n);
    int bf = get_balance_factor(n);

    // Left-left case: single right rotation
    if (bf > 1 && get_balance_factor(n->left) >= 0)
        return rotate_right(n);

    // Right-right case: single left rotation
    if (bf < -1 && get_balance_factor(n->right) <= 0)
        return rotate_left(n);

    // Left-right case: left rotate child, then right rotate root
    if (bf > 1 && get_balance_factor(n->left) < 0) {
        n->left = rotate_left(n->left);
        return rotate_right(n);
    }

    // Right-left case: right rotate child, then left rotate root
    if (bf < -1 && get_balance_factor(n->right) > 0) {
        n->right = rotate_right(n->right);
        return rotate_left(n);
    }

    return n;
}
```
`bf > 1 && get_balance_factor(n->left) >= 0` means the node is left-heavy and its left child is also left-heavy (left-left case).

`bf < -1 && get_balance_factor(n->right) <= 0` means the node is right-heavy and its right child is also right-heavy (right-right case).

`bf > 1 && get_balance_factor(n->left) < 0` means the node is left-heavy but its left child is right-heavy (left-right case).

`bf < -1 && get_balance_factor(n->right) > 0` means the node is right-heavy but its right child is left-heavy (right-left case).

`return n` returns the node unchanged if it is already balanced.

### In Rust

```rust
impl AVLTree {
    fn rebalance(mut n: Box<Node>) -> Box<Node> {
        Self::update_height(&mut n);
        let bf = Self::get_balance_factor(Some(&n));

        // Left-left case: single right rotation
        if bf > 1 && Self::get_balance_factor(n.left.as_ref()) >= 0 {
            return Self::rotate_right(n);
        }

        // Right-right case: single left rotation
        if bf < -1 && Self::get_balance_factor(n.right.as_ref()) <= 0 {
            return Self::rotate_left(n);
        }

        // Left-right case: left rotate child, then right rotate root
        if bf > 1 && Self::get_balance_factor(n.left.as_ref()) < 0 {
            let left = n.left.take().unwrap();
            n.left = Some(Self::rotate_left(left));
            return Self::rotate_right(n);
        }

        // Right-left case: right rotate child, then left rotate root
        if bf < -1 && Self::get_balance_factor(n.right.as_ref()) > 0 {
            let right = n.right.take().unwrap();
            n.right = Some(Self::rotate_right(right));
            return Self::rotate_left(n);
        }

        n
    }
}
```

`Self::get_balance_factor(Some(&n))` gets the balance factor of the current node. We wrap `&n` in `Some` because `get_balance_factor()` takes an `Option<&Node>`.

`n.left.take().unwrap()` takes ownership of the left child and unwraps it, so we can pass it into the rotation function.

`n.left = Some(Self::rotate_left(left))` threattaches the rotated left child and wraps it in `Some`. Same for the right child in the right-left case.

## Insert

### In C

```c
Node *insert_node(Node *node, int data) {
    // Base case: found the empty spot
    if (node == NULL) {
        return create_node(data);
    }

    // Recurse into the left or right subtree
    if (data < node->data) {
        node->left = insert_node(node->left, data);
    } else if (data > node->data) {
        node->right = insert_node(node->right, data);
    } else {
        return node;
    }

    // Update the height and rebalance the node on the way back up
    return rebalance(node);
}

void insert(AVLTree *tree, int data) {
    tree->root = insert_node(tree->root, data);
}

// Usage
AVLTree tree = create_avl_tree();

insert(&tree, 3); //   [3]

insert(&tree, 2); //     [3]
                  //    /
                  //  [2]

insert(&tree, 1);
//       [3]          rebalance!        [2]
//      /                ->            /   \
//    [2]                            [1]   [3]
//   /
// [1]

insert(&tree, 4);
//       [2]
//      /   \
//    [1]   [3]
//             \
//             [4]

insert(&tree, 5);
//       [2]                          [2]
//      /   \        rebalance!      /   \
//    [1]   [3]          ->        [1]   [4]
//             \                        /   \
//             [4]                    [3]   [5]
//                \
//                [5]
```

The code is basically the same as insertion in a BST, but we call `rebalance(node)` on the way back up the call stack to ensure the tree remains balanced.

### In Rust

```rust
impl AVLTree {
    fn insert_node(node: Option<Box<Node>>, data: i32) -> Box<Node> {
        match node {
            // Base case: found the empty spot
            None => Box::new(Node {
                data,
                height: 0,
                left: None,
                right: None,
            }),
            // Recurse into the left or right subtree
            Some(mut current) => {
                if data < current.data {
                    current.left = Some(Self::insert_node(current.left.take(), data));
                } else if data > current.data {
                    current.right = Some(Self::insert_node(current.right.take(), data));
                } else {
                    return current;
                }
                // Update the height and rebalance the node on the way back up
                Self::rebalance(current)
            }
        }
    }

    fn insert(&mut self, data: i32) {
        self.root = Some(Self::insert_node(self.root.take(), data));
    }
}

// Usage
let mut tree = AVLTree::new();

tree.insert(3);
tree.insert(2);
tree.insert(1);
//       [3]          rebalance!        [2]
//      /                ->            /   \
//    [2]                            [1]   [3]
//   /
// [1]

tree.insert(4);
tree.insert(5);
//       [2]                          [2]
//      /   \        rebalance!      /   \
//    [1]   [3]          ->        [1]   [4]
//             \                        /   \
//             [4]                    [3]   [5]
//                \
//                [5]
```

Just like in C, the code is basically the same as insertion in a BST, but we call `Self::rebalance(current)` to update the height and rebalance the node on the way back up the call stack.

## Complexity

| Operation | Average    | Worst case |
| --------- | ---------- | ---------- |
| Insert    | O(log n)   | O(log n)   |

Unlike a regular BST where worst case is O(n), an AVL tree guarantees O(log n) since it always stays balanced.

## Key Difference

|                  | C                              | Rust                                  |
| ---------------- | ------------------------------ | ------------------------------------- |
| Base case        | `return create_node(data)`     | `None => Box::new(Node { ... })`      |
| Recurse left     | `node->left = insert_node(..)` | `Some(Self::insert_node(..))`         |
| Move child       | Pointer assignment             | `.take()` to move out                 |
| Rebalance        | `return rebalance(node)`       | `Self::rebalance(current)`            |
