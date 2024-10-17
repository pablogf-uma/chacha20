# include <stdio.h>
# include <time.h>
# include <string.h>
# include "chacha20_functions.h"
# include <windows.h>
# include <stdlib.h>

/*

    This function will allow us to calculate the throughput of the encryption algorithm. 
    Its output is the throughput in bytes per second.

    We use windows.h for more exactitude when measuring time, since this library directly interacts with the OS.

    We will do so by counting the number of counts (ticks) produced by the high-resolution timer in a given time interval, so that we can break a second into smaller pieces.

*/

void calculate_throughput(test_vector_t *test)
{
    // Calculate the time taken to encrypt the plaintext
    LARGE_INTEGER frequency, start, end; // Frequency stores a 64-bit integer value representing the number of counts per second.
    double time_taken;                   //  This frequency will allow us to be more exact when measuring time.

    QueryPerformanceFrequency(&frequency); // Retrieves the number of counts produces per second.

    QueryPerformanceCounter(&start); // Provides the current count from the timer.
    run_test(test);
    QueryPerformanceCounter(&end); // Call it again to get the ending number of counts.

    time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart; // QuadPart to access the number of counts and convert from ticks to seconds.

    // Calculate the length in bytes of the plaintext.
    int length = strlen(test->plaintext);

    // Output results
    printf("Time taken: %.6f seconds.\n", time_taken);
    printf("Length: %i bytes.\n", length);
    if (length == 0)
    {
        printf("Test contained all 0's as plaintext.\n");
        return;
    }
    else{
        printf("Throughput: %.2f bytes per second.\n", length / time_taken);
    }
}


/* 

    This function will calculate the amount of bytes processed in 1 second by the chacha20 algorithm.

    It will execute as many encryption operations as possible in 1 second, and then output the number of bytes processed in that time.

    All the parameters required for encryption will be updated each time the encryption function is called.

    This way the processor is forced to encrypt each time with different parameters, so that we can measure the true throughut of the algorithm.

*/

void calculate_throughput_2()
{
    // Random seed for later randum numer generation.
    srand(time(NULL));

    // Get the start time
    clock_t start_time = clock();

    // Counter for number of bytes processed.
    int bytes_processed = 0;

    // Counter for number of encrypt() calls.
    int encrypt_calls = 0;

    // Start values for the encryption function´s parameters.
    uint32_t state[16] = {0};
    uint8_t key[] = { 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
                };

    uint8_t nonce[] = { 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
                };

    uint32_t blockcount = 0;

    uint8_t plaintext[] = {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            };
    uint32_t expected_ciphertext = 0;

    do {
        clock_t current_time = clock(); // Get the current time at the begining of the loop.

        // Run the encryption algorithm with the updated parameters.
        test_vector_t test;

        // Copy the parameters to the test struct.
        memcpy(test.key, key, sizeof(key));
        memcpy(test.nonce, nonce, sizeof(nonce));
        test.blockcount = blockcount;
        memcpy(test.plaintext, plaintext, sizeof(plaintext));

        encrypt(state, "expand 32-byte k", test.key, test.blockcount, test.nonce, test.plaintext, test.expected_ciphertext);

        // Update the number of bytes processe and the number of encrypt() calls.
        bytes_processed += (int)strlen(test.plaintext);
        encrypt_calls += 1;

        // Generate 3 random numbers for the positions where the parameters will be updated for the next iteration.
        int min = 0;
        int max_key = 31;
        int max_nonce = 11;
        int max_plaintext = 63;

        int rand_num_key = rand() % (max_key - min + 1) + min;
        int rand_num_nonce = rand() % (max_nonce - min + 1) + min;
        int rand_num_plaintext = rand() % (max_plaintext - min + 1) + min;

        key[rand_num_key] += 1;
        nonce[rand_num_nonce] += 1;
        blockcount += 1;
        plaintext[rand_num_plaintext] += 1;

    } while (((double)(clock() - start_time)) / CLOCKS_PER_SEC < 1.0); // Loop until 1 second has passed.
    
    // Output results
    printf("Throughput: %i bytes per second.\n", bytes_processed);
    printf("Number of chacha20 calls in 1 second: %i.\n", encrypt_calls);
}
