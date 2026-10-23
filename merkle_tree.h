#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>
#include "hash_function.h"

struct Node{
    std::string hash;
    Node* left;
    Node* right;

    Node(std::string h){
        hash=h;
        left=nullptr;
        right=nullptr;
    }
};

class MerkleTree{
private:
    Node* root;
    HashFunction* hash_alg;

    void destroy_tree(Node* node);

public:
    MerkleTree(HashFunction* alg);
    ~MerkleTree();

    void build_tree(const std::vector<std::string>& data_blocks);
    std::string get_root_hash();
};

#endif