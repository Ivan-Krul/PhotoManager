#include <iostream>
#include <filesystem>
#include <string>

int main(int args, char* argv[]) {
  
  if (args < 2) {
    std::string manual_path;
    std::cout << "Input your desired path: ";
    std::getline(std::cin, manual_path);
  }
  std::filesystem::directory_entry dir;
}
