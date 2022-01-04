#include "salts.h"
#include "bv.h"
#include "bf.h"
#include "speck.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// struct
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// constructor
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) calloc(1, sizeof(BloomFilter));
    if (bf) {
        // sets the salts
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;

        // sets the bitvector and returns the filter
        bf->filter = bv_create(size);
        if (bf->filter) {
            return bf;
        }
    }
    // constructor did not work
    return NULL;
}

// destructor
void bf_delete(BloomFilter **bf) {

    // free the bit vector
    if ((*bf)->filter) {
        bv_delete(&((*bf)->filter));
    }

    // free the filter
    if (*bf) {
        free(*bf);
        *bf = NULL;
    }
    return;
}

// returns the length of the filter
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// inserts a word into the bloom filter
void bf_insert(BloomFilter *bf, char *oldspeak) {

    // sets the bits with the three salts
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bv_length(bf->filter));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bv_length(bf->filter));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bv_length(bf->filter));
    return;
}

// probes the bloom filter for a word
bool bf_probe(BloomFilter *bf, char *oldspeak) {

    // checks each of the bits in the bv with the three salts
    bool a = bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bv_length(bf->filter));
    bool b = bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bv_length(bf->filter));
    bool c = bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bv_length(bf->filter));

    // if all of the bits are true
    if (a && b && c) {
        return true;
    }

    // not in the filter
    return false;
}

// returns the number of set bits in the filter
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;

    // count the number of set bits in the filter
    for (uint32_t i = 0; i < bv_length(bf->filter); i++) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

// prints the bloom filter
void bf_print(BloomFilter *bf) {
    printf("Bloom Filter:\n");
    bv_print(bf->filter);
}
