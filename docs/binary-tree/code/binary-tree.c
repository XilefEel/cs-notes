#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
} BST;

BST create_bst() {
    BST bst;
    bst.root = NULL;
    return bst;
}

int main() {
    BST bst = create_bst();
}
