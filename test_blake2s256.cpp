#include <iostream> 
#include <cassert>
#include <string>
#include "blake2s256.h"

int main(){
    BLAKE2s256 blake;

    //EMpty input
    assert(blake.hash("") ==
        "69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9");
}