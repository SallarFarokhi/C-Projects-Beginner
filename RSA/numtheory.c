#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>

#include "randstate.h"

// Note: the pseudoscience for all of these functions
// was provided by Professor Long and the staff of
// CSE-13S

// finds the greatest common divisor between a and b,
// and places it in g
void gcd(mpz_t g, mpz_t a, mpz_t b) {
    // temp variables
    mpz_t temp;
    mpz_init(temp);
    mpz_t temp_a;
    mpz_init(temp_a);
    mpz_t temp_b;
    mpz_init(temp_b);

    // set the variables to a and b
    mpz_set(temp_a, a);
    mpz_set(temp_b, b);

    // while b is not 0
    while (mpz_cmp_ui(temp_b, 0) != 0) {

        // temp = b
        mpz_set(temp, temp_b);

        // b = a mod b
        mpz_set(temp_b, temp_a);
        mpz_mod(temp_b, temp_b, temp);

        // a = temp
        mpz_set(temp_a, temp);
    }
    // put a in g
    mpz_set(g, temp_a);

    mpz_clear(temp);
    mpz_clear(temp_a);
    mpz_clear(temp_b);
    return;
}

// raise 'a' to the 'd' power, mod 'n', and return the value in 'o'
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    // temp variables
    mpz_t temp_o;
    mpz_init(temp_o);
    mpz_t temp_a;
    mpz_init(temp_a);
    mpz_t temp_d;
    mpz_init(temp_d);
    mpz_t temp_n;
    mpz_init(temp_n);

    // setting the temp variables
    mpz_set_ui(temp_o, 1);
    mpz_set(temp_a, a);
    mpz_set(temp_d, d);
    mpz_set(temp_n, n);

    // while d > 0
    while (mpz_cmp_ui(temp_d, 0)) {
        // if d is odd
        if (mpz_odd_p(temp_d)) {
            // o = o * a mod(n)
            mpz_mul(temp_o, temp_o, temp_a);
            mpz_mod(temp_o, temp_o, temp_n);
        }
        // a = a*a mod(n)
        mpz_mul(temp_a, temp_a, temp_a);
        mpz_mod(temp_a, temp_a, temp_n);
        // d = d / 2
        mpz_fdiv_q_ui(temp_d, temp_d, 2);
    }
    // set o to temp_o
    mpz_set(o, temp_o);

    // free the temp variables
    mpz_clear(temp_o);
    mpz_clear(temp_a);
    mpz_clear(temp_d);
    mpz_clear(temp_n);
    return;
}

// stores the inverse of a mod n in i
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    // temp variables
    mpz_t r;
    mpz_init(r);
    mpz_t r_prime;
    mpz_init(r_prime);
    mpz_t t;
    mpz_init(t);
    mpz_t t_prime;
    mpz_init(t_prime);
    mpz_t q;
    mpz_init(q);
    mpz_t temp;
    mpz_init(temp);

    // setting the variables
    mpz_set(r, n);
    mpz_set(r_prime, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t_prime, 1);

    // while r_prime is not zero
    while (mpz_cmp_ui(r_prime, 0) != 0) {

        mpz_fdiv_q(q, r, r_prime);
        mpz_mul(temp, r_prime, q);
        mpz_sub(temp, r, temp);
        mpz_set(r, r_prime);
        mpz_set(r_prime, temp);

        mpz_mul(temp, t_prime, q);
        mpz_sub(temp, t, temp);
        mpz_set(t, t_prime);
        mpz_set(t_prime, temp);
    }
    // if r is greater than 1,
    // there is no inverse
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set(i, 0);
        // frees the temps
        mpz_clear(r);
        mpz_clear(r_prime);
        mpz_clear(t);
        mpz_clear(t_prime);
        mpz_clear(q);
        mpz_clear(temp);
        return;
    }
    // if t is less than zero,
    // add n to t
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }

    // puts t in i
    mpz_set(i, t);

    // frees the temps
    mpz_clear(r);
    mpz_clear(r_prime);
    mpz_clear(t);
    mpz_clear(t_prime);
    mpz_clear(q);
    mpz_clear(temp);
    return;
}

