# Balanced Parentheses

A common problem that can be solved using a stack is checking if a string of parentheses is **balanced**.

A string of parentheses is **balanced** if every opening bracket has a matching closing bracket in the correct order.

For example:

```
Balanced:
"(())"   -> valid
"()[]{}" -> valid
"([])"   -> valid

Not balanced:
"(]"     -> invalid, wrong closing bracket
"([)]"   -> invalid, wrong order
"((("    -> invalid, no closing brackets
```

## The Approach

We use a stack to keep track of opening brackets. We then loop through the string and for each character in the string:

- If it's an **opening bracket** `(`, `[`, `{`, we push it onto the stack.
- If it's a **closing bracket** `)`, `]`, `}`, we pop it from the stack and check if it matches. If it doesn't match, the string is **not balanced**.

If the stack is empty at the end, the string is **balanced**. If it's not empty, there are unmatched opening brackets, so it's **not balanced**.

```
Input: "([{}])"

Step 1: '(' is opening, push it
        STACK: ['(']

Step 2: '[' is opening, push it
        STACK: ['(', '[']

Step 3: '{' is opening, push it
        STACK: ['(', '[', '{']

Step 4: '}' is closing, pop '{' and check
        '}' matches '{' ✓
        STACK: ['(', '[']

Step 5: ']' is closing, pop '[' and check
        ']' matches '[' ✓
        STACK: ['(']

Step 6: ')' is closing, pop '(' and check
        ')' matches '(' ✓
        STACK: []

Stack is empty → Balanced!
```

## In C

Since our existing `Stack` stores `int`, we need to create a **separate** stack structs and functions that stores and accepts `char` for this problem. If we don't, we would have to convert characters to integers and back, which can get complicated.

```c
typedef struct CharNode {
    char data;
    struct CharNode *next;
} CharNode;

typedef struct {
    CharNode *top;
    int size;
} CharStack;

CharStack create_char_stack() {
    CharStack s;
    s.top = NULL;
    s.size = 0;
    return s;
}

void char_push(CharStack *s, char data) {
    CharNode *node = (CharNode *)malloc(sizeof(CharNode));
    node->data = data;
    node->next = s->top;
    s->top = node;
    s->size++;
}

char char_pop(CharStack *s) {
    CharNode *temp = s->top;
    char data = temp->data;
    s->top = s->top->next;
    free(temp);
    s->size--;
    return data;
}

int char_is_empty(CharStack *s) {
    return s->size == 0;
}

// Check if the parentheses in the string are balanced
int is_balanced(char *str) {
    CharStack s = create_char_stack();
    
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];

        // Push opening brackets onto the stack
        if (c == '(' || c == '[' || c == '{') {
            char_push(&s, c);
        }
        
        // If closing brackets, check if they match the top
        else if (c == ')' || c == ']' || c == '}') {
            // If stack is empty, there's no matching opening bracket
            if (char_is_empty(&s)) {
                return 0;
            }
            
            char top = char_pop(&s);

            // Check if the brackets match
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return 0;
            }
        }
    }

    // If the stack is empty, all brackets were matched
    return char_is_empty(&s);
}

// Usage
printf("%d\n", is_balanced("([{}])"));  // 1 (true)
printf("%d\n", is_balanced("([)]"));    // 0 (false)
printf("%d\n", is_balanced("((("));     // 0 (false)
```

`for (int i = 0; i < len; i++)` iterates through each character in the string.

`if (c == '(' || c == '[' || c == '{')` checks if the character is an opening bracket and pushes it onto the stack.

`if (char_is_empty(&s))` checks if the stack is empty before popping. If it is, there's no matching opening bracket.

`char top = char_pop(&s)` pops the top of the stack and checks if it matches the closing bracket.

`return char_is_empty(&s)` returns `1` if all brackets were matched (empty), otherwise `0` if there were unmatched opening brackets left (not empty).

::: info
One of C's biggest limitations is that there are **no generics**, so we have to **duplicate** the entire stack implementation just to change the data type. In Rust, we can solve this cleanly with generics.
:::

## In Rust

Instead of duplicating our `Stack` and its functions, we can instead refactor it using **generics** so it works with any type.

```rust
struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

struct Stack<T> {
    top: Option<Box<Node<T>>>,
    size: usize,
}

impl<T> Stack<T> {
    fn new() -> Stack<T> {
        Stack {
            top: None,
            size: 0,
        }
    }

    fn push(&mut self, data: T) {
        let node = Box::new(Node {
            data,
            next: self.top.take(),
        });
        self.top = Some(node);
        self.size += 1;
    }

    fn pop(&mut self) -> Option<T> {
        let node = self.top.take()?;
        self.top = node.next;
        self.size -= 1;
        Some(node.data)
    }

    fn peek(&self) -> Option<&T> {
        self.top.as_ref().map(|node| &node.data)
    }

    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

fn is_balanced(str: &str) -> bool {
    let mut s: Stack<char> = Stack::new();

    for c in str.chars() {
        match c {
            // Push opening brackets onto the stack
            '(' | '[' | '{' => s.push(c),

            // If closing brackets, check if they match the top
            ')' | ']' | '}' => {
                match s.pop() {
                    // If stack is empty, there's no matching opening bracket
                    None => return false,
                    Some(top) => {
                        // Check if the brackets match
                        if (c == ')' && top != '(') ||
                           (c == ']' && top != '[') ||
                           (c == '}' && top != '{') {
                            return false;
                        }
                    }
                }
            }
            _ => {}
        }
    }

    // If the stack is empty, all brackets were matched
    s.is_empty()
}

// Usage
println!("{}", is_balanced("([{}])"));  // true
println!("{}", is_balanced("([)]"));    // false
println!("{}", is_balanced("((("));     // false
```

`struct Node<T>` and `struct Stack<T>` replace the hardcoded `i32` with a **type parameter** `T`.

`impl<T> Stack<T>` implements the stack methods for any type `T`.

`let mut s: Stack<char> = Stack::new()` creates a stack that holds `char`.

`for c in str.chars()` iterates through each character in the string.

`match s.pop()` handles both cases. If the stack is empty (`None`) we return just `false`, otherwise (`Some(top)`) we check if the brackets match.

::: info What are generics?
Generics let us write code that works with **any type** instead of a specific one. Instead of writing a separate `CharStack`, `IntStack`, `FloatStack`, etc., we write `Stack<T>` once and Rust generates the right version at compile time based on how we use it. The letter `T` is a convention for "type", but we could use any letter or name, so long as it's consistent.

When we write `Stack<char>`, Rust creates a version of `Stack` where `T` is replaced with `char`. If we later write `Stack<i32>`, Rust creates another version where `T` is replaced with `i32`. This way, we can reuse the same stack implementation for **different data types** without duplicating code.
:::

::: tip
After introducing generics, our previous stack still works perfectly! We just need to replace `Stack::new()` with `Stack::<i32>::new()` wherever it was used before. 
:::

## Complexity

| Operation      | Time | Space |
| -------------- | ---- | ----- |
| Check balanced | O(n) | O(n)  |


## Key Difference

|                | C                            | Rust                     |
| -------------- | ---------------------------- | ------------------------ |
| Generic stack  | Not possible, duplicate code | `Stack<T>`               |
| Iterate string | `for` with `strlen`          | `str.chars()`            |
| Match brackets | `if/else if`                 | `match`                  |
| Empty check    | `char_is_empty(&s)`          | `s.is_empty()`           |
