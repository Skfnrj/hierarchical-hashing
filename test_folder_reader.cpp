#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include "folder_reader.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout <<"Provide a temporary test filder\n";
        return 1;
    }

    std::filesystem::path test_root = argv[1];
    
}