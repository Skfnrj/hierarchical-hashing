#include <iostream> 
#include <cassert>
#include <string>
#include "blake2s256.h"

int main(){
    BLAKE2s256 blake;

    //Empty input
    assert(blake.hash("") ==
        "69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9");

    //Short input
    assert(blake.hash("abc") ==
        "508c5e8c327c14e2e1a72ba34eeb452f37458b209ed63a294d999b4c86675982");

    //Exactly one 64-byte block
    assert(blake.hash(std::string(64, 'a')) ==
        "651d2f5f20952eacaea2fba2f2af2bcd633e511ea2d2e4c9ae2ac0d9ffb7b252");

    //One full block and one extra byte
    assert(blake.hash(std::string(65, 'a')) ==
        "045f8ae18932119bd051ac7ba5c73db59892055fad5c32f82d79a6543d92a497");

    std::cout << "All BLAKE2s tests passed\n";
    return 0;
}