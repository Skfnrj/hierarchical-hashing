#include <iostream>
#include <vector>
#include <string>
#include "sha256.h"
#include "merkle_tree.h"
#include "folder_reader.h"

int main(int argc, char* argv[]){
    if(argc !=3){
        std::cout<< "Usage: ./merkle_demo <first_folder> <second_folder>\n";
        return 1;
    }

    std::vector<std::string> first_names;
    std::vector<std::string> second_names;
    std::vector<std::string> first_contents;
    std::vector<std::string> second_contents;

    if(!read_folder(argv[1], first_names, first_contents) ||
        !read_folder(argv[2], second_names, second_contents)){
            return 1;
    }

    if(first_names != second_names){
        std::cout<< "The folders must contain the same filenames "
                    "for this comparison.\n";
        return 1;
    }

    SHA256 sha;
    MerkleTree first(&sha);
    MerkleTree second(&sha);

    first.build_tree(first_contents);
    second.build_tree(second_contents);

    std::vector<int> differences;
    if(!first.compare_tree(second, differences)){
        std::cout<< "Could not compare the two trees\n";
        return 1;
    }

    if(differences.empty()){
        std::cout<< "The files match\n";
    }
    else{
        std::cout<< "Files with different contents:\n";
        for(int index : differences){
            std::cout<< first_names[index] << "\n";
        }
    }

    return 0;
}