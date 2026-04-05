# Preorder Traversal

Preorder traversal visits nodes in this order: **Root -> Left -> Right**. We visit the current node first, then recursively traverse the left subtree, then the right subtree.

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

Preorder: 5, 3, 1, 4, 7, 10
```

## The Approach

At each node:

1. **Visit** the current node
2. **Recurse** into the left subtree
3. **Recurse** into the right subtree

```
preorder([5])
├─ visit [5] -> print 5
├─ preorder([3])
│  ├─ visit [3] -> print 3
│  ├─ preorder([1])
│  │  ├─ visit [1] -> print 1
│  │  ├─ preorder(NULL)
│  │  └─ preorder(NULL)
│  └─ preorder([4])
│     ├─ visit [4] -> print 4
│     ├─ preorder(NULL)
│     └─ preorder(NULL)
└─ preorder([7])
   ├─ visit [7] -> print 7
   ├─ preorder(NULL)
   └─ preorder([10])
      ├─ visit [10] -> print 10
      ├─ preorder(NULL)
      └─ preorder(NULL)

Output: 5, 3, 1, 4, 7, 10
```

## In C

```c
// Recursively traverse the tree in preorder
void preorder(Node *node) {
    // Base case: empty node
    if (node == NULL) {
        return;
    }

    // Print the current node
    printf("%d ", node->data);

    // Recurse into the left subtree
    preorder(node->left);

    // Recurse into the right subtree
    preorder(node->right);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

preorder(bst.root); // 5 3 1 4 7 10
```

`if (node == NULL) return` is the base case. We stop when we hit an empty node.

`printf("%d ", node->data)` visits the current node and prints its value.

`preorder(node->left)` recursively traverses the left subtree.

`preorder(node->right)` recursively traverses the right subtree.

## In Rust

```rust
impl BST {
    fn preorder(&self) {
        Self::preorder_node(self.root.as_ref());
    }

    fn preorder_node(node: Option<&Box<Node>>) {
        match node {
            // Base case: empty node
            None => return,
            Some(current) => {
                // Print the current node
                print!("{} ", current.data);

                // Recurse into the left subtree
                Self::preorder_node(current.left.as_ref());

                // Recurse into the right subtree
                Self::preorder_node(current.right.as_ref());
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

bst.preorder();  // 5 3 1 4 7 10
```

`self.root.as_ref()` borrows the root without taking ownership since we're only reading.

`None => return` is the base case. We stop when we hit an empty node.

`print!("{} ", current.data)` visits the current node and prints its value.

`Self::preorder_node(current.left.as_ref())` recursively traverses the left subtree.

`Self::preorder_node(current.right.as_ref())` recursively traverses the right subtree.

## Complexity

| Operation          | Time | Space |
| ------------------ | ---- | ----- |
| Preorder Traversal | O(n) | O(h)  |

Space is O(h) where `h` is the height of the tree. This is the maximum depth of the call stack at any point. For a balanced tree this is O(log n), for an unbalanced tree it's O(n).

## Key Difference

|             | C                      | Rust                            |
| ----------- | ---------------------- | ------------------------------- |
| Base case   | `if (node == NULL)`    | `match node { None => return }` |
| Visit node  | `printf("%d ", ...)`   | `print!("{} ", ...)`            |
| Recurse     | `preorder(node->left)` | `Self::preorder_node(...)`      |
| Borrow node | Pointer access         | `.as_ref()`                     |
