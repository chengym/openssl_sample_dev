/*
 * * dsa.cc
 * * - Show the usage of DSA sign/verify
 * */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <openssl/dsa.h>
 
int main(int argc, char** argv)
{
    DSA* dsa;
    unsigned char* input_string;
    unsigned char* sign_string;
    unsigned int sig_len;
    unsigned int i;
    int ret;
    int is_valid_signature;

    if (argc != 2) {
        fprintf(stderr, "%s <plain text>\n", argv[0]);
        exit(-1);
    }

    input_string = (unsigned char*)malloc(strlen(argv[1]) + 1);
    assert(input_string);
    strncpy((char*)input_string, argv[1], strlen(argv[1]));

    // Generate random DSA parameters with 1024 bits 
    dsa = DSA_generate_parameters(1024, NULL, 0, NULL, NULL, NULL, NULL);

    // Generate DSA keys
    DSA_generate_key(dsa);

    // alloc sign_string
    sign_string = (unsigned char*)malloc(DSA_size(dsa));    
    assert(sign_string);

    // sign input_string
    ret = DSA_sign(0, input_string,
            strlen((char*)input_string), sign_string, &sig_len, dsa);
    if (0 == ret) {
        fprintf(stderr, "Sign Error.\n");
        exit(-1);
    }

    // verify signature and input_string
    is_valid_signature = DSA_verify(0, input_string,
            strlen((char*)input_string), sign_string, sig_len, dsa);

    // print
    DSAparams_print_fp(stdout, dsa);
    printf("input_string = %s\n", input_string);
    printf("signed string = ");
    for (i = 0; i < sig_len; ++i) {
        printf("%x%x",
                (sign_string[i] >> 4) & 0xf, sign_string[i] & 0xf);    
    }
    printf("\n");
    printf("is_valid_signature? = %d\n", is_valid_signature);

    return 0;
}