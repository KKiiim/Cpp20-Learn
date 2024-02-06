#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

void findJobFilesPath(const std::filesystem::path &directoryPath, std::vector<std::string> &result) {

  if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
    std::cout<<"TEST_INFRA , MAIN" << "Invalid directory path: " << directoryPath << '\n';
    std::terminate();
  }

  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    const std::filesystem::path &filePath = entry.path();
    if (std::filesystem::is_regular_file(filePath) && filePath.extension() == ".cpp") {
      result.push_back(filePath.string());
    } else if (std::filesystem::is_directory(filePath)) {
      findJobFilesPath(filePath, result);
    }
  }
}

int main(){
    std::vector<std::string> files;
    std::filesystem::path directoryPath = "/home/kkiiim/Kiim/Cpp20-Learn/mutex";
    findJobFilesPath(directoryPath, files);
    for(auto&i:files){
        std::cout << i << '\n';
    }
}