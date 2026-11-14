#include "merkle_tree.h"
#include <iostream>

MerkleTree::MerkleTree(HashFunction* alg){
    root = nullptr;
    hash_alg = alg;
    data_count = 0;
}

MerkleTree::~MerkleTree(){
    destroy_tree(root);
}


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

std::string combine_hashes(HashFunction* alg, const std::string& left, const std::string& right){
    return alg->hash(left + right);
}

//build the tree from bottom up
void MerkleTree::build_tree(const std::vector<std::string>& data_blocks){
    if(root != nullptr){
        destroy_tree(root);
        root = nullptr;
    }

    data_count = data_blocks.size();

    if(data_blocks.empty()){
        return;
    }

    std::vector<Node*> nodes;
    //hash base leaves
    for(size_t i=0; i < data_blocks.size(); i++){
        std::string current_hash = hash_alg->hash(data_blocks[i]);
        Node* leaf = new Node(current_hash); //store leaf address
        leaf->data_index = static_cast<int>(i); //record input position
        nodes.push_back(leaf); //add tree pointer to current tree lvl
    }


    //combine layers until only 1 root remains
    while(nodes.size() > 1){
        std::vector<Node*> parent_layer;

        for(size_t i = 0; i < nodes.size(); i += 2){
            if(i + 1 < nodes.size()){
                //normal pair
                std::string left_hash = nodes[i]->hash;
                std::string right_hash = nodes[i + 1]->hash;
                std::string combined = combine_hashes(hash_alg, left_hash, right_hash);

                Node* parent = new Node(combined);
                parent->left = nodes[i];
                parent->right = nodes[i + 1];
                parent_layer.push_back(parent);
            }

            else{
                //odd node out, hash with itself
                std::string odd_hash = nodes[i]->hash;
                std::string combined = combine_hashes(hash_alg, odd_hash, odd_hash);

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

void MerkleTree::compare_nodes(const Node* first, const Node* second,
                                std::vector<int>& differences) const{
    if(first == nullptr || second == nullptr){
        return;
    }

    //for matching hashes, skip this subtree
    if(first->hash == second->hash){
        return;
    }

    //differing leaf, its input position recorded
    if(first->left == nullptr && first->right == nullptr){
        differences.push_back(first->data_index);
        return;
    }

    compare_nodes(first->left, second->left, differences);
    compare_nodes(first->right, second->right, differences);

}

bool MerkleTree::compare_tree(const MerkleTree& other,
                                std::vector<int>& differences) const{
    differences.clear();

    if(data_count != other.data_count){
        return false;
    }

    //both trees must use the same hasshing object for now
    if(hash_alg == nullptr || hash_alg != other.hash_alg){
        return false;
    }

    compare_nodes(root, other.root, differences);
    return true; 
}

static void print_node(const Node* node, const std::vector<std::string>& filenames,
                        std::size_t depth){
    if(node == nullptr){
        return;
    }

    std::cout << std::string(depth *2, ' ');

    if(node->data_index >= 0){
        std::cout << filenames[node->data_index];
    }
    else{
        std::cout << (depth == 0 ? "Root" : "Branch");
    }

    std::cout << ": "<< node->hash.substr(0, 12)<< "\n";
    print_node(node->left, filenames, depth + 1);
    print_node(node->right, filenames, depth + 1);
}

void MerkleTree::print_tree(const std::vector<std::string>& filenames) const{
    if(root == nullptr){
        std::cout<< "(empty tree)\n";
        return;
    }

    if(filenames.size() != data_count){
        std::cout << "Cannot display tree: filename count does not match\n";
        return;
    }

    print_node(root, filenames, 0);
}
