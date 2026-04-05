# Postorder Traversal

Postorder traversal is also very similar to inorder and preorder traversals. Postorder traversal visits nodes in this order: **Left → Right → Root**. We recursively traverse the left subtree first, then the right subtree, then visit the current node last.

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

Postorder: 1, 4, 3, 10, 7, 5
```

::: tip
Postorder traversal is useful when we need to **delete** a tree because it visits each node’s children before the parent, allowing us to safely free a node only after its children have already been freed.
:::

## The Approach

At each node:

1. **Recurse** into the left subtree
2. **Recurse** into the right subtree
3. **Visit** the current node

```
postorder([5])
├─ postorder([3])
│  ├─ postorder([1])
│  │  ├─ postorder(NULL)
│  │  ├─ postorder(NULL)
│  │  └─ visit [1] -> print 1
│  ├─ postorder([4])
│  │  ├─ postorder(NULL)
│  │  ├─ postorder(NULL)
│  │  └─ visit [4] -> print 4
│  └─ visit [3] -> print 3
└─ postorder([7])
   ├─ postorder(NULL)
   ├─ postorder([10])
   │  ├─ postorder(NULL)
   │  ├─ postorder(NULL)
   │  └─ visit [10] -> print 10
   └─ visit [7] -> print 7
└─ visit [5] -> print 5

Output: 1, 4, 3, 10, 7, 5
```

## In C

```c
// Recursively traverse the tree in postorder
void postorder(Node *node) {
    // Base case: empty node
    if (node == NULL) {
        return;
    }

    // Recurse into the left subtree
    postorder(node->left);

    // Recurse into the right subtree
    postorder(node->right);

    // Visit the current node
    printf("%d ", node->data);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

postorder(bst.root);  // 1 4 3 10 7 5
```

`if (node == NULL) return` is the base case. We stop when we hit an empty node.

`postorder(node->left)` recursively traverses the left subtree first.

`postorder(node->right)` recursively traverses the right subtree second.

`printf("%d ", node->data)` visits the current node last after both subtrees.

## In Rust

```rust
impl BST {
    fn postorder(&self) {
        Self::postorder_node(self.root.as_ref());
    }

    fn postorder_node(node: Option<&Box<Node>>) {
        match node {
            // Base case: empty node
            None => return,
            Some(current) => {
                // Recurse into the left subtree
                Self::postorder_node(current.left.as_ref());

                // Recurse into the right subtree
                Self::postorder_node(current.right.as_ref());

                // Visit the current node
                print!("{} ", current.data);
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

bst.postorder();  // 1 4 3 10 7 5
```

`Self::postorder_node(current.left.as_ref())` recursively traverses the left subtree first.

`Self::postorder_node(current.right.as_ref())` recursively traverses the right subtree second.

`print!("{} ", current.data)` visits the current node last after both subtrees.

## Complexity

| Operation           | Time | Space |
| ------------------- | ---- | ----- |
| Postorder Traversal | O(n) | O(h)  |

## Key Difference

|             | C                       | Rust                            |
| ----------- | ----------------------- | ------------------------------- |
| Base case   | `if (node == NULL)`     | `match node { None => return }` |
| Visit node  | `printf("%d ", ...)`    | `print!("{} ", ...)`            |
| Recurse     | `postorder(node->left)` | `Self::postorder_node(...)`     |
| Borrow node | Pointer access          | `.as_ref()`                     |
