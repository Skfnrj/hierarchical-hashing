#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <system_error>
#include "sha256.h"
#include "merkle_tree.h"
#include "folder_reader.h"
#include "blake2s256.h"

int main(int argc, char* argv[]){
    bool copy_changes = false;
    bool use_blake2s = false;
    bool show_tree = false;

    if(argc < 3 || argc > 6){
        std::cout << "Usage: ./merkle_demo <first_folder> <second_folder> "
                    "[--copy] [--blake2s] [--show-tree]\n";
        return 1;
    }

    for(int i =3; i < argc; i++){
        std::string option = argv[i];

        if(option == "--copy" && !copy_changes){
            copy_changes = true;
        }
        else if(option == "--blake2s" && !use_blake2s){
            use_blake2s = true;
        }
        else if(option == "--show-tree" && !show_tree){
            show_tree = true;
        }
        else{
            std::cout << "Unknown or repeated option: " << option<< "\n";
            return 1;
        }
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
    BLAKE2s256 blake;
    HashFunction* hash_alg = &sha;

    if(use_blake2s){
        hash_alg = &blake;
    }

    MerkleTree first(hash_alg);
    MerkleTree second(hash_alg);

    first.build_tree(first_contents);
    second.build_tree(second_contents);

    std::cout <<  "Source root:      "<< first.get_root_hash() << "\n";
    std::cout <<  "Destination root: "<< second.get_root_hash() << "\n";

    if(show_tree){
        std::cout << "\nSource tree\n";
        first.print_tree(first_names);

        std::cout << "\nDestination tree\n";
        second.print_tree(second_names);
        std::cout << "\n";
    }


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