#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

using Path = std::filesystem::path;

struct Date {
  short year;
  short month : 4;
  short day : 5;
  short hours : 5;
  short minutes : 6;
  short seconds : 6;
  short is_there : 1;
};

struct Branch {
  Path path;
  std::vector<short> years;
  bool has_research = false;
  bool has_buffer_dir = false;
};

Branch initDirList(Path path) {
  Branch branch;
  branch.path = path;

  for (const auto& item : std::filesystem::directory_iterator(path)) {
    if (!item.is_directory()) continue;

    if (isdigit(item.path().filename().string()[0]))
      branch.years.push_back(std::stoi(item.path().filename().string()));
    else if (item.path().filename().string() == ".research")
      branch.has_research = true;
    else if (item.path().filename().string() == "buffer")
      branch.has_buffer_dir = true;
  }

  return branch;
}

int main(int args, char* argv[]) {
  Branch branch;
  Date desired_date = {0};

  if (args < 2) {
    std::string manual_path;
    std::cout << "Input your desired path: ";
    std::getline(std::cin, manual_path);
    branch.path = manual_path;
  } else
    branch.path = argv[1];

  branch = initDirList(branch.path);

  std::string inp;

  while (inp != "exit") {
    std::cout << "[]: ";
    std::getline(std::cin, inp);

    if (inp == "exit") continue;

    if (inp == "goin") {
      inp.erase(0, 5);
      short year = std::stoi(inp.erase(0, inp.find(' ')));

      // todo there

    }
    

  }

  std::cout << "exiting... \n";
}
