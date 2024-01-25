#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_ROUND 16

// Define the 56-bit Permutation Order
const uint8_t pc1[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60,
52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

// Define the 46-bit Permutation Order
const uint8_t pc2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7,
27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

// Define the round counter
const uint8_t rc[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Define the Initial Permutation
const uint8_t ip[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 
    21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

// Define the Final Permutation
const uint8_t fp[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 
    18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};

// Define the Round Permutation
const uint8_t rp[64] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19,
    13, 30, 6, 22, 11, 4, 25};

// Define the Expanded Output
const uint8_t expout[64] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17,
    18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

// Define Des s-Box table 
const uint8_t s[8][64] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 
                        8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
                        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 
                        5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
                        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15,
                        1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
                        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14,
                        9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
                        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 
                        6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
                        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 
                        8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
                        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 
                        6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
                        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 
                        2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

//Generates 32-round keys for each round
void GenerateRoundKey(uint64_t userKey, uint64_t *roundKey){
    
    uint64_t tempUserKey = 0;

    // Perform PC1
    for (uint8_t i=0; i < 56; i++){
        tempUserKey <<= 0x1;
        tempUserKey |= (userKey >> (64 - pc1[i])) & 0x1;
    }
    userKey = tempUserKey; // PC1 performed, 56 bit key obtained
    // printf("UserKey: %016llx\n", userKey);

    //Split 56-bit to 28-bit halves
    uint32_t msb28 = (userKey >> 28) & 0xFFFFFFF;
    uint32_t lsb28 = userKey & 0xFFFFFFF;

    for(uint8_t i=0; i < NUMBER_OF_ROUND; i++){
        //Rotate 28-bit halves with rotation count. I used for loop to shift operation since input is not high, it does not cause any performance leak!
        for(uint8_t j=0; j < rc[i]; j++){
            // Firstly, get the 27-bit then OR it with the previous step MSB
            msb28 = ((msb28 << 1) & 0xFFFFFFE) | ((msb28 >> 27) & 0x1);
            lsb28 = ((lsb28 << 1) & 0xFFFFFFE) | ((lsb28 >> 27) & 0x1); 
        }
        // Merge 28-bit halves to obtain 56-bit
        tempUserKey = ((uint64_t)msb28 << 28) | lsb28; 
        // Perform PC2
        roundKey[i] = 0; // clear the subkey before updating
        for(uint8_t j=0; j < 48; j++){
            roundKey[i] <<= 0x1;
            roundKey[i] |= (tempUserKey >> (56 - pc2[j])) & 0x1;
        }
    }
    return;
}

// Initial Permutation
void InitialPermutation(uint64_t *userInput){
    
    uint64_t tempIn = {0};
    
    for (uint8_t i=0; i < 64; i++){
        tempIn <<= 0x1;
        tempIn |= (*userInput >> (64 - ip[i])) & 0x1;
    }
    *userInput = tempIn;
    return;
}

// Final Permutation
uint64_t FinalPermutation(uint64_t userInput){
    
    uint64_t tempIn = {0};
    
    for (uint8_t i=0; i < 64; i++){
        tempIn <<= 0x1;
        tempIn |= (userInput >> (64 - fp[i])) & 0x1;
    }
    return tempIn;
}

// Round Operation Permutation Function
void RoundPermutation(uint32_t *userInput){
    uint32_t tempIn = 0;
    *userInput = (uint64_t)*userInput;
    for (uint8_t i=0; i < 32; i++){
        tempIn <<= 0x1;
        tempIn |= (*userInput >> (32 - rp[i])) & 0x1;
    }
    *userInput = (uint32_t)tempIn;
    return;
}

// Round Expansion Function
uint64_t RoundExpansion(uint32_t userInput){
    
    uint64_t expandedOut = {0};
    for (uint8_t i=0; i < 48; i++){
        expandedOut <<= 0x1;
        expandedOut |= (uint64_t)(userInput >> (32 - expout[i])) & 0x1;
    }
    return expandedOut;
}

// 48bit bit input S-box operation
uint32_t RoundSBox(uint64_t sBoxIn)
{
    uint32_t sBoxOut = 0;
    uint8_t sRow, sCol;
    for (uint8_t i = 0; i < 8; i++)
    {
        // Extract the sRow to detect which s-box will be used(bit 0 and 5) 
        sRow = (uint8_t)((sBoxIn & (0x0000840000000000 >> 6 * i)) >> 42 - 6 * i);
        sRow = (sRow >> 4) | sRow & 0x1;
        // Extract the sCol to give 4-bit input to s-box(bit 1, 2, 3 and 4)
        sCol = (uint8_t)((sBoxIn & (0x0000780000000000 >> 6 * i)) >> 43 - 6 * i);
        sBoxOut <<= 4;
        sBoxOut |= (uint32_t)(s[i][16 * sRow + sCol] & 0xF); // Perform the s-box operation with extracted sCol and sRow
    }
    return sBoxOut;
}

//Generates 32-round keys for each round
uint64_t Des(uint64_t input, uint64_t userKey){
    uint64_t roundKey[NUMBER_OF_ROUND] = {0}; 
    uint64_t roundInput = {0}; 
    uint32_t sBoxOut = {0}; 
    uint64_t ciphertext = {0}; 

    GenerateRoundKey(userKey, roundKey); // Generate 16 round keys for each round 
    InitialPermutation(&input); // Intial permutation for 64-bit input
    uint32_t msb32 = (uint32_t)((input >> 32) & 0xFFFFFFFF); // Seperate most significant 32-bit
    uint32_t lsb32 = (uint32_t)(input & 0xFFFFFFFF); // Seperate least significant 32-bit
    // Round Operation
    for(uint8_t i = 0; i < NUMBER_OF_ROUND; i++){
        roundInput = RoundExpansion(lsb32); // Expand 32-bit input to 48-bit output
        roundInput ^= roundKey[i]; // XOR'ing round key and 48-bit input (For encryption)
        sBoxOut = RoundSBox(roundInput); // S-box operation for 48-bit input to map 32-bit
        RoundPermutation(&sBoxOut); // Permute 32-bit in each round
        // Swap right most 32-bit and left-most 32-bit
        uint32_t tempSwap;
        tempSwap = lsb32;
        lsb32 = msb32 ^ sBoxOut;
        msb32 = tempSwap;
        // printf("Round: %i -> msb32: %08x lsb32: %08x K: %012llx \n",i+1, msb32, lsb32, roundKey[i]);
    }
    input = ((uint64_t)lsb32 << 32) | (uint64_t)msb32;
    // printf("Round: 16 -> after concat. input: %016llx\n", input, roundKey[15]);
    ciphertext = FinalPermutation(input);
    // printf("ciphertext: %016llx\n", ciphertext);
    return ciphertext;
}

// CBC-moded DES
void CbcModeDes(uint64_t *plaintext, uint64_t *ciphertext, uint64_t key, uint8_t block_number)
{
    uint64_t IV = 0;
    // Obtain 64-bit random value
    for (int i = 0; i < 64; i++){
        IV = IV * 2 + rand() % 2;
    }
    // Perform DES for the first block
    uint64_t out = IV;
    plaintext[0] ^= out;
    out = Des(plaintext[0], key);
    ciphertext[0] = out;
    // Perform DES for the remaining blocks
    for (int i = 1; i < block_number; i++)
    {
        plaintext[i] ^= out;
        out = Des(plaintext[i], key);
        ciphertext[i] = out;
    }
};
