#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include "folder_reader.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout <<"Provide a temporary test folder\n";
        return 1;
    }

    std::filesystem::path test_root = argv[1];
    
    std::filesystem::path folder = test_root;
    folder.append("files");

    std::filesystem::path empty_folder = test_root;
    empty_folder.append("empty");

    std::filesystem::path subfolder = folder;
    subfolder.append("subfolder");

    std::filesystem::create_directory(folder);
    std::filesystem::create_directory(empty_folder);
    std::filesystem::create_directory(subfolder);

    std::filesystem::path last_path = folder;
    last_path.append("z.txt");
    std::ofstream last(last_path);
    last<< "last file";
    last.close();

    std::filesystem::path first_path = folder;
    first_path.append("a.txt");
    std::ofstream first(first_path);
    first<< "first file";
    first.close();

    std::filesystem::path empty_path = folder;
    empty_path.append("empty.txt");
    std::ofstream empty_file(empty_path);
    empty_file.close();

    std::filesystem::path link_path = folder;
    link_path.append("link.txt");
    std::filesystem::create_symlink(first_path, link_path);

    std::vector<std::string> filenames;
    std::vector<std::string> expected ={
        "a.txt", "empty.txt", "z.txt"
    };

    assert(list_files(folder.string(), filenames));
    assert(filenames == expected);

    std::vector<std::string> contents;
    std::vector<std::string> expected_contents = {
        "first file", "", "last file"
    };

    assert(read_folder(folder.string(), filenames, contents));
    assert(filenames == expected);
    assert(contents == expected_contents);

    assert(list_files(empty_folder.string(), filenames));
    assert(filenames.empty());

    std::filesystem::path missing_folder = test_root;
    missing_folder.append("missing");

    filenames.push_back("old.txt");
    assert(!list_files(missing_folder.string(), filenames));
    assert(filenames.empty());

    std::cout<< "All folder listing tests passed\n";
    return 0;
}