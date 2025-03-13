#pragma once
#include <iostream>
#include "Branch.h"

Branch initDirList(Path path);

int init(int args, char* argv[], Status& status) {
#ifdef DEBUG
  status.branch.path = Path("D:/Cellphone");
  status.date.year = 2024;
  status.date.month = 5;
#else
  if (args < 2) {
    std::string manual_path;
    std::cout << "Input your desired path: ";
    std::getline(std::cin, manual_path);
    status.branch.path = manual_path;
    
  } else
    status.branch.path = argv[1];
#endif

  status.branch = initDirList(status.branch.path);

 for (size_t y = 0; y < status.branch.years.size(); y++) {
   if (y == 0) std::cout << "Years:";
   std::cout << '\t' << status.branch.years[y];
   if (y + 1 != status.branch.years.size()) std::cout << ',';
   
 }
  std::cout << '\n';

  return 0;
}

Branch initDirList(Path path) {
  Branch branch;
  branch.path = path;

  std::filesystem::directory_iterator items;

  try {
    items = std::filesystem::directory_iterator(path);
  } catch (std::exception& e) {
    std::cout << "exception with input access\n";
    std::cout << e.what() << '\n';
    exit(1);
  }

  for (const auto& item : items) {
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
