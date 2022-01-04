#include "ht.h"
#include "node.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// contains the number of lookups
uint64_t lookups = 0;

// struct
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// constructor
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) calloc(1, sizeof(HashTable));
    // if the allocation worked
    if (ht) {
        // salt values from salts.h
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;

        // array of node pointers
        ht->trees = (Node **) calloc(size, sizeof(Node *));

        // if the allocation worked
        if (ht->trees) {
            // setting each node to null
            for (uint32_t i = 0; i < size; i++) {
                ht->trees[i] = NULL;
                return ht;
            }
        }
    }
    return NULL;
}

// destructor
void ht_delete(HashTable **ht) {

    // frees the node array
    if ((*ht)->trees) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            bst_delete(&((*ht)->trees[i]));
            (*ht)->trees[i] = NULL;
        }
        free((*ht)->trees);
        (*ht)->trees = NULL;
    }

    // frees the hash table
    if (*ht) {
        free(*ht);
        *ht = NULL;
    }
    return;
}

// returns the size of the hash table
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// returns the node of a certain word in the hash table
// will return NULL if the node does not exist
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    // incriment the total number of lookups
    lookups += 1;

    // index at which the bst exists
    uint32_t hash_index = hash(ht->salt, oldspeak) % ht_size(ht);

    // looks for and returns the node
    return bst_find(ht->trees[hash_index], oldspeak);
}

// returns the number of existing BSTs in the hash table
uint32_t ht_count(HashTable *ht) {

    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            count += 1;
        }
    }
    return count;
}

// inserts a node into the hash table
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    // incrimenting the number of lookups
    lookups += 1;
    // index at which bst exists
    uint32_t hash_index = hash(ht->salt, oldspeak) % ht_size(ht);
    // changes the bst at the root
    ht->trees[hash_index] = bst_insert(ht->trees[hash_index], oldspeak, newspeak);
    // printf("HT: %s -> %s\n", oldspeak, newspeak);
    return;
}

// returns the average BST size in the hash table
double ht_avg_bst_size(HashTable *ht) {

    // keeps count
    uint32_t total_nodes = 0;
    uint32_t total_trees = 0;

    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            // get the total number of nodes
            total_nodes += bst_size(ht->trees[i]);
            total_trees += 1;
        }
    }

    // get the average
    double avg_size = (double) total_nodes / total_trees;
    return avg_size;
}

// returns the average BST size in the hash table
double ht_avg_bst_height(HashTable *ht) {

    // keeps count
    uint32_t total_height = 0;
    uint32_t total_trees = 0;

    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            // get the total number of nodes
            total_height += bst_height(ht->trees[i]);
            total_trees += 1;
        }
    }

    // get the average
    double avg_size = (double) total_height / total_trees;
    return avg_size;
}

// prints the hash table
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        // prints the node at the index
        node_print(ht->trees[i]);
    }
    return;
}
