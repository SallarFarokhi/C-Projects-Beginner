#include "randstate.h"
#include "rsa.h"
#include "numtheory.h"

#include <stdint.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

// cm args
#define OPTIONS "b:i:n:d:s:vh"

// this main takes in command line arguments giving
// specifications, a public and private key are
// created, and written out to seperate files
int main(int argc, char **argv) {

    int opt = 0;

    // default values
    bool help, verbose;
    char *pbname = "rsa.pub";
    char *pvname = "rsa.priv";
    randstate_init(time(NULL));
    uint64_t iterations = 50;
    uint64_t bits = 256;

    // checks for each command argument
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'b': bits = strtoul(optarg, NULL, 10); break;
        case 'i': iterations = strtoul(optarg, NULL, 10); break;
        case 'n': pbname = optarg; break;
        case 'd': pvname = optarg; break;
        case 's': randstate_init(strtoul(optarg, NULL, 10));
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }

    // prints the help message
    if (help) {
        printf("SYNOPSIS\n  Generates an RSA public/private key pair.\n\n");
        printf("USAGE\n  ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n\n");
        printf("OPTIONS\n");
        printf("  -h             Display program help and usage.\n");
        printf("  -v             Display verbose program output.\n");
        printf("  -b             Minimum bits needed for public key n.\n");
        printf("  -n pbfile      Public key file (default: rsa.pub).\n");
        printf("  -d pvfile      Private key file (default: rsa.priv).\n");
        printf("  -s seed        Random seed for testing.\n");
        return 0;
    }

    // initialize and open the files
    FILE *pbfile = fopen(pbname, "w+");
    FILE *pvfile = fopen(pvname, "w+");
    if (pbfile == NULL || pvfile == NULL) {
        printf("The files specified could not be accessed or created.\n");
        fclose(pbfile);
        fclose(pvfile);
        randstate_clear();
        return 1;
    }

    // sets the perms of pvfile
    fchmod(fileno(pvfile), S_IRUSR | S_IWUSR);

    // mpz numbers
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t e;
    mpz_t d;
    mpz_t s;
    mpz_t user;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    mpz_init(s);
    mpz_init(user);

    // makes a public and private key
    rsa_make_pub(p, q, n, e, bits, iterations);
    rsa_make_priv(d, e, p, q);

    // get the username
    char *name = getenv("USER");
    mpz_set_str(user, name, 62);

    // rsa signing
    rsa_sign(s, user, d, n);

    // create a public and private key
    rsa_write_pub(n, e, s, name, pbfile);
    rsa_write_priv(n, d, pvfile);

    // prints out the data
    if (verbose) {
        printf("%s\n", name);
        printf("s (%zu bits) = %s\n", mpz_sizeinbase(s, 2), mpz_get_str(NULL, 10, s));
        printf("p (%zu bits) = %s\n", mpz_sizeinbase(p, 2), mpz_get_str(NULL, 10, p));
        printf("q (%zu bits) = %s\n", mpz_sizeinbase(q, 2), mpz_get_str(NULL, 10, q));
        printf("n (%zu bits) = %s\n", mpz_sizeinbase(n, 2), mpz_get_str(NULL, 10, n));
        printf("e (%zu bits) = %s\n", mpz_sizeinbase(e, 2), mpz_get_str(NULL, 10, e));
        printf("d (%zu bits) = %s\n", mpz_sizeinbase(d, 2), mpz_get_str(NULL, 10, d));
    }

    // clearing space and closing files
    mpz_clears(p, q, n, e, d, s, user);
    randstate_clear();
    fclose(pbfile);
    fclose(pvfile);

    return 0;
}
