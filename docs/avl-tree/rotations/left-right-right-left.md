# Left-Right and Right-Left Rotation

The **left-right** and **right-left** rotations are **double rotations**. They are combinations of the two basic rotations from the [previous page](./left-right.md). They are needed when a single rotation isn't enough to restore balance.

## Left-Right Rotation

A left-right rotation is used when the tree is **left heavy** but the **left child is right heavy**. A single right rotation won't fix this, so we need to first **left rotate the left child**, then **right rotate the root**.

```
                   left rotate [1]       right rotate [3]
     [3]                 [3]                   [2]
    /         ->        /          ->         /   \
  [1]                 [2]                   [1]   [3]
     \               /
     [2]           [1]

```

### The Approach

- **Left rotate** the left child first to convert it into a left-left case
- Then **right rotate** the root to restore balance

```
Before:                      left rotate [x]:               right rotate [z]:
      [z]                            [z]                           [y]
     /   \                          /   \                         /   \
   [x]  [T4]          ->          [y]  [T4]          ->         [x]   [z]
  /   \                          /   \                         /  \   /  \
[T1]  [y]                      [x]  [T3]                    [T1][T2][T3][T4]
     /   \                    /   \
   [T2] [T3]                [T1] [T2]
```

### In C

```c
// Left-right case: left rotate child, then right rotate root
Node *rotate_left_right(Node *y) {
    y->left = rotate_left(y->left);
    return rotate_right(y);
}
```

`y->left = rotate_left(y->left)` left rotates the left child to convert it into a left-left case.

`return rotate_right(y)` right rotates the root to restore balance, returning the new root.

### In Rust

```rust
impl AVLTree {
    fn rotate_left_right(mut y: Box<Node>) -> Box<Node> {
        let left = y.left.take().unwrap();
        y.left = Some(Self::rotate_left(left));
        Self::rotate_right(y)
    }
}
```

`y.left.take().unwrap()` takes ownership of the left child so we can rotate it.

`Self::rotate_left(left)` left rotates the left child, returning the new subtree root.

`y.left = Some(...)` reattaches the rotated subtree as the new left child.

`Self::rotate_right(y)` right rotates the root and returns the new root.

## Right-Left Rotation

A right-left rotation is the mirror image of a left-right rotation. It is used when the tree is **right heavy** but the **right child is left heavy**. We first **right rotate the right child**, then **left rotate the root**.

```
               right rotate [3]       left rotate [1]
[1]                 [1]                   [2]
   \        ->         \       ->        /   \
   [3]                 [2]             [1]   [3]
  /                       \
[2]                       [3]
```

### The Approach

- **Right rotate** the right child first to convert it into a right-right case
- Then **left rotate** the root to restore balance

```
Before:                   right rotate [x]:                 left rotate [z]:
    [z]                            [z]                            [y]
   /   \                          /   \                          /   \
 [T1]  [x]           ->        [T1]   [y]          ->          [z]   [x]
      /   \                          /   \                    /  \   /  \
    [y]  [T4]                     [T2]   [x]                [T1][T2][T3][T4]
   /   \                                /   \
 [T2] [T3]                            [T3]  [T4]
```

### In C

```c
// Right-left case: right rotate child, then left rotate root
Node *rotate_right_left(Node *y) {
    y->right = rotate_right(y->right);
    return rotate_left(y);
}
```

`y->right = rotate_right(y->right)` right rotates the right child to convert it into a right-right case.

`return rotate_left(y)` left rotates the root to restore balance, returning the new root.

### In Rust

```rust
impl AVLTree {
    fn rotate_right_left(mut y: Box<Node>) -> Box<Node> {
        let right = y.right.take().unwrap();
        y.right = Some(Self::rotate_right(right));
        Self::rotate_left(y)
    }
}
```

`y.right.take().unwrap()` takes ownership of the right child so we can rotate it.

`Self::rotate_right(right)` right rotates the right child, returning the new subtree root.

`y.right = Some(...)` reattaches the rotated subtree as the new right child.

`Self::rotate_left(y)` left rotates the root and returns the new root.

## Key Difference

|                    | C                                    | Rust                                  |
| ------------------ | ------------------------------------ | ------------------------------------- |
| Left-right         | `y->left = rotate_left(y->left)`     | `.take().unwrap()` then `Some(...)`   |
| Right-left         | `y->right = rotate_right(y->right)`  | `.take().unwrap()` then `Some(...)`   |
| Return new root    | `return rotate_right/left(y)`        | `Self::rotate_right/left(y)`          |
| Ownership transfer | Not applicable                       | `.take()` to move out, `Some` to move in |
