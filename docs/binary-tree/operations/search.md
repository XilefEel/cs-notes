# Searching for a Node

Searching in a BST is efficient because at each node we can eliminate half the tree by deciding to go left or right based on the value we're looking for.

```
Search for 4 in:
        [5]
       /   \
     [3]   [7]
    /   \
  [1]   [4]

4 < 5, go left
4 > 3, go right
4 == 4, found!
```

## The Approach

We recursively traverse the tree starting from the root:

- If the current node is **empty**, the value is not in the tree
- If the value **equals** the current node, we found it
- If the value is **less than** the current node, recurse into the **left** subtree
- If the value is **greater than** the current node, recurse into the **right** subtree

## In C

```c
// Returns the node if found, NULL otherwise
Node *search(Node *node, int data) {
    // Base case: empty spot, value not in tree
    if (node == NULL) {
        return NULL;
    }

    // Found it!
    if (data == node->data) {
        return node;
    }

    // Recurse into the left or right subtree
    if (data < node->data) {
        return search(node->left, data);
    } else {
        return search(node->right, data);
    }
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);

Node *result = search(bst.root, 4);
if (result != NULL) {
    printf("Found: %d\n", result->data);    // Found: 4
} else {
    printf("Not found\n");
}

Node *missing = search(bst.root, 99);
if (missing != NULL) {
    printf("Found: %d\n", missing->data);
} else {
    printf("Not found\n");  // Not found
}
```

`if (node == NULL)` is the base case. If we've hit an empty spot, that means that the value doesn't exist in the tree.

`if (data == node->data)` checks if we've found the value and returns the node.

`return search(node->left, data)` recurses into the left subtree if the value is smaller.

`return search(node->right, data)` recurses into the right subtree if the value is larger.

## In Rust

```rust
impl BST {
    fn search(&self, data: i32) -> Option<&Node> {
        Self::search_node(self.root.as_ref(), data)
    }

    fn search_node(node: Option<&Box<Node>>, data: i32) -> Option<&Node> {
        match node {
            // Base case: empty spot, value not in tree
            None => None,
            Some(current) => {
                if data == current.data {
                    // Found it!
                    Some(current)
                } else if data < current.data {
                    // Recurse into the left subtree
                    Self::search_node(current.left.as_ref(), data)
                } else {
                    // Recurse into the right subtree
                    Self::search_node(current.right.as_ref(), data)
                }
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

match bst.search(4) {
    Some(node) => println!("Found: {}", node.data),     // Found: 4
    None => println!("Not found"),
}

match bst.search(99) {
    Some(node) => println!("Found: {}", node.data),
    None => println!("Not found"),                      // Not found
}
```

`&self` gives read-only access since we're only reading the tree, not modifying it.

`self.root.as_ref()` borrows the root without taking ownership.

`Self::search_node(current.left.as_ref(), data)` borrows the left child and recurses into it.

`Some(current)` returns a reference to the found node wrapped in `Some`.

## Complexity

| Operation | Average  | Worst case |
| --------- | -------- | ---------- |
| Search    | O(log n) | O(n)       |

## Key Difference

|             | C                    | Rust                     |
| ----------- | -------------------- | ------------------------ |
| Not found   | `return NULL`        | `None`                   |
| Found       | `return node`        | `Some(current)`          |
| Borrow node | Pointer access       | `.as_ref()` to borrow    |
| Recurse     | `return search(...)` | `Self::search_node(...)` |
