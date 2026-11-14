#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>
#include <cstddef>
#include "hash_function.h"

struct Node{
    std::string hash;
    Node* left;
    Node* right;
    int data_index; //for input position

    Node(std::string h){
        hash=h;
        left=nullptr;
        right=nullptr;
        data_index = -1; //no individual ipnut position since it starts at 0
    }
};

class MerkleTree{
private:
    Node* root;
    HashFunction* hash_alg;
    std::size_t data_count;

    void destroy_tree(Node* node);

    void compare_nodes(const Node* first, const Node* second,
                        std::vector<int>& differences) const;

public:
    MerkleTree(HashFunction* alg);
    ~MerkleTree();

    void build_tree(const std::vector<std::string>& data_blocks);
    std::string get_root_hash();

    void print_tree(const std::vector<std::string>& filenames) const;

    bool compare_tree(const MerkleTree& other,
                        std::vector<int>& differences) const;
};

#endif