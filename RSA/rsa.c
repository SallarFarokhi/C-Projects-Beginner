#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "randstate.h"
#include "numtheory.h"

// creates a public key in p, q, n, and e
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    /* These have to go in the main()*/
    randstate_init(time(NULL));
    srand(time(NULL));

    mpz_t totient;
    mpz_t bits;
    mpz_t p_sub;
    mpz_t q_sub;
    mpz_init(p_sub);
    mpz_init(q_sub);
    mpz_init_set_ui(bits, nbits);
    mpz_init(totient);

    // number of bits for p and q
    uint64_t p_bits = rand() % (nbits / 2) + (nbits / 4);
    uint64_t q_bits = nbits - p_bits;

    // assigning primes to p and q
    make_prime(p, p_bits, iters);
    make_prime(q, q_bits, iters);

    // finding (p-1)(q-1)
    mpz_sub_ui(p_sub, p, 1);
    mpz_sub_ui(q_sub, q, 1);
    mpz_mul(totient, p_sub, q_sub);
    mpz_mul(n, p, q);

    // q_sub stores the random number, 0 - nbits--
    // p_sub stores the gcd between the totient and q_sub
    // stop when p_sub is equal to 1
    do {
        mpz_urandomb(q_sub, state, nbits);
        gcd(p_sub, q_sub, totient);
    } while (mpz_cmp_ui(p_sub, 1) != 0);
    mpz_set(e, q_sub);

    // clear the temp variables
    mpz_clear(p_sub);
    mpz_clear(q_sub);
    mpz_clear(totient);
    return;
}

// prints out the public key to pbfile
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {

    fprintf(pbfile, "%s\n", mpz_get_str(NULL, 16, n));
    fprintf(pbfile, "%s\n", mpz_get_str(NULL, 16, e));
    fprintf(pbfile, "%s\n", mpz_get_str(NULL, 16, s));
    fprintf(pbfile, "%s\n", username);
    return;
}

// reads in the public key from pbfile
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    char text[4096];
    fscanf(pbfile, "%s\n", text);
    mpz_set_str(n, text, 16);
    fscanf(pbfile, "%s\n", text);
    mpz_set_str(e, text, 16);
    fscanf(pbfile, "%s\n", text);
    mpz_set_str(s, text, 16);
    fscanf(pbfile, "%s\n", username);
    return;
}

// makes a private key
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    // temp variables
    mpz_t p_sub;
    mpz_t q_sub;
    mpz_init(p_sub);
    mpz_init(q_sub);

    // p_sub = totient of p and q
    mpz_sub_ui(p_sub, p, 1);
    mpz_sub_ui(q_sub, q, 1);
    mpz_mul(p_sub, p_sub, q_sub);

    // d = inverse of e, modulus p_sub
    mod_inverse(d, e, p_sub);

    // free the temp variables
    mpz_clear(p_sub);
    mpz_clear(q_sub);
    return;
}

// writes out the private key to pvfile
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    fprintf(pvfile, "%s\n", mpz_get_str(NULL, 16, n));
    fprintf(pvfile, "%s\n", mpz_get_str(NULL, 16, d));
    return;
}

// reads the private key from pvfile
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {

    char text[4096];
    fscanf(pvfile, "%s\n", text);
    mpz_set_str(n, text, 16);
    fscanf(pvfile, "%s\n", text);
    mpz_set_str(d, text, 16);
    return;
}

// encrypts the ciphertext c, c = m^e mod n
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}

// encrypts the contents of infile and writes to outfile
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {

    mpz_t m;
    mpz_init(m);
    mpz_t m_encrypt;
    mpz_init(m_encrypt);

    // k_size = block size
    mpz_t k;
    uint64_t n_int = mpz_get_ui(n);
    mpz_init_set_ui(k, log2(n_int));
    mpz_sub_ui(k, k, 1);
    mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_size = mpz_get_ui(k);

    // allocate an array of int pointers, k_size
    uint8_t *block = (uint8_t *) calloc(k_size, sizeof(uint8_t));
    // prepend the first byte
    block[0] = 0xff;

    uint64_t j = 0;

    // while the file's not done
    while (!feof(infile)) {
        // read in (k_size-1) bytes from infile, and place them into block+1
        j = fread(block + 1, sizeof(uint8_t), k_size - 1, infile);

        // import  j (k_size) bytes from block into mpz_t m
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);
        // encrypt the contents of m into m_encrypt
        rsa_encrypt(m_encrypt, m, e, n);

        // print m_encrypt to outfile as a hexstring
        fprintf(outfile, "%s\n", mpz_get_str(NULL, 16, m_encrypt));
    }

    // clear the space
    mpz_clear(m);
    mpz_clear(m_encrypt);
    mpz_clear(k);
    free(block);
    return;
}

// decrypts c and puts it in m
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}

// decrypts infile and writes to outfile
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {

    mpz_t c;
    mpz_t c_decrypt;
    mpz_init(c);
    mpz_init(c_decrypt);

    // k_size = block size
    mpz_t k;
    uint64_t n_int = mpz_get_ui(n);
    mpz_init_set_ui(k, log2(n_int));
    mpz_sub_ui(k, k, 1);
    mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_size = mpz_get_ui(k);

    // allocate an array of int pointers, k_size
    uint8_t *block = (uint8_t *) calloc(k_size, sizeof(uint8_t));

    uint64_t j = 0;

    while (!feof(infile)) {

        // scan a hexstring from infile into c
        gmp_fscanf(infile, "%Zx\n", c);
        // decrypt the contents of c into c_decrypt
        rsa_decrypt(c_decrypt, c, d, n);

        // export c into block, setting j
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, c_decrypt);

        // write out the fixed block to outfile
        fwrite(block + 1, sizeof(block[0]), j - 1, outfile);
    }

    mpz_clear(c);
    mpz_clear(c_decrypt);
    mpz_clear(k);
    free(block);
    return;
}

// creates an RSA signature
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}

// checks if the signature is correct
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t temp;
    mpz_init(temp);

    pow_mod(temp, s, e, n);
    if (mpz_cmp(temp, m) != 0) {
        mpz_clear(temp);
        return false;
    }
    mpz_clear(temp);
    return true;
}
