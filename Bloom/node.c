#include "node.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// constructor
Node *node_create(char *oldspeak, char *newspeak) {
    // if oldspeak is wrong
    if (!oldspeak) {
        return NULL;
    }
    // if the address exists
    Node *n = (Node *) calloc(1, sizeof(Node));
    if (n) {
        n->oldspeak = strdup(oldspeak);
        // if there is a newspeak
        if (newspeak) {
            n->newspeak = strdup(newspeak);
        } else {
            n->newspeak = NULL;
        }
        n->left = NULL;
        n->right = NULL;
    }
    return n;
}

// destructor
void node_delete(Node **n) {
    // if the address exists
    if (*n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
    return;
}

// prints the node
void node_print(Node *n) {
    if (!n) {
        return;
    }
    printf("Badspeak: %s\n", n->oldspeak);
    if (n->newspeak != NULL) {
        printf("Newspeak: %s\n", n->newspeak);
    } else {
        printf("No newspeak available\n");
    }

    if (n->left == NULL && n->right == NULL) {
        printf("Leaf node\n");
    }
    return;
}
