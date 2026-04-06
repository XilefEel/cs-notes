# Implementing a Priority Queue

A **priority queue** is a data structure where each element has a **priority**, and elements are dequeued in order of priority rather than insertion order.

The challenge is to create a priority queue using only a **singly linked list**.

```
Enqueue elements with priorities:

enqueue(2, TOP):    HEAD -> [2:TOP] -> NULL
enqueue(5, NORMAL): HEAD -> [2:TOP] -> [5:NORMAL] -> NULL
enqueue(3, HIGH):   HEAD -> [2:TOP] -> [3:HIGH] -> [5:NORMAL] -> NULL
enqueue(8, LOW):    HEAD -> [2:TOP] -> [3:HIGH] -> [5:NORMAL] -> [8:LOW] -> NULL
enqueue(1, NORMAL): HEAD -> [2:TOP] -> [3:HIGH] -> [5:NORMAL] -> [1:NORMAL] -> [8:LOW] -> NULL

dequeue() -> 2  (always the head, highest priority)
```

## The Approach

We maintain a **sorted linked list** by priority. When we insert a new element:

- If the list is empty or the new element has **higher priority** than the head, insert at the head
- Otherwise, traverse until we find the correct position and insert there

Dequeuing is always O(1), just remove the head.

## In C

```c
typedef enum {
    TOP = 0,
    HIGH = 1,
    NORMAL = 2,
    LOW = 3,
} Priority;

typedef struct Node {
    int data;
    Priority priority;
    struct Node *next;
} Node;

Node *create_node(int data, Priority priority) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->priority = priority;
    node->next = NULL;
    return node;
}

void enqueue(Node **head, int data, Priority priority) {
    Node *node = create_node(data, priority);

    // Insert at head if list is empty or new node has higher priority
    if (*head == NULL || node->priority < (*head)->priority) {
        node->next = *head;
        *head = node;
        return;
    }

    // Traverse to find the correct position
    Node *current = *head;
    while (current->next != NULL && current->next->priority <= priority) {
        current = current->next;
    }

    // Point the new node to what current was pointing to
    node->next = current->next;

    // Point current to the new node
    current->next = node;
}

int dequeue(Node **head) {
    if (*head == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    Node *temp = *head;
    int data = temp->data;
    *head = (*head)->next;
    free(temp);
    return data;
}

// Usage
Node *head = NULL;
enqueue(&head, 5, NORMAL);
enqueue(&head, 3, HIGH);
enqueue(&head, 8, LOW);
enqueue(&head, 1, NORMAL);
enqueue(&head, 4, HIGH);
enqueue(&head, 7, LOW);
enqueue(&head, 2, TOP);

// HEAD -> [2:TOP] -> [3:HIGH] -> [4:HIGH] -> [5:NORMAL] -> [1:NORMAL] -> [8:LOW] -> [7:LOW] -> NULL

printf("%d\n", dequeue(&head));  // 2 (TOP)
printf("%d\n", dequeue(&head));  // 3 (HIGH)
printf("%d\n", dequeue(&head));  // 4 (HIGH)
```

`typedef enum` defines the priority levels as named constants. `TOP = 0` has the highest priority since lower numbers come first in the sorted list.

`if (node->priority < (*head)->priority)` inserts at the head directly if the new node has a strictly higher priority than the current head.

We traverse using `while (current->next != NULL && current->next->priority <= priority)` until we find a node with lower priority, then we insert before it.

`node->next = current->next` and `current->next = node` relinks the node into the correct position.

`dequeue()` simply removes and returns the head.

::: info Why lower number = higher priority?
We use lower numbers for higher priorities so that we can just sort in ascending order, which naturally gives us highest priority first. If we used higher numbers for higher priorities, we'd have to sort in descending order.
:::

::: tip
We use `<=` in `current->next->priority <= priority` so that elements with the same priority maintain their **insertion order**.
:::

## In Rust

