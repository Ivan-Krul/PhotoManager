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

class CommandInputDateStamp {
  //
};

// instead of bunch of commands, we should make it a class

void inputDateStampYear(std::string& reserved_input, Branch& branch, Date& date) {
  short year = 0;
  std::string ybuffer = reserved_input.erase(0, reserved_input.find(' '));

  if (ybuffer == "XXXX") {
    date.year = std::numeric_limits<short>::min();
  }

  try {
    year = std::stoi(ybuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid year input: " << e.what() << '\n';
    return;
  }

  bool similar = false;
  for (size_t i = 0; i < branch.years.size(); i++) {
    similar = branch.years[i] == year;
    if (similar) break;
  }

  if (!similar) {
    std::cout << "The year isn't existing\n";
    return;
  }
}

void inputDateStamp(std::string& reserved_input, Branch& branch, Date& date) {
  inputDateStampYear(reserved_input, branch, date);
  // todo there
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

    if (inp.substr(0,4) == "exit") continue;

    if (inp == "date stamp") {
      inp.erase(0, 11);
      inputDateStamp(inp, branch, desired_date);

    }
  }

  std::cout << "exiting... \n";
}
