#ifndef FOLDER_READER_H
#define FOLDER_READER_H

#include <string>
#include <vector>

bool list_files(const std::string& folder_path,
                std::vector<std::string>& filenames);

bool read_folder(const std::string& folder_path,
                std::vector<std::string>& filenames,
                std::vector<std::string>& contents);

#endif