# Deleting a Node

Deleting a node from a BST is the most complex operation because we need to maintain the BST property after removal. There are three cases to handle when deleting a node:

## The Three Cases

**Case 1: Node has no children (leaf node)**. Simply remove it.

```
Delete [1]:
      [5]                  [5]
     /   \       ->       /   \
   [3]   [7]            [3]   [7]
  /   \                    \
[1]   [4]                  [4]
```

**Case 2: Node has one child**. Replace the node with its only child.

```
Delete [3]:
      [5]                  [5]
     /   \       ->       /   \
   [3]   [7]            [4]   [7]
      \
      [4]
```

**Case 3: Node has two children**. Find the **inorder successor**, which is the smallest node in the right subtree. Copy its value into the current node, then delete the inorder successor.

```
Delete [5]:
      [5]                  [6]
     /   \       ->       /   \
   [3]   [8]            [3]   [8]
  /  \  /   \          /   \     \
[1] [4][6]  [9]      [1]   [4]   [9]
```

::: info What is the inorder successor?
The **inorder successor** of a node is the smallest node in its **right subtree**. It is guaranteed to be the next largest value after the current node. We use it because replacing the deleted node with its inorder successor maintains the BST property since:

- It's larger than everything in the left subtree
- It's smaller than everything else in the right subtree
  :::

## The Approach

We recursively traverse the tree to find the node to delete, then handle one of the three cases:

1. **No children** — return `NULL`, the parent relinks to nothing
2. **One child** — return the child, the parent relinks to it
3. **Two children** — find the inorder successor, copy its value, delete the successor

## In C

```c
// Helper to find the inorder successor
Node *find_successor(Node *node) {
    node = node->right;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Recursively delete a node
Node *delete_node(Node *node, int data) {
    // Base case: node not found
    if (node == NULL) {
        return NULL;
    }

    if (data < node->data) {
        // Recurse into the left subtree
        node->left = delete_node(node->left, data);
    } else if (data > node->data) {
        // Recurse into the right subtree
        node->right = delete_node(node->right, data);
    } else {
        // Found the node to delete

        // Case 1 & 2: No left child or no right child
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

        // Case 3: Two children
        // Find the inorder successor
        Node *successor = find_successor(node);

        // Copy the successor's value into the current node
        node->data = successor->data;

        // Delete the successor from the right subtree
        node->right = delete_node(node->right, successor->data);
    }

    return node;
}

// The actual delete function that the user calls
void delete(BST *bst, int data) {
    bst->root = delete_node(bst->root, data);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

//        [5]
//       /   \
//     [3]   [7]
//    /   \     \
//  [1]   [4]   [10]

delete(&bst, 1);    // Case 1: leaf node
//        [5]
//       /   \
//     [3]   [7]
//       \     \
//       [4]   [10]

delete(&bst, 3);    // Case 2: one child
//        [5]
//       /   \
//     [4]   [7]
//              \
//              [10]

delete(&bst, 5);    // Case 3: two children
//        [7]
//       /   \
//     [4]   [10]
```

`find_successor()` goes to the right once and traverses as far left as possible to find the smallest node in the right subtree.

`if (node->left == NULL)` handles both Case 1 (no children) and Case 2 (only right child), which we just return the right child.

`if (node->right == NULL)` handles Case 2 with only a left child, which we return the left child.

`node->data = successor->data` copies the successor's value into the current node instead of relinking pointers.

`node->right = delete_node(node->right, successor->data)` deletes the successor from the right subtree, by deleting the node with the successor's value.

`free(node)` deallocates the deleted node.

::: tip
The inorder successor is guaranteed to always have **at most one child**, which is a right child. If it had a left child, that child would be smaller, which meant it would be the successor instead. This is why we can safely delete the successor with a simple Case 1 or Case 2 deletion.
:::

## In Rust

```rust
impl BST {
    // The actual delete function that the user calls
    fn delete(&mut self, data: i32) {
        self.root = Self::delete_node(self.root.take(), data);
    }

    // Helper to find the inorder successor
    fn find_successor(node: &Box<Node>) -> i32 {
       let mut current = node.right.as_ref().unwrap();
       while current.left.is_some() {
           current = current.left.as_ref().unwrap();
       }
       current.data
    }

    // Recursively delete a node
    fn delete_node(node: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match node {
            // Base case: node not found
            None => None,
            Some(mut current) => {
                if data < current.data {
                    // Recurse into the left subtree
                    current.left = Self::delete_node(current.left.take(), data);
                } else if data > current.data {
                    // Recurse into the right subtree
                    current.right = Self::delete_node(current.right.take(), data);
                } else {
                    // Found the node to delete

                    // Case 1 & 2: No left child or no right child
                    if current.left.is_none() {
                        return current.right;
                    }
                    if current.right.is_none() {
                        return current.left;
                    }

                    // Case 3: Two children
                    // Find the inorder successor
                    let successor_data = Self::find_successor(&current);

                    // Copy the successor's value into the current node
                    current.data = successor_data;

                    // Delete the successor from the right subtree
                    current.right = Self::delete_node(current.right.take(), successor_data);
                }

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

//        [5]
//       /   \
//     [3]   [7]
//    /   \     \
//  [1]   [4]   [10]

bst.delete(1);  // Case 1: leaf node
//        [5]
//       /   \
//     [3]   [7]
//       \     \
//       [4]   [10]

bst.delete(3);  // Case 2: one child
//        [5]
//       /   \
//     [4]   [7]
//              \
//              [10]

bst.delete(5);  // Case 3: two children
//        [7]
//       /   \
//     [4]   [10]
```

`if current.left.is_none() { return current.right }` handles Case 1 and Case 2 with only a right child, returning `current.right` which may be `None`.

`if current.right.is_none() { return current.left }` handles Case 2 with only a left child.

`Self::find_successor(&current)` finds the inorder successor's value without taking ownership.

`current.data = successor_data` copies the successor's value into the current node.

`current.right = Self::delete_node(current.right.take(), successor_data)` deletes the successor from the right subtree.

::: info Why does find_successor return i32 instead of &Node?
We only need the successor's **value** to copy into the current node, so we don't need a reference to the node itself. Returning `i32` keeps things simple and avoids lifetime issues that would come with returning a reference into the tree while also mutating it.
:::

## Complexity

| Operation | Average  | Worst case |
| --------- | -------- | ---------- |
| Delete    | O(log n) | O(n)       |

## Key Difference

|                | C                              | Rust                            |
| -------------- | ------------------------------ | ------------------------------- |
| Find minimum   | Iterative `while` loop         | Iterative `while` loop          |
| Case 1 & 2     | `return temp` after `free()`   | `return current.right/left`     |
| Copy successor | `node->data = successor->data` | `current.data = successor_data` |
| Free node      | `free(node)`                   | Automatic                       |
| Base case      | `return NULL`                  | `None`                          |
