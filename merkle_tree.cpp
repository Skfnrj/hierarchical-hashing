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

//build the tree from bottom up
void MerkleTree::build_tree(const std::vector<std::string>& data_blocks){
    if(data_blocks.empty()) return;

    std::vector<Node*> nodes;

    //hash base leaves
    for(size_t i=0; i < data_blocks.size(); i++){
        std::string raw_data = data_blocks[i];
        std::string hashed_data = sha.hash(raw_data);
        Node* leaf = new Node(hashed_data);
        nodes.push_back(leaf);
    }


    //combine layers until only 1 root remains
    while(nodes.size() > 1){
        std::vector<Node*> parent_layer;

        for(size_t i = 0; i < nodes.size(); i += 2){
            if(i + 1 < nodes.size()){
                //normal pair
                std::string left_hash = nodes[i]->hash;
                std::string right_hash = nodes[i + 1]->hash;
                std::string combined = combine_hashes(left_hash, right_hash);

                Node* parent = new Node(combined);
                parent->left = nodes[i];
                parent->right = nodes[i + 1];
                parent_layer.push_back(parent);
            }

            else{
                //odd node out, hash with itself
                std::string odd_hash = nodes[i]->hash;
                std::string combined = combine_hashes(odd_hash, odd_hash);

                Node* parent = new Node(combined);
                parent->left = nodes[i];
                parent->right = nullptr;
                parent_layer.push_back(parent);
            }
        }
        //move up a layer
        nodes = parent_layer;
    }

    //set root
    root = nodes[0];
}