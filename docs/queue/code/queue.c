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

int main() {
    Queue q = create_queue();
    enqueue(&q, 10);    // FRONT -> [10] <- BACK
    enqueue(&q, 20);    // FRONT -> [10] -> [20] <- BACK
    enqueue(&q, 30);    // FRONT -> [10] -> [20] -> [30] <- BACK
    return 0;
}
