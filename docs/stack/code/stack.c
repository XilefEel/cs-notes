#include <stdio.h>
#include <stdlib.h>

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

int main() {
    Stack s = create_stack();
    push(&s, 10);       // TOP -> [10] -> NULL
    push(&s, 20);       // TOP -> [20] -> [10] -> NULL
    push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

    int a = pop(&s);    // a = 30, TOP -> [20] -> [10] -> NULL
    int b = pop(&s);    // b = 20, TOP -> [10] -> NULL
    int c = pop(&s);    // c = 10, TOP -> NULL
    int d = pop(&s);    // d = -1, stack is empty

    is_empty(&s);       // 1 (true)

    push(&s, 10);       // TOP -> [10] -> NULL
    push(&s, 20);       // TOP -> [20] -> [10] -> NULL
    push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

    a = peek(&s);       // a = 30, stack unchanged
    b = peek(&s);       // b = 30, stack unchanged

    is_empty(&s);       // 0 (false)

    return 0;
}
