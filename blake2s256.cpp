#include "blake2s256.h"
#include <cstdint>
#include <cstddef>
#include <iomanip>
#include <sstream>

static std::uint32_t rotate_right(std::uint32_t value, int count){
    return (value >> count) | (value << (32 - count));
}


//BLAKE2s G mixing function from RFC 7693, section 3.1. 
static void mix(std::uint32_t& a, std::uint32_t& b,
                std::uint32_t& c, std::uint32_t& d,
                std::uint32_t x, std::uint32_t y){
    
    a = a + b + x;
    d = rotate_right(d ^ a, 16);
    c = c + d;
    b = rotate_right(b ^ c, 12);

    a = a + b + y;
    d = rotate_right( d ^ a, 8);
    c = c + d;
    b = rotate_right(b ^ c, 7);
}

//Initialization vector
static const std::uint32_t IV[8] = {
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
    0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

static const int SIGMA[10][16] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
    { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
    { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
    { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
    { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
    { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
    { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
    { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
    { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 }
};

static void compress(std::uint32_t state[8],
                    const std::uint32_t words[16],
                    std::uint64_t byte_count, bool last_block){
    std::uint32_t v[16];

    for(int i = 0; i < 8; i++){
        v[i] = state[i];
        v[i+8]= IV[i];
    }

    v[12] ^= static_cast<std::uint32_t>(byte_count);
    v[13] ^= static_cast<std::uint32_t>(byte_count >> 32);

    if(last_block){
        v[14] = ~v[14];
    }

    for(int round = 0; round < 10; round++){
        const int* order = SIGMA[round];

        //Mix the columns
        mix(v[0], v[4], v[8], v[12], words[order[0]], words[order[1]]);
        mix(v[1], v[5], v[9], v[13], words[order[2]], words[order[3]]);
        mix(v[2], v[6], v[10], v[14], words[order[4]], words[order[5]]);
        mix(v[3], v[7], v[11], v[15], words[order[6]], words[order[7]]);

        //Mix the diagonals
        mix(v[0], v[5], v[10], v[15], words[order[8]], words[order[9]]);
        mix(v[1], v[6], v[11], v[12], words[order[10]], words[order[11]]);
        mix(v[2], v[7], v[8], v[13], words[order[12]], words[order[13]]);
        mix(v[3], v[4], v[9], v[14], words[order[14]], words[order[15]]);
    }

    for(int i=0; i < 8; i++){
        state[i] = state[i] ^ v[i] ^ v[i + 8];
    }
}

std::string BLAKE2s256::hash(const std::string& input){
    std::uint32_t state[8];
    for(int i = 0; i < 8; i++){
        state[i] = IV[i];
    }

    //32-byte digest, no key, sequential hashing
    state[0] ^= 0x01010020;

    std::size_t offset = 0;

    do{
        std::uint32_t words[16] = {};
        std::size_t remaining = input.size() - offset;
        std::size_t block_size = remaining < 64 ? remaining : 64;

        for(std::size_t i =0; i < block_size; i++){
            std::uint32_t byte = static_cast<unsigned char>(input[offset + i]);
            
            std::size_t word_index = i/4;
            std::size_t shift = 8 * (i % 4);

            words[word_index] = words[word_index] | (byte << shift);
        }

        offset += block_size;
        bool last_block = offset == input.size();

        compress(state, words, offset, last_block);
    }while(offset < input.size());

    std::ostringstream output;
    output << std::hex << std::setfill('0');

    for(int i =0; i < 8; i++){
        for(int shift = 0; shift < 32; shift +=8){
            std::uint32_t byte = (state[i] >> shift) & 0xFF;
            output << std::setw(2) << byte;
        }
    }

    return output.str();
}