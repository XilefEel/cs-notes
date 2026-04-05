# Creating a BST

To create a BST, we need two things:

- A **node** to store each element on the heap that has a `left` and `right` pointer
- A **BST struct** that holds a pointer to the root

An empty BST is simply a BST with `root` pointing to nothing.

## In C

```c
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
} BST;

// Creates a new empty BST
BST create_bst() {
    BST bst;
    bst.root = NULL;
    return bst;
}

// Usage
BST bst = create_bst();
```

`left` points to the left child of the node.

`right` points to the right child of the node.

`create_bst()` returns a BST with `root = NULL`, representing an empty tree.

## In Rust

```rust
struct Node {
    data: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

struct BST {
    root: Option<Box<Node>>,
}

impl BST {
    fn new() -> BST {
        BST { root: None }
    }
}

// Usage
let mut bst = BST::new();
```

`left` and `right` are both `Option<Box<Node>>` since they can either point to a child node (`Box<Node>`) or be empty (`None`).

`BST::new()` returns a BST with `root = None`, representing an empty tree.

::: info Ownership in a BST
In Rust, a value can only have **one** owner. In a BST, each node is owned by exactly **one parent**. There are no cycles, no shared ownership, just a clean tree of ownership flowing from root to leaves. This is why `Option<Box<Node>>` works perfectly here, unlike a doubly linked list where `prev` pointers would create cycles of ownership that the borrow checker rejects.
:::

## Key Difference

|             | C              | Rust                |
| ----------- | -------------- | ------------------- |
| Left child  | `Node *left`   | `Option<Box<Node>>` |
| Right child | `Node *right`  | `Option<Box<Node>>` |
| Empty tree  | `root = NULL`  | `root = None`       |
| Create      | `create_bst()` | `BST::new()`        |
