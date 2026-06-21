# Left and Right Rotation

A **rotation** is the fundamental operation that allows us to restore balance in an AVL tree. It involves changing the structure of the tree to reduce the height of the taller subtree and increase the height of the shorter subtree. There are two basic rotations: **left rotation** and **right rotation**.

## Right Rotation

A right rotation is used when the tree is **left heavy**, which is when the left subtree is too tall. We rotate the unbalanced node **down to the right**.

```
    [3]                [2]
    /         ->      /   \
  [2]               [1]   [3]
 /
[1]
```

`[3]` is unbalanced with a balance factor of `2`. After rotating right, `[2]` becomes the new root and the tree is balanced.

### The Approach

- The **left child** becomes the new root
- The **old root** becomes the right child of the new root
- The **right subtree of the left child** moves to become the left child of the old root

```
Before:                   After:
       [y]                       [x]
      /   \                     /   \
    [x]   [T3]      ->       [T1]   [y]
   /   \                           /   \
 [T1] [T2]                       [T2] [T3]
```

::: info General case with subtrees
`T1`, `T2`, and `T3` represent arbitrary subtrees. Notice that the BST property is maintained after the rotation, since everything in `T1` and `T2` is still less than `[3]`, and everything in `T3` is still greater than `[2]`.
:::

### In C

```c
Node *rotate_right(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    update_height(y);
    update_height(x);

    return x;
}
```

`Node *x = y->left` saves the left child since it becomes the new root.

`Node *T2 = x->right` saves the right subtree of the left child since it will be moved.

`x->right = y` makes the old root the right child of the new root.

`y->left = T2` moves the saved subtree to the left of the old root.

`update_height(y)` updates the old root's height first since it is now lower in the tree.

`update_height(x)` updates the new root's height after since it depends on the old root's height.

`return x` returns the new root so the parent can relink it.

::: warning
We must update `y` before `x` since `x`'s height depends on `y`'s new height after the rotation.
:::

### In Rust

```rust
impl AVLTree {
    fn rotate_right(mut y: Box<Node>) -> Box<Node> {
        let mut x = y.left.take().unwrap();
        let t2 = x.right.take();

        // Perform rotation
        y.left = t2;
        update_height(&mut y);

        x.right = Some(y);
        update_height(&mut x);

        x
    }
}
```

`y.left.take().unwrap()` moves the left child out of `y`, giving us ownership of it as the new root.

`x.right.take()` moves the right subtree of `x` out so we can attach it to `y`.

`y.left = t2` attaches the saved subtree to the left of the old root.

`update_height(&mut y)` updates the old root's height first since it is now lower in the tree.

`x.right = Some(y)` makes the old root the right child of the new root, wrapping in `Some`.

`update_height(&mut x)` updates the new root's height after.

::: info Why unwrap() here?
We call `.unwrap()` on `y.left.take()` because we only ever call `rotate_right` when we know the left child exists, since a right rotation is only triggered when the tree is left heavy. This means `y.left` cannot be `None`, so it's safe to unwrap
:::

::: tip Ownership in a rotation
Notice that we update the height of `y` before setting `x.right = Some(y)`. This is because of ownership rules in Rust. Once we move `y` into `x.right`, we can no longer access it to update its height, so we must update `y`'s height before moving it. This is a key difference from C, where we can update `y`'s height after since we still have a pointer to it.
:::

## Left Rotation

A left rotation is the mirror image of a right rotation. It is used when the tree is **right heavy**, meaning that the right subtree is too tall. We rotate the unbalanced node **down to the left**.

```
[1]                  [2]
  \          ->     /   \
  [2]             [1]   [3]
    \
    [3]
```

`[1]` is unbalanced with a balance factor of `-2`. After rotating left, `[2]` becomes the new root and the tree is balanced.

### The Approach

- The **right child** becomes the new root
- The **old root** becomes the left child of the new root
- The **left subtree of the right child** moves to become the right child of the old root

```
Before:                      After:
    [y]                            [x]
   /   \                          /   \
 [T1]  [x]        ->            [y]  [T3]
      /   \                    /   \
    [T2] [T3]                [T1] [T2]
```

### In C

```c
Node *rotate_left(Node *y) {
    Node *x = y->right;
    Node *T2 = x->left;

    // Perform rotation
    x->left = y;
    y->right = T2;

    // Update heights
    update_height(y);
    update_height(x);

    return x;
}
```

`Node *x = y->right` saves the right child since it becomes the new root.

`Node *T2 = x->left` saves the left subtree of the right child since it will be moved.

`x->left = y` makes the old root the left child of the new root.

`y->right = T2` moves the saved subtree to the right of the old root.

`update_height(y)` updates the old root's height first since it is now lower in the tree.

`update_height(x)` updates the new root's height after since it depends on the old root's height.

`return x` returns the new root so the parent can relink it.

### In Rust

```rust
impl AVLTree {
    fn rotate_left(mut y: Box<Node>) -> Box<Node> {
        let mut x = y.right.take().unwrap();
        let t2 = x.left.take();

        // Perform rotation
        y.right = t2;
        update_height(&mut y);

        x.left = Some(y);
        update_height(&mut x);

        x
    }
}
```

`y.right.take().unwrap()` moves the right child out of `y`, giving us ownership of it as the new root.

`x.left.take()` moves the left subtree of `x` out so we can attach it to `y`.

`y.right = t2` attaches the saved subtree to the right of the old root.

`update_height(&mut y)` updates the old root's height first since it is now lower in the tree.

`x.left = Some(y)` makes the old root the left child of the new root, wrapping in `Some`.

`update_height(&mut x)` updates the new root's height after.

## Key Difference

|                  | C                              | Rust                              |
| ---------------- | ------------------------------ | --------------------------------- |
| Save child       | Pointer assignment             | `.take().unwrap()`                |
| Save subtree     | Pointer assignment             | `.take()`                         |
| Relink child     | Direct pointer assignment      | `Some(node)`                      |
| Update height    | `update_height(y)`             | `update_height(&mut y)`           |
