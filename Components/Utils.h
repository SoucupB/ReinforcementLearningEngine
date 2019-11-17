#pragma once
#include <windows.h>
#include <vector>
#include <iostream>

class Utils {
    private:
        static std::string get_name_fron_link(std::string link);
        static std::string changes_signs(std::string link);
    public:
        static std::vector<std::string> get_folder_content(std::string folder);
        static std::string get_path_from_cfg(std::string command_name);
};