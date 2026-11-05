#include "blake2s256.h"
#include <cstdint>

static std::uint32_t rotate_right(std::uint32_t value, int count){
    return (value >> count) | (value << (32 - count));
}


//BLAKE2s G mixing fucntion from RFC 7693, section 3.1.
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