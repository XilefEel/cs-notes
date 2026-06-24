# Deleting a Node

Deleting a node from an AVL tree is similar to [deleting from a BST](../binary-tree/operations/delete.md), but after every deletion we **update the height** and **rebalance** the node if needed. The same three cases from BST deletion still apply in AVL trees: no children, one child, or two children.

```
Delete [3] from an AVL tree:

        [4]                    [4]                      [4]
       /   \                  /   \                    /   \
     [2]   [5]    ->        [2]   [5]      ->        [2]   [5]
    /   \                  /                        /
  [1]   [3]              [1]                      [1]
```

## The Approach

- We recursively traverse the tree to find the node to delete, just like in a BST
- We handle one of the three cases: no children, one child, or two children
- For the two children case, replace with the **inorder successor**
- On the way back up the call stack, we **update the height** and **rebalance** each node

## In C

```c
Node *find_successor(Node *node) {
    Node *current = node->right;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node *delete_node(Node *node, int data) {
    // Base case: node not found
    if (node == NULL) return NULL;

    if (data < node->data) {
        node->left = delete_node(node->left, data);
    } else if (data > node->data) {
        node->right = delete_node(node->right, data);
    } else {
        // Case 1 & 2: no left child or no right child
        if (node->left == NULL) {
            Node *temp = node->right;
            free(node);
            return temp;
        }
        if (node->right == NULL) {
            Node *temp = node->left;
            free(node);
            return temp;
        }

        // Case 3: two children, replace with inorder successor
        Node *successor = find_successor(node);
        node->data = successor->data;
        node->left = delete_node(node->right, successor->data);
    }

    return rebalance(node);
}

void delete(AVLTree *tree, int data) {
    tree->root = delete_node(tree->root, data);
}

// Usage
AVLTree tree = create_avl_tree();
insert(&tree, 1);
insert(&tree, 2);
insert(&tree, 3);
insert(&tree, 4);
insert(&tree, 5);
//       [2]
//      /   \
//    [1]   [4]
//         /   \
//       [3]   [5]

delete(&tree, 1);
//       [2]                     [4]
//          \    rebalance!     /   \
//          [4]      ->       [2]   [5]
//         /   \                \
//       [3]   [5]              [3]

delete(&tree, 4);
//       [5]                      [3]
//      /        rebalance!      /   \
//    [2]           ->         [2]   [5]
//      \
//      [3]

delete(&tree, 3);
//       [5]
//      /
//    [2]
```

## In Rust

```rust
impl AVLTree {
    fn find_successor(node: &Box<Node>) -> i32 {
        let mut current = node.right.as_ref().unwrap();
        while current.left.is_some() {
            current = current.left.as_ref().unwrap();
        }
        current.data
    }

    fn delete_node(node: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match node {
            // Base case: node not found
            None => None,
            Some(mut current) => {
                if data < current.data {
                    current.left = Self::delete_node(current.left.take(), data);
                } else if data > current.data {
                    current.right = Self::delete_node(current.right.take(), data);
                } else {
                    // Case 1 & 2: no left child or no right child
                    if current.left.is_none() {
                        return current.right;
                    }
                    if current.right.is_none() {
                        return current.left;
                    }

                    // Case 3: two children, replace with inorder successor
                    let successor_data = Self::find_successor(&current);
                    current.data = successor_data;
                    current.left = Self::delete_node(current.left.take(), successor_data);
                }

                Some(Self::rebalance(current))
            }
        }
    }

    fn delete(&mut self, data: i32) {
        self.root = Self::delete_node(self.root.take(), data);
    }
}

// Usage
let mut tree = AVLTree::new();
tree.insert(1);
tree.insert(2);
tree.insert(3);
tree.insert(4);
tree.insert(5);
//       [2]
//      /   \
//    [1]   [4]
//          /   \
//        [3]   [5]

tree.delete(1);
//       [2]                     [4]
//          \     rebalance!    /   \
//          [4]      ->       [2]   [5]
//         /   \                \
//       [3]   [5]              [3]

tree.delete(4);
//       [5]                      [3]
//      /        rebalance!      /   \
//    [2]           ->         [2]   [5]
//      \
//      [3]

tree.delete(3)
//       [5]
//      /
//    [2]
```

## Complexity

| Operation | Average    | Worst case |
| --------- | ---------- | ---------- |
| Delete    | O(log n)   | O(log n)   |

Same with insertion, deletion in an AVL tree is guaranteed to be O(log n) because the tree is always balanced.

## Key Difference

|                  | C                                        | Rust                                      |
| ---------------- | ---------------------------------------- | ----------------------------------------- |
| Base case        | `return NULL`                            | `None => None`                            |
| Free node        | `free(node)`                             | Automatic                                 |
| Rebalance        | `return rebalance(node)`                 | `Some(Self::rebalance(current))`          |
| Move child       | Pointer assignment                       | `.take()` to move out                     |
