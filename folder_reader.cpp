#include "folder_reader.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <system_error>

bool list_files(const std::string& folder_path, std::vector<std::string>& filenames){
    filenames.clear();

    std::error_code error;
    std::filesystem::directory_iterator current(folder_path,error);
    std::filesystem::directory_iterator end;

    if(error){
        std::cout<<"Cannot open folder: "<< folder_path<<"\n";
        return false;
    }

    while(current != end){
        std::filesystem::file_status status = current->symlink_status(error);

        if(error){
            std::cout<< "Cannot check a folder entry\n";
            filenames.clear();
            return false;
        }

        if(std::filesystem::is_regular_file(status)){
            filenames.push_back(current->path().filename().string());
        }

        current.increment(error);

        if(error){
            std::cout<<"Cannot continue reading folder\n";
            filenames.clear();
            return false;
        }
    }

    std::sort(filenames.begin(), filenames.end());
    return true;
}