// tests if the number n is prime
bool is_prime(mpz_t n, uint64_t iters) {

    // test case for an even number
    if (mpz_even_p(n)) {
        return false;
    }

    // temp variables
    mpz_t s;
    mpz_t s_sub;
    mpz_t n_sub;
    mpz_t r;
    mpz_t a;
    mpz_t y;
    mpz_t j;
    mpz_t test;
    mpz_t test2;
    mpz_t number2;
    mpz_init(s);
    mpz_init(r);
    mpz_init(a);
    mpz_init(y);
    mpz_init(j);
    mpz_init(test);
    mpz_init(test2);
    mpz_init_set_ui(number2, 2);
    mpz_init(n_sub);
    mpz_init(s_sub);

    /* Gets s and r */

    // (2^s)r = n-1
    mpz_sub_ui(test, n, 1);
    while (mpz_even_p(test)) {
        mpz_fdiv_q_ui(test, test, 2);
        mpz_add_ui(s, s, 1);
    }

    mpz_sub_ui(test, n, 1);
    mpz_set_ui(test2, 1);
    mpz_set_ui(r, 0);

    for (uint64_t i = 0; mpz_cmp_ui(s, i) != 0; i++) {
        mpz_mul_ui(test2, test2, 2);
    }

    mpz_fdiv_q(r, test, test2);

    /* Gets n_sub and s_sub */

    // s_sub = s-1
    // n_sub = n-1
    mpz_sub_ui(s_sub, s, 1);
    mpz_sub_ui(n_sub, n, 1);

    /* Loops from 1 to k */

    for (uint64_t i = 1; i <= iters; i++) {

        /* Gets a random number between 2 and n-2 */

        mpz_sub_ui(test, n, 3);
        mpz_urandomm(test2, state, test);
        mpz_add_ui(a, test2, 2);

        pow_mod(y, a, r, n);

        // test  = n-1
        // test2 = s-1
        if (mpz_cmp_d(y, 1) != 0 && mpz_cmp(y, n_sub) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s_sub) <= 0 && mpz_cmp(y, n_sub) != 0) {
                pow_mod(y, y, number2, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    //clearing the temp variables and returning false
                    mpz_clear(s);
                    mpz_clear(r);
                    mpz_clear(a);
                    mpz_clear(y);
                    mpz_clear(j);
                    mpz_clear(test);
                    mpz_clear(test2);
                    mpz_clear(number2);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_sub) != 0) {
                // clearing the temp variables and returning false
                mpz_clear(s);
                mpz_clear(r);
                mpz_clear(a);
                mpz_clear(y);
                mpz_clear(j);
                mpz_clear(test);
                mpz_clear(test2);
                mpz_clear(number2);
                return false;
            }
        }
    }
    // return true and end program
    mpz_clear(s);
    mpz_clear(r);
    mpz_clear(a);
    mpz_clear(y);
    mpz_clear(j);
    mpz_clear(test);
    mpz_clear(test2);
    mpz_clear(number2);
    return true;
}

// makes a prime number p of bit bits
void make_prime(mpz_t p, uint64_t bit, uint64_t iters) {
    bool prime;
    mpz_t temp;
    mpz_init_set_ui(temp, 1);
    //randstate_init(time(NULL));
    while (true) {
        // random number between 0 and bit-1
        // NEED TO ADD to random number to get
        // AT LEAST n bits, p += 2^n

        // p = random number, reset the temp
        mpz_set_ui(p, gmp_urandomb_ui(state, bit));
        mpz_set_ui(temp, 1);
        for (uint64_t i = 0; i < bit; i++) {
            mpz_mul_ui(temp, temp, 2);
        }

        // add the value of temp to p to get
        // AT LEAST n bits random number
        mpz_add(p, p, temp);

        //gmp_printf("Value of p: %Zd\n", p);
        prime = is_prime(p, iters);
        if (prime) {
            return;
        }
    }
}
