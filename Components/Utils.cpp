#include "Utils.h"
#include <algorithm>
#include <map>
#include <fstream>

std::string Utils::get_name_fron_link(std::string link) {
    int link_index = link.size() - 1;
    std::string result = "";
    while(link_index >= 0 && link[link_index] != '.') {
        link_index--;
    }
    link_index--;
    while(link_index >= 0 && link[link_index] != '\\') {
        result += link[link_index];
        link_index--;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string Utils::get_path_from_cfg(std::string command_name) {
  std::ifstream reader;
  reader.open("cfg", std::ios::in);
  std::string line;
  while(reader >> line) {
    std::string path_config = "";
    int index = 0;
    while(index < line.size() && line[index] != '=') {
      path_config += line[index];
      index++;
    }
    if(path_config == command_name) {
      index++;
      std::string path_to_exp_files = "";
      while(index < line.size()) {
        path_to_exp_files += line[index];
        index++;
      }
      return path_to_exp_files;
    }
  }
  return "";
}

std::vector<std::string> Utils::get_folder_content(std::string folder) {
    FILE* pipe =  NULL;
    std::replace(folder.begin(), folder.end(), '/', '\\');
    std::vector<std::string> result_vector;
    std::map<std::string, bool> uniq_checker;
    std::string pCmd = "dir /B /S " + std::string(folder);
    char buf[256];
    if( (pipe = _popen(pCmd.c_str(),"rt")) == NULL)
    {
        std::cout << "The specified path does not exists!!" << "\n";
        return result_vector;
    }
    while (!feof(pipe))
    {
        if(fgets(buf,256,pipe) != NULL)
        {
            std::string file_name = get_name_fron_link(std::string(buf));
            if(!uniq_checker[file_name]) {
                uniq_checker[file_name] = true;
                result_vector.push_back(file_name);
            }
        }
    }
    _pclose(pipe);
    return result_vector;
}