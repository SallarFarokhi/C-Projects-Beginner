#include <stdint.h>
#include <gmp.h>
#include "randstate.h"

// state variable
gmp_randstate_t state;

// initializes the state variable
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

// clears the state variable
void randstate_clear(void) {
    gmp_randclear(state);
}
