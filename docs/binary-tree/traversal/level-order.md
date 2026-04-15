# Level Order Traversal (BFS)

Level order traversal visits nodes **level by level**, from left to right. Unlike the previous three traversals which used recursion, level order uses a [queue](../../queue/intro.md).

```
        [5]
       /   \
     [3]   [7]
    /   \     \
  [1]   [4]   [10]

Level 0:  5
Level 1:  3, 7
Level 2:  1, 4, 10

Output: 5, 3, 7, 1, 4, 10
```

## The Approach

We use a queue to keep track of nodes to visit. At each step:

1. **Dequeue** a node and visit it
2. **Enqueue** its left child if it exists
3. **Enqueue** its right child if it exists
4. Repeat until the queue is empty

```
Start: enqueue [5]
Queue: [[5]]

Dequeue [5], visit 5, enqueue [3] and [7]
Queue: [[3], [7]]

Dequeue [3], visit 3, enqueue [1] and [4]
Queue: [[7], [1], [4]]

Dequeue [7], visit 7, enqueue [10]
Queue: [[1], [4], [10]]

Dequeue [1], visit 1, no children
Queue: [[4], [10]]

Dequeue [4], visit 4, no children
Queue: [[10]]

Dequeue [10], visit 10, no children
Queue: []

Output: 5, 3, 7, 1, 4, 10
```

## In C

```c
// A queue that stores Node pointers for BFS
typedef struct BFSNode {
    Node *data;
    struct BFSNode *next;
} BFSNode;

typedef struct {
    BFSNode *front;
    BFSNode *back;
    int size;
} BFSQueue;

BFSQueue create_bfs_queue() {
    BFSQueue q;
    q.front = NULL;
    q.back = NULL;
    q.size = 0;
    return q;
}

void bfs_enqueue(BFSQueue *q, Node *data) {
    BFSNode *node = (BFSNode *)malloc(sizeof(BFSNode));
    node->data = data;
    node->next = NULL;
    if (q->back == NULL) {
        q->front = node;
        q->back = node;
    } else {
        q->back->next = node;
        q->back = node;
    }
    q->size++;
}

Node *bfs_dequeue(BFSQueue *q) {
    BFSNode *temp = q->front;
    Node *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->back = NULL;
    }
    free(temp);
    q->size--;
    return data;
}

int bfs_is_empty(BFSQueue *q) {
    return q->size == 0;
}

void level_order(BST *bst) {
    if (bst->root == NULL) {
        return;
    }

    BFSQueue q = create_bfs_queue();

    // Enqueue the root node to start the traversal
    bfs_enqueue(&q, bst->root);

    while (!bfs_is_empty(&q)) {
        // Dequeue the front node and print it
        Node *node = bfs_dequeue(&q);
        printf("%d ", node->data);

        // Enqueue left child if it exists
        if (node->left != NULL) {
            bfs_enqueue(&q, node->left);
        }

        // Enqueue right child if it exists
        if (node->right != NULL) {
            bfs_enqueue(&q, node->right);
        }
    }
}

// Usage
BST bst = create_bst();
insert(&bst, 5);
insert(&bst, 3);
insert(&bst, 7);
insert(&bst, 1);
insert(&bst, 4);
insert(&bst, 10);
```

`bfs_enqueue(&q, bst->root)` enqueues the root node to start the traversal.

`while (!bfs_is_empty(&q))` keeps going until all nodes have been visited.

`Node *node = bfs_dequeue(&q)` dequeues the front node first, then `printf("%d ", node->data);` visits it by printing its data.

`if (node->left != NULL) bfs_enqueue(&q, node->left)` enqueues the left child if it exists. Same for the right child.

::: info Why a separate BFSQueue?
Just like we needed a separate `CharStack` for the balanced parentheses problem, C has no generics so we need a separate `BFSQueue` that stores `Node *` instead of `int`. In Rust, we have generics so we can avoid duplicating code.
:::

## In Rust

```rust
impl BST {
    fn level_order(&self) {
        if self.root.is_none() {
            return;
        }

        let mut q: Queue<*const Node> = Queue::new();

        unsafe {
            // Enqueue the root node to start the traversal
            q.enqueue(self.root.as_deref().unwrap() as *const Node);

            while !q.is_empty() {
                // Dequeue the front node and print it
                let node = &*q.dequeue().unwrap();
                print!("{} ", node.data);

                // Enqueue left child if it exists
                if let Some(left) = node.left.as_deref() {
                    q.enqueue(left as *const Node);
                }

                // Enqueue right child if it exists
                if let Some(right) = node.right.as_deref() {
                    q.enqueue(right as *const Node);
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
bst.insert(10);

bst.level_order();  // 5 3 7 1 4 10
```

`Queue<*const Node>` creates a queue that stores raw pointers to nodes. We use raw pointers because we want to store references to nodes without taking ownership.

`self.root.as_deref().unwrap() as *const Node` gets a raw pointer to the root node without taking ownership.

`let node = &*q.dequeue().unwrap()` dequeues the raw pointer and dereferences it to get a reference to the node.

`node.left.as_deref()` borrows the left child without taking ownership so we can enqueue a pointer to it.

::: info What is *const Node?
`*const Node`is a raw pointer type that points to a `Node` but does not have any ownership to it. It is used here to store pointers to nodes in the queue without taking ownership of them, since we just want to read the nodes without modifying them. If we used `*mut Node`, it would imply we might modify the nodes, which is not what we are doing.

We use `unsafe` blocks to dereference these raw pointers when we need to access the node data or its children.
:::

::: info What is as_deref()?
`as_deref()` converts `Option<Box<T>>` to `Option<&T>`. It borrows the value inside the `Box` without taking ownership. It's essentially a shorthand for `.as_ref().map(|b| b.as_ref())`.
In our case, `self.root.as_deref()` converts `Option<Box<Node>>` to `Option<&Node>`, giving us a reference to the root node without moving it out of the tree.
:::

::: info Why raw pointers?
We can't enqueue `&Node` references directly because Rust's borrow checker would require us to prove the references stay valid for the entire duration of the traversal, which is difficult to express with our queue's lifetime. Raw pointers sidestep this, since the tree owns all the nodes and outlives the queue, so we know the pointers are always valid.
:::

## Complexity

| Operation             | Time | Space |
| --------------------- | ---- | ----- |
| Level Order Traversal | O(n) | O(w)  |

Space is O(w) where `w` is the maximum width of the tree or simply the maximum number of nodes at any level. For a balanced tree this is O(n/2) = O(n) in the worst case at the bottom level.

## Key Difference

|                | C                         | Rust                      |
| -------------- | ------------------------- | ------------------------- |
| Queue type     | `BFSQueue` of `Node *`    | `Queue<*const Node>`      |
| Dequeue        | `dequeue(&q)`             | `q.dequeue().unwrap()`    |
| Check children | `if (node->left != NULL)` | `if let Some(left) = ...` |
