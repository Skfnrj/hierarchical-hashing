#include "folder_reader.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <system_error>
#include <fstream>

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

bool read_folder(const std::string&folder_path,
                std::vector<std::string>& filenames,
                std::vector<std::string>& contents){
    contents.clear();

    if(!list_files(folder_path, filenames)){
        return false;
    }

    for(const std::string& filename : filenames){
        std::filesystem::path file_path = folder_path;
        file_path.append(filename);

        std::ifstream file(file_path, std::ios::binary);
        std::string data;
        char buffer[4096];

        while(file.read(buffer, sizeof(buffer))){
            data.append(buffer, sizeof(buffer));
        }

        if(file.bad() || !file.eof()){
            std::cout<< "Cannot read file: "<< file_path<< "\n";
            filenames.clear();
            contents.clear();
            return false;
        }

        data.append(buffer, file.gcount());
        contents.push_back(data);
    }

    return true;

}