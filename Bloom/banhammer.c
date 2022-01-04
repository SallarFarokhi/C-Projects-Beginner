#include "bst.h"
#include "bf.h"
#include "ht.h"
#include "bv.h"
#include "node.h"
#include "parser.h"
#include "speck.h"
#include "messages.h"

#include <unistd.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <math.h>

// cm args
#define OPTIONS "ht:f:s"

// regex word
#define WORD "[a-zA-Z0-9_]+'([a-zA-Z0-9_])*|[a-zA-Z0-9_]+-([a-zA-Z0-9_])*|[a-zA-Z0-9_]+"

// NOTE: Certain parts of this program, the pseudocode and
// code, were provided by Professor Long and the staff of CSE-13S

// bst_print, but also prints the newspeak
void full_bst_print(Node *root) {
    if (root) {
        full_bst_print(root->left);
        printf("%s -> %s\n", root->oldspeak, root->newspeak);
        full_bst_print(root->right);
    }
    return;
}

uint64_t lookups;
uint64_t branches;

// this main function will take in command line arguments,
// read in from two text files, and use a bloom filter to
// flag any words that were in the bloom filter, while
// possibly returning any alternatives
int main(int argc, char **argv) {

    int opt = 0;

    // default values
    uint32_t hash_size = 65536; // 2 ^ 16
    uint32_t bloom_size = 1048576; // 2 ^ 20
    bool help = false;
    bool stats = false;

    // checks for each command argument
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        // sets the user inputs and bool values
        case 't': hash_size = strtoul(optarg, NULL, 10);
        case 'f': bloom_size = strtoul(optarg, NULL, 10);
        case 's': stats = true; break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }

    // prints the help menu and exits
    if (help) {
        printf("SYNOPSIS\n  A word filtering program for the GPRSC.\n");
        printf("  Filters out and reports bad words parsed from stdin.\n");
        printf("\nUSAGE\n  ./banhammer [-hs] [-t size ] [-f size]\n\n");
        printf("OPTIONS\n");
        printf("  -h            Program usage and help.\n");
        printf("  -s            Print program statistics.\n");
        printf("  -t size       Specify hash table size (default: 2^16).\n");
        printf("  -f size       Specify Bloom filter size (default: 2^20).\n");
        return 0;
    }

    // initialize the bloom filter and hash table
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size);

    // open the text files
    FILE *badspeak = fopen("badspeak.txt", "rw");
    FILE *newspeak = fopen("newspeak.txt", "rw");

    // temp address
    char badword[100] = { 0x0 };
    char oldword[100] = { 0x0 };
    char newword[100] = { 0x0 };

    // read through the text file
    // put each bad word in the bloom filter and hash table
    while (fscanf(badspeak, "%s\n", badword) != EOF) {
        ht_insert(ht, badword, NULL);
        bf_insert(bf, badword);
    }

    // read through the text file
    // put each old word in the bloom filter
    // put each old and new word in the hash table
    while (fscanf(newspeak, "%s %s\n", oldword, newword) != EOF) {
        bf_insert(bf, oldword);
        ht_insert(ht, oldword, newword);
    }

    // binary search trees to keep track of flagged words
    Node *badtree = bst_create();
    Node *oldtree = bst_create();

    // reads in from stdin, and checks the words with
    // the bloom filter
    // provided by Professor Long
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        bf_delete(&bf);
        ht_delete(&ht);
        bst_delete(&badtree);
        bst_delete(&oldtree);
        fclose(badspeak);
        fclose(newspeak);
        return 1;
    }
    char *word = NULL;
    // work through each word
    while ((word = next_word(stdin, &re)) != NULL) {
        // if the word is in the filter
        if (bf_probe(bf, word)) {
            Node *test = ht_lookup(ht, word);
            // if the word is in the hash table
            if (test) {
                // if the word has a newspeak
                if (test->newspeak) {
                    // put it in the old tree
                    if (bst_find(oldtree, test->oldspeak) == NULL) {
                        oldtree = bst_insert(oldtree, test->oldspeak, test->newspeak);
                    }
                } else {
                    // put it in the bad tree
                    if (bst_find(badtree, test->oldspeak) == NULL) {
                        badtree = bst_insert(badtree, test->oldspeak, test->newspeak);
                    }
                }
            }
        }
    }

    // prints the statistics, and ends the program
    if (stats) {
        printf("Average BST size: %0.6f\n", ht_avg_bst_size(ht));

        printf("Average BST height: %0.6f\n", ht_avg_bst_height(ht));

        double d_branches = branches;
        double d_lookups = lookups;
        double branches_traversed = d_branches / d_lookups;
        printf("Average branches traversed: %0.6f\n", branches_traversed);

        double d_ht_count = ht_count(ht);
        double ht_load = 100 * d_ht_count / ht_size(ht);
        printf("Hash table load: %0.6f%%\n", ht_load);

        double d_bf_count = bf_count(bf);
        double bf_load = 100 * d_bf_count / bf_size(bf);
        printf("Bloom filter load: %0.6f%%\n", bf_load);

        // end of program
        fclose(badspeak);
        fclose(newspeak);
        bf_delete(&bf);
        ht_delete(&ht);
        bst_delete(&badtree);
        bst_delete(&oldtree);
        clear_words();
        regfree(&re);
        return 0;
    }

    // state of the citizen:
    // good, bad, or mixed
    bool g_message = (oldtree) ? true : false;
    bool b_message = (badtree) ? true : false;
    bool m_message = g_message && b_message;

    // prints the message, the words, and turns
    // off the flags, ending the program
    if (m_message) {
        printf("%s", mixspeak_message);
        bst_print(badtree);
        full_bst_print(oldtree);

        g_message = b_message = false;
    }

    // prints the message, the words, and turns
    // off the flags, ending the program
    if (b_message) {
        printf("%s", badspeak_message);
        bst_print(badtree);

        g_message = false;
    }

    // prints the message, the words, and turns
    // off the flags, ending the program
    if (g_message) {
        printf("%s", goodspeak_message);
        full_bst_print(oldtree);
    }

    // end of program
    fclose(badspeak);
    fclose(newspeak);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&badtree);
    bst_delete(&oldtree);
    clear_words();
    regfree(&re);
    return 0;
}
