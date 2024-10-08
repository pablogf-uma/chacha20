#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void quarter_round(uint32_t state[], int a, int b, int c, int d);

void columns(uint32_t state[]);	

void diagonals(uint32_t state[]);

void state_init(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12]);

void permute_state(uint32_t state[16], uint32_t output_keystream[64]);

void encrypt(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12], char *plaintext, char *output);

typedef struct {uint8_t key[32]; uint8_t nonce[12]; uint32_t blockcount; char plaintext[512]; char expected_ciphertext[512];} test_vector_t;

int run_test(test_vector_t *test);

void time_taken(test_vector_t *test);

#endif // CHACHA20_FUNCTIONS_H