#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "sha256.h"
#include "merkle_tree.h"


std::string read_files(std::string filepath){
    std::ifstream file(filepath);
    if (!file.is_open()){
        std::cout <<"Error opening file: " << filepath<< "\n";
        return "";
    }

    std::stringstream buffer;   
    buffer<< file.rdbuf();
    return buffer.str();
}

int main(){
    SHA256 sha256_alg;
    MerkleTree tree(&sha256_alg);

    std::vector<std::string> filenames = {"fileA.txt", "fileB.txt", "fileC.txt"};
    std::vector<std::string> file_contents;

    for(size_t i = 0; i < filenames.size(); i++){
        std::string content = read_files(filenames[i]);
        if(content != ""){
            file_contents.push_back(content);
        }
    }

    tree.build_tree(file_contents);

    std::cout << "Files Hashed: "<< file_contents.size() << "\n";
    std::cout << "Merkle Root Hash: "<< tree.get_root_hash()<< "\n";

    return 0;
}
