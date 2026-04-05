# Inserting a Node

Inserting a node into a BST is different from inserting into a linked list because we need to maintain the BST property. This means finding the correct position that maintains the property that smaller values go left, larger values go right.

```
Insert 5, 3, 7, 1, 4 into an empty BST:

Insert 5:            Insert 3:           Insert 7:
    [5]                 [5]                 [5]
                       /                   /   \
                     [3]                 [3]   [7]

Insert 1:            Insert 4:
    [5]                 [5]
   /   \               /   \
 [3]   [7]           [3]   [7]
 /                  /   \
[1]               [1]   [4]
```

## The Approach

We recursively traverse the tree starting from the root:

- If the tree is **empty**, the new node becomes the root
- If the value is **less than** the current node, recurse into the **left** subtree
- If the value is **greater than** the current node, recurse into the **right** subtree
- When we reach an empty spot, insert the new node there

## In C

```c
// Helper function to recursively insert a node
Node *insert_node(Node *node, int data) {
    // Base case: if we found an empty spot, insert the new node here
    if (node == NULL) {
        return create_node(data);
    }

    // Recurse into the left or right subtree
    if (data < node->data) {
        node->left = insert_node(node->left, data);
    } else if (data > node->data) {
        node->right = insert_node(node->right, data);
    }

    // Return the current node (unchanged if data already exists)
    return node;
}

// The actual insert function that the user calls
void insert(BST *bst, int data) {
    bst->root = insert_node(bst->root, data);
}

// Usage
BST bst = create_bst();
insert(&bst, 5);    //   [5]


insert(&bst, 3);    //     [5]
                    //    /
                    //  [3]

insert(&bst, 7);    //    [5]
                    //   /   \
                    // [3]   [7]

insert(&bst, 1);
//       [5]
//      /   \
//    [3]   [7]
//   /
// [1]

insert(&bst, 4);
//       [5]
//      /   \
//    [3]   [7]
//   /   \
// [1]   [4]
```

`if (node == NULL)` is the base case when we found the empty spot where the new node belongs.

`if (data < node->data)` recurses into the left subtree if the value is smaller.

`else if (data > node->data)` recurses into the right subtree if the value is larger.

`node->left = insert_node(node->left, data)` updates the left child with the result of the recursive call. This is how the new node gets linked into the tree on the way back up the call stack. Same goes for the right child.

`return node` returns the current node unchanged so the parent can relink it correctly.

**Call stack visualization — inserting `4` into an existing tree:**

```
Before:
        [5]
       /   \
     [3]   [7]
    /
  [1]

insert_node([5], 4)
├─ 4 < 5, go left
├─ node->left = insert_node([3], 4)
│  ├─ 4 > 3, go right
│  ├─ node->right = insert_node(NULL, 4)
│  │  ├─ node is NULL, create new node [4]
│  │  └─ return [4]   <- new node returned to parent
│  ├─ [3]->right = [4]
│  └─ return [3]      <- [3] returned unchanged
├─ [5]->left = [3]
└─ return [5]         <- [5] returned unchanged

Result:
        [5]
       /   \
     [3]   [7]
    /   \
  [1]   [4]
```

::: info Why return the node?
Instead of modifying the tree directly with pointers, we can return the node at each level and let the parent relink it. This makes the recursive logic clean since each call either returns the same node unchanged or a new node if we've reached the insertion point.
:::

::: warning
If the value already exists in the tree, we do nothing and return the current node unchanged. This ensures we don't insert duplicates. In production code, you might want to handle this case differently.
:::

## In Rust

```rust
impl BST {
    // The actual insert function that the user calls
    fn insert(&mut self, data: i32) {
        self.root = Self::insert_node(self.root.take(), data);
    }

    // Helper function to recursively insert a node
    fn insert_node(node: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match node {
            // Base case: if we found an empty spot, insert the new node here
            None => Some(Box::new(Node {
                data,
                left: None,
                right: None,
            })),
            // Recurse into the left or right subtree
            Some(mut current) => {
                if data < current.data {
                    current.left = Self::insert_node(current.left.take(), data);
                } else if data > current.data {
                    current.right = Self::insert_node(current.right.take(), data);
                }

                // Return the current node (unchanged if data already exists)
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

//       [5]
//      /   \
//    [3]   [7]
//   /   \
// [1]   [4]
```

`self.root = Self::insert_node(self.root.take(), data)` takes ownership of the root, passes it into the recursive helper, and assigns the result back to `self.root`.

`match node` handles both cases: `None` is the base case where we insert, `Some(mut current)` is where we keep traversing.

`current.left = Self::insert_node(current.left.take(), data)` takes ownership of the left child, recurses into it, and assigns the result back, just like in C.

`current.left.take()` moves the left child out of the node so we can pass it into the recursive call.

`Some(current)` returns the current node wrapped in `Some` so the parent can relink it.

::: info Why Self::insert_node instead of &mut self?
`Self::` is used to call the static method `insert_node` from within the instance method `insert`.

Notice that `insert_node` doesn't take `self`. This is intentional so that we can avoid the borrow checker issue of having a mutable reference to `self` while also trying to move parts of it into recursive calls. By making `insert_node` a static helper function that just passes nodes by value and returns them, we can sidestep the problem entirely.
:::

## Complexity

| Operation | Average  | Worst case |
| --------- | -------- | ---------- |
| Insert    | O(log n) | O(n)       |

The worst case is O(n) when the tree is completely unbalanced. For example, inserting 1, 2, 3, 4, 5 in order creates a straight line like a linked list.

## Key Difference

|              | C                               | Rust                                    |
| ------------ | ------------------------------- | --------------------------------------- |
| Base case    | `if (node == NULL)`             | `match node { None => ... }`            |
| Recurse left | `node->left = insert_node(...)` | `current.left = Self::insert_node(...)` |
| Move child   | Pointer assignment              | `.take()` to transfer ownership         |
| Return node  | `return node`                   | `Some(current)`                         |
