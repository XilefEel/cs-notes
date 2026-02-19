#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
    struct Node *prev;  
} Node;


Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void print_list(Node *head) {
    Node *current = head;

    while (current != NULL) {
        printf("%d", current->data);

        if (current->next != NULL) {
            printf(" <-> ");
        }

        current = current->next;
    }

    printf("\n");
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}


void insert_at_head(Node **head, int data) {
    Node *new_node = create_node(data);
    new_node->prev = NULL;
    new_node->next = *head;

    if (*head != NULL) {
        (*head)->prev = new_node;
    }

    *head = new_node;
}

void insert_at_tail(Node **head, int data) {
    Node *new_node = create_node(data);

    new_node->next = NULL;

    if (*head == NULL) {
        new_node->prev = NULL;
        *head = new_node;
        return;
    }

    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;

    new_node->prev = current;
}

void insert_at_index(Node **head, int data, int index) {
    if (index == 0) {
        insert_at_head(head, data);
        return;
    }

    Node *current = *head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    Node *new_node = create_node(data);

    new_node->next = current->next;
    new_node->prev = current;

    if (current->next != NULL) {
        current->next->prev = new_node;
    }

    current->next = new_node;
}

int main() {
    Node *head = NULL;

    insert_at_head(&head, 10);  // HEAD <-> [10] <-> NULL
    insert_at_head(&head, 20);  // HEAD <-> [20] <-> [10] <-> NULL
    insert_at_head(&head, 30);  // HEAD <-> [30] <-> [20] <-> [10] <-> NULL

    print_list(head);

    insert_at_tail(&head, 40);  // HEAD <-> [30] <-> [20] <-> [10] <-> [40] <-> NULL
    print_list(head);

    insert_at_index(&head, 50, 2);  // HEAD <-> [30] <-> [20] <-> [50] <-> [10] <-> [40] <-> NULL
    print_list(head);

    free_list(head);
    return 0;
}
