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

Node *merge_sorted(Node *list1, Node *list2) {
    Node dummy;
    dummy.next = NULL;
    Node *tail = &dummy;

    while (list1 != NULL && list2 != NULL) {
        if (list1->data <= list2->data) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    if (list1 != NULL) {
        tail->next = list1;
    } else {
        tail->next = list2;
    }

    return dummy.next;
}

Node *remove_nth_from_end(Node *head, int n) {
    Node dummy;
    dummy.next = head;

    Node *fast = &dummy;
    Node *slow = &dummy;

    for (int i = 0; i < n; i++) {
        fast = fast->next;
    }

    while (fast->next != NULL) {
        slow = slow->next;
        fast = fast->next;
    }

    Node *temp = slow->next;
    slow->next = slow->next->next;
    free(temp);

    return dummy.next;
}

int is_palindrome(Node *head) {
    if (head == NULL || head->next == NULL) {
        return 1;
    }

    // Step 1: Find the middle
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Step 2: Reverse the second half
    Node *second = reverse_list(slow);
    Node *copy = second;  // Keep a copy to restore later

    // Step 3: Compare both halves
    Node *left = head;
    Node *right = second;

    int result = 1;
    while (right != NULL) {
        if (left->data != right->data) {
            result = 0;
            break;
        }
        left = left->next;
        right = right->next;
    }

    // Restore the list
    reverse_list(copy);

    return result;
}

typedef enum {
    TOP = 0,
    HIGH = 1,
    NORMAL = 2,
    LOW = 3,
} Priority;

typedef struct PriorityNode {
    int data;
    Priority priority;
    struct PriorityNode *next;
} PriorityNode;

PriorityNode *create_priority_node(int data, Priority priority) {
    PriorityNode *node = (PriorityNode *)malloc(sizeof(PriorityNode));
    node->data = data;
    node->priority = priority;
    node->next = NULL;
    return node;
}

void enqueue(PriorityNode **head, int data, Priority priority) {
    PriorityNode *node = create_priority_node(data, priority);

    if (*head == NULL || node->priority < (*head)->priority) {
        node->next = *head;
        *head = node;
        return;
    }

    PriorityNode *current = *head;
    while (current->next != NULL && current->next->priority <= priority) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
}

int dequeue(PriorityNode **head) {
    if (*head == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    PriorityNode *temp = *head;
    int data = temp->data;
    *head = (*head)->next;
    free(temp);
    return data;
}

int main() {
    Node *head = NULL;

    insert_at_head(&head, 10);  // HEAD -> [10] -> NULL
    insert_at_head(&head, 20);  // HEAD -> [20] -> [10] -> NULL
    insert_at_head(&head, 30);  // HEAD -> [30] -> [20] -> [10] -> NULL

    print_list(head);

    insert_at_tail(&head, 40);  // HEAD -> [30] -> [20] -> [10] -> [40] -> NULL
    print_list(head);

    insert_at_index(&head, 50, 2);  // HEAD -> [30] -> [20] -> [50] -> [10] -> [40] -> NULL
    print_list(head);

    delete_at_head(&head);  // HEAD -> [50] -> [20] -> [10] -> [40] -> NULL
    print_list(head);

    delete_at_tail(&head);  // HEAD -> [50] -> [20] -> [10] -> NULL
    print_list(head);

    delete_at_index(&head, 1);  // HEAD -> [50] -> [10] -> NULL
    print_list(head);

    head = reverse_list(head);  // HEAD -> [10] -> [50] -> NULL
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

    Node *list1 = NULL;
    insert_at_tail(&list1, 1);
    insert_at_tail(&list1, 3);
    insert_at_tail(&list1, 5);

    Node *list2 = NULL;
    insert_at_tail(&list2, 2);
    insert_at_tail(&list2, 4);
    insert_at_tail(&list2, 6);

    Node *merged = merge_sorted(list1, list2);
    print_list(merged); // HEAD -> [1] -> [2] -> [3] -> [4] -> [5] -> [6] -> NONE

    Node *head_2 = NULL;
    insert_at_tail(&head_2, 1);
    insert_at_tail(&head_2, 2);
    insert_at_tail(&head_2, 3);
    insert_at_tail(&head_2, 4);
    insert_at_tail(&head_2, 5);

    head_2 = remove_nth_from_end(head_2, 2);
    print_list(head_2);   // HEAD -> 1 -> 2 -> 3 -> 5 -> NULL

    Node *head_3 = NULL;
    insert_at_tail(&head_3, 1);
    insert_at_tail(&head_3, 2);
    insert_at_tail(&head_3, 3);
    insert_at_tail(&head_3, 2);
    insert_at_tail(&head_3, 1);

    if (is_palindrome(head_3)) {
        printf("Palindrome!\n");    // Prints this
    } else {
        printf("Not a palindrome\n");
    }

    PriorityNode *priority_queue = NULL;
    enqueue(&priority_queue, 5, NORMAL);
    enqueue(&priority_queue, 3, HIGH);
    enqueue(&priority_queue, 8, LOW);
    enqueue(&priority_queue, 1, NORMAL);
    enqueue(&priority_queue, 4, HIGH);
    enqueue(&priority_queue, 7, LOW);
    enqueue(&priority_queue, 2, TOP);

    // [2:TOP] -> [3:HIGH] -> [4:HIGH] -> [5:NORMAL] -> [1:NORMAL] -> [8:LOW] -> [7:LOW]

    printf("%d\n", dequeue(&priority_queue));  // 2 (TOP)
    printf("%d\n", dequeue(&priority_queue));  // 3 (HIGH)
    printf("%d\n", dequeue(&priority_queue));  // 4 (HIGH)

    return 0;
}
