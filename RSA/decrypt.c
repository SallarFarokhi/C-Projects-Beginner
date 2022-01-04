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
#define OPTIONS "i:o:n:vh"

// this main takes in command line arguments giving
// specifications, and decrypts a file using a private
// key, which is then written out to a file
int main(int argc, char **argv) {

    int opt = 0;

    // default values
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pvname = "rsa.priv";
    bool help, verbose;

    // checks for each command argument
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'i': {
            infile = fopen(optarg, "r");
            break;
        }
        case 'o': {
            outfile = fopen(optarg, "w");
            break;
        }
        case 'n': pvname = optarg; break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }

    // prints the help message
    if (help) {
        printf("SYNOPSIS\n  Encrypts data using RSA encryption.\nEncrypted data is decrypted by "
               "the decrypt program.\n\n");
        printf("USAGE\n  ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n\n");
        printf("OPTIONS\n");
        printf("  -h             Display program help and usage.\n");
        printf("  -v             Display verbose program output.\n");
        printf("  -i infile      Input file of data to encrypt (default: stdin)\n");
        printf("  -o outfile     Output file for encrypted data (default: stdout)\n");
        printf("  -n pbfile      Public key file (default: rsa.pub).\n");
        fclose(outfile);
        fclose(infile);
        randstate_clear();
        return 0;
    }

    // initialize and open the files
    FILE *pvfile = fopen(pvname, "r");
    if (pvfile == NULL || infile == NULL || outfile == NULL) {
        printf("The files specified could not be accessed or created.\n");
        fclose(pvfile);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    // mpz variables
    mpz_t n;
    mpz_t d;
    mpz_init(n);
    mpz_init(d);

    // read in the private key from pvfile
    rsa_read_priv(n, d, pvfile);

    // prints out the data
    if (verbose) {
        printf("n (%zu bits) = %s\n", mpz_sizeinbase(n, 2), mpz_get_str(NULL, 10, n));
        printf("d (%zu bits) = %s\n", mpz_sizeinbase(d, 2), mpz_get_str(NULL, 10, d));
    }

    // decrypts the contents of infile into outfile
    rsa_decrypt_file(infile, outfile, n, d);

    // clearing space and closing files
    //mpz_clears(n, e, s, user);
    mpz_clear(n);
    mpz_clear(d);
    fclose(pvfile);
    fclose(infile);
    fclose(outfile);

    return 0;
}
