#include <iostream>
#include "merkle_tree.h"

int main(){

    std::vector<std::string> data = {
        "Hello World",
        "This is a test",
        "I need $5"
    };

    MerkleTree tree;
    tree.build_tree(data);



    std::cout << "Data Blocks Hashed: "<< data.size() << "\n";
    std::cout << "Merkle Root Hash: "<< tree.get_root_hash()<< "\n";

    return 0;
}
