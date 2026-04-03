#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *back;
    int size;
} Queue;

Queue create_queue() {
    Queue q;
    q.front = NULL;
    q.back = NULL;
    q.size = 0;
    return q;
}

void enqueue(Queue *q, int data) {
    Node *node = (Node *)malloc(sizeof(Node));
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

int dequeue(Queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    Node *temp = q->front;
    int data = temp->data;

    q->front = q->front->next;

    if (q->front == NULL) {
        q->back = NULL;
    }

    free(temp);
    q->size--;

    return data;
}

int peek(Queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    return q->front->data;
}

int is_empty(Queue *q) {
    return q->size == 0;
}

int main() {
    Queue q = create_queue();

    is_empty(&q);   // 1 (true)

    enqueue(&q, 10);     // FRONT -> [10] <- BACK
    enqueue(&q, 20);     // FRONT -> [10] -> [20] <- BACK
    enqueue(&q, 30);     // FRONT -> [10] -> [20] -> [30] <- BACK

    int a = peek(&q);   // a = 10, queue unchanged
    int b = peek(&q);   // b = 10, queue unchanged

    is_empty(&q);   // 0 (false)

    printf("Peeked values: %d, %d\n", a, b); // Output: Peeked values: 10, 10

    int d = dequeue(&q);    // a = 1, FRONT -> [20] -> [30] <- BACK
    int e = dequeue(&q);    // b = 2, FRONT -> [30] <- BACK
    int f = dequeue(&q);    // c = 3, FRONT -> NULL <- BACK

    printf("Dequeued values: %d, %d, %d\n", d, e, f); // Output: Dequeued values: 10, 20, 30
    return 0;
}
