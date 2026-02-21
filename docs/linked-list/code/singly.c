#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;


Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

Node *get_node(Node *head, int index) {
    Node *current = head;
    int i = 0;

    while (current != NULL) {
        if (i == index) {
            return current;
        }

        current = current->next;
        i++;
    }

    return NULL;
}

void print_list(Node *head) {
    Node *current = head;

    while (current != NULL) {
        printf("%d", current->data);

        if (current->next != NULL) {
            printf(" -> ");
        }

        current = current->next;
    }

    printf("\n");
}

void traverse_apply(Node *head, void (*func)(int *)) {
    Node *current = head;

    while (current != NULL) {
        func(&current->data);

        current = current->next;
    }
}

void double_value(int *data) {
    *data *= 2;
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
    new_node->next = *head;
    *head = new_node;
}

void insert_at_tail(Node **head, int data) {
    Node *new_node = create_node(data);

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;
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
    current->next = new_node;
}


void delete_at_head(Node **head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node *temp = *head;
    *head = (*head)->next;

    free(temp);
}

void delete_at_tail(Node **head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    Node *current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    free(current->next);

    current->next = NULL;
}

void delete_at_index(Node **head, int index) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    if (index == 0) {
        delete_at_head(head);
        return;
    }

    Node *current = *head;
    for (int i = 0; i < index - 1 && current->next != NULL; i++) {
        current = current->next;
    }

    if (current->next == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    Node *temp = current->next;
    current->next = temp->next;

    free(temp);
}


Node *reverse_list(Node *head) {
    Node *prev = NULL;
    Node *curr = head;
    Node *next = NULL;

    // Traverse and reverse each pointer
    while (curr != NULL) {
        // Save the next node
        next = curr->next;

        // Reverse the current node's pointer
        curr->next = prev;

        // Move prev and curr one step forward
        prev = curr;
        curr = next;
    }

    // prev is now the new head
    return prev;
}

int has_cycle(Node *head) {
    if (head == NULL || head->next == NULL) {
        return 0;
    }

    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return 1;
        }
    }

    return 0;
}

int main() {
    Node *head = NULL;

    insert_at_head(&head, 10); // HEAD -> [10] -> NULL
    insert_at_head(&head, 20); // HEAD -> [20] -> [10] -> NULL
    insert_at_head(&head, 30); // HEAD -> [30] -> [20] -> [10] -> NULL

    print_list(head);

    insert_at_tail(&head, 40); // HEAD -> [30] -> [20] -> [10] -> [40] -> NULL
    print_list(head);

    insert_at_index(&head, 50, 2); // HEAD -> [30] -> [20] -> [50] -> [10] -> [40] -> NULL
    print_list(head);

    delete_at_head(&head); // HEAD -> [50] -> [20] -> [10] -> [40] -> NULL
    print_list(head);

    delete_at_tail(&head); // HEAD -> [50] -> [20] -> [10] -> NULL
    print_list(head);

    delete_at_index(&head, 1); // HEAD -> [50] -> [10] -> NULL
    print_list(head);

    head = reverse_list(head); // HEAD -> [10] -> [50] -> NULL
    print_list(head);

    free_list(head);

    insert_at_tail(&head, 1);
    insert_at_tail(&head, 2);
    insert_at_tail(&head, 3);
    insert_at_tail(&head, 4);

    Node *second = head->next;
    Node *last = head;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = second; 

    if (has_cycle(head)) {
        printf("Cycle detected!\n"); 
    } else {
        printf("No cycle\n");
    }

    return 0;
}
