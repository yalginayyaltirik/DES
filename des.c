#include <math.h>
#include <time.h>
#include "des.h"


int main(){

    // Question-1: Implement DES
    uint64_t userKey = 0x0123456789abcdef;
    uint64_t plaintext = 0x4e6f772069732074;
    uint64_t ciphertext = 0x0;
 
    ciphertext = Des(plaintext, userKey);

    // Question-2: Encrypting my fullname by DES with CBC mode
    // Yalginay Yaltirik ASCI: 59616C67696E61792059616C746972696B
    // Yalginay Yaltirik ASCI with padding (10*)
    uint64_t fullname[3] = {0x59616C67696E6179, 0x2059616C74697269, 0x6B80000000000000};
    uint64_t keyForName = 0x59616C67696E6179;
    uint64_t nameCiphertext[3] = {0x0, 0x0, 0x0};
    uint64_t numberOfBlock = 3;
 
    CbcModeDes(fullname, nameCiphertext, keyForName, 3);
    for(uint8_t i=0; i < 3; i++){
        printf("nameCiphertext[%d]: %016llx\n", i, nameCiphertext[i]);
    }

    // Question 3
    uint64_t testInput = 0x2059616C74697269;
    uint64_t testKey = 0x59616C67696E6179;
    uint64_t testCipher = testInput;

    clock_t startTime = clock();
    for (uint32_t i = 0; i < pow(2, 23); i++)
    {
        testCipher = Des(testCipher, testKey);
    }
    clock_t endTime = clock();
    double cpuUsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    printf("DES encryption takes: %lf seconds on a single core of Intel(R) Core(TM) i7-10700F CPU @ 2.90GHz\n ", cpuUsedTime);

    return 0;
}
