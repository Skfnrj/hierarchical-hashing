#ifndef BLAKE2S256_H
#define BLAKE2S256_H

#include <string>
#include "hash_function.h"

class BLAKE2s256 : public HashFunction {
    public:
    std::string hash(const std::string& input) override;
};

#endif