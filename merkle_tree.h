#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>
#include "sha256.h"

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
    SHA256 sha;

    void destroy_tree(Node* node);

public:
    MerkleTree(){
        root=nullptr;
    }

    ~MerkleTree(){
        destroy_tree(root);
    }

    std::string combine_hashes(const std::string& hash1, const std::string& hash2){
        return sha.hash(hash1 + hash2);
    }

    void build_tree(const std::vector<std::string>& data_blocks);
    std::string get_root_hash();

};

#endif