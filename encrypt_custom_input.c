#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chacha20_functions.h"

void encrypt_custom_input(char *ciphertext)
{
    uint32_t state[16];
    uint8_t key[32];
    char key_char[33]; // To accomodate the null terminator
    uint8_t nonce[12];
    char nonce_char[13];
    char input[256];
    
    printf("\nKey (32 bytes):\n");
    // Read key as a string
    fgets(key_char, sizeof(key_char), stdin);
    int key_char_len = strlen(key_char);
    // Convert each character of the string to hex and load it to the key array
    for (size_t i = 0; i < key_char_len; i++)
    {   
        key[i] = (uint8_t)key_char[i];
    }

    /* TEST
    for (size_t i = 0; i < key_char_len; i++) {
        printf("%02X ", key[i]);
    }
    printf("\n");*/

    // Clear the input buffer to erase the newline character
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // Same operation for the nonce
    printf("\nNonce (12 bytes):\n");
    fgets(nonce_char, sizeof(nonce_char), stdin);
    int nonce_char_len = strlen(nonce_char);

    for (size_t i = 0; i < nonce_char_len; i++)
    {   
        nonce[i] = (uint8_t)nonce_char[i];
    }

    /* TEST
    for (size_t i = 0; i < nonce_char_len; i++) {
        printf("%02X ", nonce[i]);
    }
    printf("\n");*/

    /* TEST (hardcoded key and nonce)
    uint8_t key[] = { 
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
                };

    uint8_t nonce[] = {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a,
                0x00, 0x00, 0x00, 0x00 
                };*/


    int d;
    while ((d = getchar()) != '\n' && d != EOF);
    printf("\nPlaintext:\n");
    fgets(input, sizeof(input), stdin);
    int plaintext_len = strlen(input) - 1; // -1 to eliminate the null terminator from the input string
    
    /* TEST
    for (size_t i = 0; i < plaintext_len; i++) {
        printf("%02X ", input[i]);
    }
    printf("\n");*/
    
    encrypt(state, "expand 32-byte k", key, 0, nonce, input, ciphertext);

    printf("\nCiphertext (in hex):\n");
    
    for (size_t j = 0; j < plaintext_len; j++) {
        printf("%02X", (unsigned char)ciphertext[j]);
    }
}