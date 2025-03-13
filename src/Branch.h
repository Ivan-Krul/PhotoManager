#pragma once
#include <vector>
#include <filesystem>

using Path = std::filesystem::path;

struct Branch {
  Path path;
  std::vector<short> years;
  bool has_research = false;
  bool has_buffer_dir = false;
};
