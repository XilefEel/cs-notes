# Inorder Traversal

Inorder traversal is very simmilar to preorder traversal, but the order of visiting nodes is different. Inorder traversal visits nodes in this order: **Left → Root → Right**. We recursively traverse the left subtree first, then visit the current node, then finally the right subtree.

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

Inorder: 1, 3, 4, 5, 7, 10
```

::: tip
Inorder traversal of a BST always produces a **sorted sequence**. This is because we always visit the left (smaller) subtree before the current node, then the right (larger) subtree after.
:::

## The Approach

At each node:

1. **Recurse** into the left subtree
2. **Visit** the current node
3. **Recurse** into the right subtree

```
inorder([5])
├─ inorder([3])
│  ├─ inorder([1])
│  │  ├─ inorder(NULL)
│  │  ├─ visit [1] -> print 1
│  │  └─ inorder(NULL)
│  ├─ visit [3] -> print 3
│  └─ inorder([4])
│     ├─ inorder(NULL)
│     ├─ visit [4] -> print 4
│     └─ inorder(NULL)
├─ visit [5] -> print 5
└─ inorder([7])
   ├─ inorder(NULL)
   ├─ visit [7] -> print 7
   └─ inorder([10])
      ├─ inorder(NULL)
      ├─ visit [10] -> print 10
      └─ inorder(NULL)

Output: 1, 3, 4, 5, 7, 10
```

## In C

```c
// Recursively traverse the tree in inorder
void inorder(Node *node) {
    // Base case: empty node
    if (node == NULL) {
        return;
    }

    // Recurse into the left subtree
    inorder(node->left);

    // Print the current node
    printf("%d ", node->data);

    // Recurse into the right subtree
    inorder(node->right);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);

inorder(bst.root);  // 1 3 4 5 7 10
```

`if (node == NULL) return` is the base case. We stop when we hit an empty node.

`inorder(node->left)` recursively traverses the left subtree first.

`printf("%d ", node->data)` visits the current node after the left subtree.

`inorder(node->right)` recursively traverses the right subtree last.

::: tip
yes we just change the order of the code. that's it.
:::

## In Rust

```rust
impl BST {
    fn inorder(&self) {
        Self::inorder_node(self.root.as_ref());
    }

    fn inorder_node(node: Option<&Box<Node>>) {
        match node {
            // Base case: empty node
            None => return,
            Some(current) => {
                // Recurse into the left subtree
                Self::inorder_node(current.left.as_ref());

                // Print the current node
                print!("{} ", current.data);

                // Recurse into the right subtree
                Self::inorder_node(current.right.as_ref());
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

bst.inorder();  // 1 3 4 5 7 10
```

`Self::inorder_node(current.left.as_ref())` recursively traverses the left subtree first.

`print!("{} ", current.data)` visits the current node after the left subtree.

`Self::inorder_node(current.right.as_ref())` recursively traverses the right subtree last.

## Complexity

| Operation         | Time | Space |
| ----------------- | ---- | ----- |
| Inorder Traversal | O(n) | O(h)  |

## Key Difference

|             | C                     | Rust                            |
| ----------- | --------------------- | ------------------------------- |
| Base case   | `if (node == NULL)`   | `match node { None => return }` |
| Visit node  | `printf("%d ", ...)`  | `print!("{} ", ...)`            |
| Recurse     | `inorder(node->left)` | `Self::inorder_node(...)`       |
| Borrow node | Pointer access        | `.as_ref()`                     |
