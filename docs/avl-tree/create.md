# Creating an AVL Tree

Creating an AVL tree is similar to [creating a BST](../binary-tree/create.md), but each node stores an extra **height** field.

An empty AVL tree is simply a tree with no nodes and a root pointing to nothing.

## In C

```c
typedef struct Node {
    int data;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
} AVLTree;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLTree create_avl_tree() {
    AVLTree tree;
    tree.root = NULL;
    return tree;
}

// Usage
AVLTree tree = create_avl_tree();
```

Unlike our BST node, we now have `int height` to store the height of the node.

`create_node()` allocates memory for a new node with height `0` since it has no children.

## In Rust

```rust
struct Node {
    data: i32,
    height: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

struct AVLTree {
    root: Option<Box<Node>>,
}

impl AVLTree {
    fn new() -> AVLTree {
        AVLTree { root: None }
    }
}

// Usage
let mut tree = AVLTree::new();
```

## Key Difference

|                  | C                          | Rust                        |
| ---------------- | -------------------------- | --------------------------- |
| Node struct      | `typedef struct Node`      | `struct Node`               |
| Height field     | `int height`               | `height: i32`               |
| Empty child      | `NULL`                     | `None`                      |
| Node allocation  | `malloc(sizeof(Node))`     | `Box::new(Node { ... })`    |
| Empty tree       | `root = NULL`              | `root: None`                |
| Initial height   | `node->height = 0`         | `height: 0`                 |
