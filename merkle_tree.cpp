#include "merkle_tree.h"


//recursive helper to free memory
void MerkleTree::destroy_tree(Node* node){
    if(node){
        destroy_tree(node->left);
        destroy_tree(node-> right);
        delete node;
    }
}

//grab the root hash (return empty if tree isn't built yet)
std::string MerkleTree::get_root_hash(){
    if(!root){
        return "";
    }
    return root-> hash;
}