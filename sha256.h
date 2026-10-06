#ifndef SHA256_H
#define SHA256_H

#include <cstdint>
#include <vector>
#include <string>

class SHA256{
    public:

    //These are the initial hash values I got from Wikipedia pseudocode
    //(first 32 bits of the fractional parts of the sq roots of the first 8 primes)
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    //Initialize array of round constants
    uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    //Helper function for right rotate
    // (x >> n) shifts the main bits right
    // (x << (32-n)) catches the bits that fall off the edge
    // the bitwise OR glues them back together
    uint32_t rightrotate(uint32_t x, uint32_t n){
        return (x >> n) | (x << (32-n));
    }

    // Choose (ch): For each bit index, if x is 1, the bit from y is chosen. If x is 0, choose the bit from z.
    uint32_t ch(uint32_t x, uint32_t y, uint32_t z){
        return (x & y) ^ (~x & z);
    }

    // Majority (maj): Returns majority bit of x, y, and z.
    uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    //Uppercase Sigma 0 and 1 (used in the main compression loop)
    uint32_t upper_sigma(uint32_t x){
        return rightrotate(x,2) ^ rightrotate(x,13) ^ rightrotate(x,22);
    }

    uint32_t upper_sigma(uint32_t x){
        return rightrotate(x,6) ^ rightrotate(x,11) ^ rightrotate(x,25);
    }

    //Lowercase sigma 0 and 1 (used to prepare the message schedule)
    //both use right rotation and a standard right shift
    uint32_t lower_sigma(uint32_t x){
        return rightrotate(x,7) ^ rightrotate(x,18) ^ (x >> 3);
    }

    uint32_t lower_sigma(uint32_t x){
        return rightrotate(x,17) ^ rightrotate(x,19) ^ (x >> 10);
    }



    //TODO: Write out the sigma functions from pseudocode
    //TODO:Figure out how to handle the 512-bit message padding


};

 
#endif