#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H
#include <string>

class HashFunction{
public:
    virtual ~HashFunction() {}
    virtual std::string hash(const std::string& input) = 0;
};

#endif