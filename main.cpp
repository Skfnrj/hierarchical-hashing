#include <iostream>
#include "sha256.h"

int main(){
    SHA256 sha;
    std::string msg = "abc";
    std::string digest = sha.hash(msg);

    std::cout << "Input: "<< msg<< "\n";
    std::cout << "Hash: "<< digest<< "\n";

    return 0;
}