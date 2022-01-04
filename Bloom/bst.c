#include "bst.h"
#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// keeps track of the number of branches
uint64_t branches = 0;

// constructor
Node *bst_create(void) {
    return NULL;
}

// destructor
void bst_delete(Node **root) {

    // if the node exists
    if (*root) {
        bst_delete(&((*root)->left));
        bst_delete(&((*root)->right));

        // delete the node
        node_delete(root);
    }
    return;
}

// credit to Professor Long
static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

// returns the height of the tree
// credit to Professor Long
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// returns the number of nodes
// derrived from bst_height
uint32_t bst_size(Node *root) {

    if (root == NULL) {
        return 0;
    }
    return 1 + bst_size(root->left) + bst_size(root->right);
}

// finds a node in a tree
// credit to Professor Long
Node *bst_find(Node *root, char *oldspeak) {
    branches += 1;
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}

// creates and insert a node into the tree
// credit to Professor Long
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    branches += 1;
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else {
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    return node_create(oldspeak, newspeak);
}

// prints the binary search tree
// credit to Professor Long
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        printf("%s\n", root->oldspeak);
        bst_print(root->right);
    }
    return;
}
