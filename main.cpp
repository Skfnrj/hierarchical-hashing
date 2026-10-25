#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "sha256.h"
#include "merkle_tree.h"


bool read_files(std::string filepath, std::string& content){
    std::ifstream file(filepath, std::ios::binary);

    if(!file.is_open()){
        std::cout<<"Error opening file: "<< filepath << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer<< file.rdbuf();
    content= buffer.str();

    return true;
}

int main(){
    SHA256 sha256_alg;
    MerkleTree tree(&sha256_alg);

    std::vector<std::string> filenames = {"fileA.txt", "fileB.txt", "fileC.txt"};
    std::vector<std::string> file_contents;

    for(size_t i=0; i < filenames.size(); i++){
        std::string content;

        if(read_files(filenames[i], content)){
            file_contents.push_back(content);
        }
    }

    tree.build_tree(file_contents);

    std::cout << "Files Hashed: "<< file_contents.size() << "\n";
    std::cout << "Merkle Root Hash: "<< tree.get_root_hash()<< "\n";

    return 0;
}
