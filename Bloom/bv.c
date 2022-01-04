#include "bv.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// provided by Professor long
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// constructor
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) calloc(1, sizeof(BitVector));

    // if the address exists
    if (bv) {
        // get the length of the array in bytes
        uint32_t array_length = (length % 8 > 0) ? (length / 8 + 1) : (length / 8);
        bv->vector = (uint8_t *) calloc(array_length, sizeof(uint8_t));
        bv->length = length;
    } else {
        bv->vector = NULL;
    }

    return bv;
}

// destructor
void bv_delete(BitVector **bv) {

    // free the array
    if ((*bv)->vector) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
    }

    // free the bit vector
    if (*bv) {
        free(*bv);
        *bv = NULL;
    }
    return;
}

// returns the length
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// sets the bit at index i
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << (i % 8));
    return true;
}

// clears the bit at index i
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << (i % 8));
    return true;
}

// returns the bit at index i
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i > bv->length) {
        return false;
    }
    int foo = bv->vector[i / 8] & (0x1 << (i % 8));
    return foo > 0;
}

// prints the vector
void bv_print(BitVector *bv) {
    int foo;
    for (uint32_t i = 0; i < bv->length; i++) {
        foo = (bv_get_bit(bv, i)) ? 1 : 0;
        printf("%d ", foo);
    }
    printf("\n");
    return;
}
