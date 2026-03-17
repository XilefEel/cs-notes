#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    int size;
} Stack;

Stack create_stack() {
    Stack s;
    s.top = NULL;
    s.size = 0;
    return s;
}

void push(Stack *s, int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;

    node->next = s->top;
    s->top = node;

    s->size++;
}

int pop(Stack *s) {
    if (s->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    Node *temp = s->top;
    int data = temp->data;

    s->top = s->top->next;
    s->size--;

    free(temp);

    return data;
}

int peek(Stack *s) {
    if (s->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }

    return s->top->data;
}

int is_empty(Stack *s) {
    return s->size == 0;
}

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

int is_balanced(char *str) {
    CharStack s = create_char_stack();

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];

        if (c == '(' || c == '[' || c == '{') {
            char_push(&s, c);
        }

        else if (c == ')' || c == ']' || c == '}') {
            if (char_is_empty(&s)) {
                return 0;
            }

            char top = char_pop(&s);

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return 0;
            }
        }
    }

    return char_is_empty(&s);
}

typedef struct {
    Stack stack;
    Stack min_stack;
} MinStack;

MinStack create_min_stack() {
    MinStack ms;
    ms.stack = create_stack();
    ms.min_stack = create_stack();
    return ms;
}

void min_push(MinStack *ms, int data) {
    push(&ms->stack, data);

    if (is_empty(&ms->min_stack) || data <= peek(&ms->min_stack)) {
        push(&ms->min_stack, data);
    }
}

int min_pop(MinStack *ms) {
    int data = pop(&ms->stack);

    if (data == peek(&ms->min_stack)) {
        pop(&ms->min_stack);
    }

    return data;
}

int get_min(MinStack *ms) {
    if (is_empty(&ms->min_stack)) return -1;

    return peek(&ms->min_stack);
}

int main() {
    Stack s = create_stack();
    push(&s, 10);       // TOP -> [10] -> NULL
    push(&s, 20);       // TOP -> [20] -> [10] -> NULL
    push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

    int a = pop(&s);    // a = 30, TOP -> [20] -> [10] -> NULL
    int b = pop(&s);    // b = 20, TOP -> [10] -> NULL
    int c = pop(&s);    // c = 10, TOP -> NULL
    int d = pop(&s);    // d = -1, stack is empty

    printf("%d %d %d %d\n", a, b, c, d);

    is_empty(&s);       // 1 (true)

    push(&s, 10);       // TOP -> [10] -> NULL
    push(&s, 20);       // TOP -> [20] -> [10] -> NULL
    push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

    int e = peek(&s);       // e = 30, stack unchanged
    int f = peek(&s);       // f = 30, stack unchanged

    printf("%d %d\n", e, f);

    is_empty(&s);       // 0 (false)

    printf("%d\n", is_balanced("([{}])"));  // 1 (true)
    printf("%d\n", is_balanced("([)]"));    // 0 (false)
    printf("%d\n", is_balanced("((("));     // 0 (false)

    MinStack ms = create_min_stack();
    min_push(&ms, 3);   // stack: [3],          min: [3]
    min_push(&ms, 5);   // stack: [3, 5],       min: [3]
    min_push(&ms, 2);   // stack: [3, 5, 2],    min: [3, 2]
    min_push(&ms, 1);   // stack: [3, 5, 2, 1], min: [3, 2, 1]

    printf("%d\n", get_min(&ms));   // 1

    min_pop(&ms);                   // stack: [3, 5, 2],    min: [3, 2]
    printf("%d\n", get_min(&ms));   // 2

    min_pop(&ms);                   // stack: [3, 5],       min: [3]
    printf("%d\n", get_min(&ms));   // 3

    return 0;
}