```rust
#[derive(PartialEq, PartialOrd, Eq, Ord)]
enum Priority {
    Top = 0,
    High = 1,
    Normal = 2,
    Low = 3,
}

struct Node {
    data: i32,
    priority: Priority,
    next: Option<Box<Node>>,
}

fn create_node(data: i32, priority: Priority) -> Box<Node> {
    Box::new(Node { data, priority, next: None })
}

fn enqueue(head: Option<Box<Node>>, data: i32, priority: Priority) -> Option<Box<Node>> {
    let mut node = create_node(data, priority);

    match head {
        // Insert at head if list is empty or new node has higher priority
        None => Some(node),
        Some(current) if node.priority < current.priority => {
            node.next = Some(current);
            Some(node)
        }
        Some(mut current) => {
            // Traverse to find the correct position
            let mut walker = &mut current;
            while walker.next.is_some() &&
                  walker.next.as_ref().unwrap().priority <= node.priority {
                walker = walker.next.as_mut().unwrap();
            }
            
            // Point the new node to what current was pointing to
            node.next = walker.next.take();
            
            // Point current to the new node
            walker.next = Some(node);
            
            Some(current)
        }
    }
}

fn dequeue(head: &mut Option<Box<Node>>) -> Option<i32> {
    let node = head.take()?;
    *head = node.next;
    Some(node.data)
}

// Usage
let mut head = None;
head = enqueue(head, 5, Priority::Normal);
head = enqueue(head, 3, Priority::High);
head = enqueue(head, 8, Priority::Low);
head = enqueue(head, 1, Priority::Normal);
head = enqueue(head, 4, Priority::High);
head = enqueue(head, 7, Priority::Low);
head = enqueue(head, 2, Priority::Top);

// HEAD -> [2:Top] -> [3:High] -> [4:High] -> [5:Normal] -> [1:Normal] -> [8:Low] -> [7:Low] -> NONE

let data = dequeue(&mut head);
println!("{}", data.unwrap());  // 2 (Top)

let data = dequeue(&mut head);
println!("{}", data.unwrap());  // 3 (High)

let data = dequeue(&mut head);
println!("{}", data.unwrap());  // 4 (High)
```

`#[derive(PartialEq, PartialOrd, Eq, Ord)]` automatically implements comparison operators for `Priority` so we can use `<` and `<=` directly.

`Some(current) if node.priority < current.priority` is a **match guard**. It only matches if the new node has higher priority than the head.

We use `while walker.next.is_some() && walker.next.as_ref().unwrap().priority <= node.priority` to traverse to the correct insertion position, just like in C.

`node.next = walker.next.take()` and `walker.next = Some(node)` relinks the node into the correct position.

`dequeue` takes `&mut head` so the function can modify it directly. It's the same reason C uses `Node **head` instead of `Node *head`.

`head.take()` takes ownership of the head node, leaving `head` as `None`. The `?` returns `None` early if the queue is empty.

::: info What is #[derive]?
`#[derive(...)]` is a Rust **attribute** that automatically generates trait implementations for a type. `PartialOrd` and `Ord` give us comparison operators (`<`, `<=`, `>`, `>=`) based on the enum variant order. Without these, Rust wouldn't know how to compare two `Priority` values.
:::

::: info What is a match guard?
A **match guard** is an extra condition added to a match arm using `if`. In our code, the arm only matches if both the pattern (`Some(current)`) and the guard (`node.priority < current.priority`) are true.
:::

## Complexity

| Operation | Time | Space |
| --------- | ---- | ----- |
| Enqueue   | O(n) | O(1)  |
| Dequeue   | O(1) | O(1)  |

Enqueue is O(n) since we may need to traverse the entire list to find the correct position. Dequeue is O(1) since the head is always the highest priority element.

## Key Difference

|                  | C                              | Rust                              |
| ---------------- | ------------------------------ | --------------------------------- |
| Priority type    | `typedef enum`                 | `enum` with `#[derive(Ord)]`      |
| Compare priority | `<`, `<=` on enum values       | `<`, `<=` via derived `Ord`       |
| Insert at head   | `*head = newNode`              | Match guard + ownership transfer  |
| Dequeue          | Modify `*head` in place        | Modify `head` via `&mut`          |
