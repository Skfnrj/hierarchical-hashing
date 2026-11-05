#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <system_error>
#include "sha256.h"
#include "merkle_tree.h"
#include "folder_reader.h"

int main(int argc, char* argv[]){
    bool copy_changes = argc == 4 && std::string(argv[3]) == "--copy";
    if(argc !=3 && !copy_changes){
        std::cout<< "Usage: ./merkle_demo <first_folder> <second_folder> [--copy]\n";
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

    if(copy_changes){
        for(int index : differences){
            const std::string& filename = first_names[index];

            std::filesystem::path source = argv[1];
            source.append(filename);

            std::filesystem::path destination = argv[2];
            destination.append(filename);

            std::error_code error;
            bool copied = std::filesystem::copy_file(
                source, destination,
                std::filesystem::copy_options::overwrite_existing, error);

            if(!copied){
                    std::cout<< "Could not copy "<< filename
                             <<": " << error.message()<< "\n";
                    return 1;
                }

            std::cout<< "Copied: "<< filename << "\n";
            
        }
    }

    return 0;
}