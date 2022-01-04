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
// specification, and encrypts the contents of a
// file, which is written out to a different file
int main(int argc, char **argv) {

    int opt = 0;

    // default values
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pbname = "rsa.pub";
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
        case 'n': pbname = optarg; break;
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
    FILE *pbfile = fopen(pbname, "r");
    if (pbfile == NULL || infile == NULL || outfile == NULL) {
        printf("The files specified could not be accessed or created.\n");
        fclose(pbfile);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    // username, as a string
    char username[4096];

    // mpz numbers
    mpz_t n;
    mpz_t e;
    mpz_t s;
    mpz_t user;
    mpz_init(n);
    mpz_init(e);
    mpz_init(s);
    mpz_init(user);

    // fills the mpz values
    rsa_read_pub(n, e, s, username, pbfile);

    // prints out the data
    if (verbose) {
        printf("%s\n", username);
        printf("s (%zu bits) = %s\n", mpz_sizeinbase(s, 2), mpz_get_str(NULL, 10, s));
        printf("n (%zu bits) = %s\n", mpz_sizeinbase(n, 2), mpz_get_str(NULL, 10, n));
        printf("e (%zu bits) = %s\n", mpz_sizeinbase(e, 2), mpz_get_str(NULL, 10, e));
    }

    // turn the username into an mpz_t
    mpz_set_str(user, username, 62);

    // checks if the encryption is correct
    if (!rsa_verify(user, s, e, n)) {
        printf("The signature does not match the encryption. Sorry!\n");
        fclose(pbfile);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    // encrypt the contents of infile into outfile
    rsa_encrypt_file(infile, outfile, n, e);

    // clearing space and closing files
    //mpz_clears(n, e, s, user);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(s);
    mpz_clear(user);
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);

    return 0;
}
