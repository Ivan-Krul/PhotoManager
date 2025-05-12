#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

using RecurseItemIterator = std::filesystem::recursive_directory_iterator;
using ItemIterator = std::filesystem::directory_iterator;
using Item = std::filesystem::directory_entry;
using Path = std::filesystem::path;

inline std::vector<Item> getChildItem(const Path& path_of_dir) {
  std::vector<Item> items;
  items.reserve(16);

  ItemIterator iterator(path_of_dir);

  for (const auto item : iterator) {
    items.emplace_back(item);
  }

  return items;
}

inline std::vector<Item> getChildRecursiveItem(const Path& path_of_dir) {
  std::vector<Item> items;
  items.reserve(32);

  RecurseItemIterator iterator(path_of_dir);

  for (const auto item : iterator) {
    items.emplace_back(item);
  }

  return items;
}

inline std::string anchorNumber(const std::string& filepath) {
  // search for the longest sequence of digits
  size_t max_streaks = 0;
  size_t max_in_index = -1;

  size_t streaks = 0;
  size_t start_current_streak = -1;

  for (size_t i = 0; i < filepath.size(); i++) {
    if (isdigit(filepath[i]) == 0) {
      start_current_streak = -1;
      streaks = 0;
      continue;
    }

    if (start_current_streak == -1)
      start_current_streak = i;

    streaks++;

    if (max_streaks < streaks) {
      max_in_index = start_current_streak;
      max_streaks = streaks;
    }
  }
  
  return max_streaks == 0 ? "" : filepath.substr(max_in_index, max_streaks);
}

inline void sortByNumber(std::vector<Item>& items) {
  std::vector<std::pair<int, int>> number_index;
  std::vector<Item> items_res;
  std::string inner_text;

  for (int i = 0; i < items.size(); i++) {
    inner_text = items[i].path().filename().string();
    inner_text = anchorNumber(inner_text);
    number_index.emplace_back(
      std::make_pair(
        (inner_text != "") ? std::stoi(inner_text) : 0x7fff - items.size() + i,
        i
      ));
  }

  std::sort(number_index.begin(), number_index.end(), [=](const std::pair<int, int>& first, const std::pair<int, int>& second){
    return first.first < second.first;
  });

  items_res.resize(items.size());
  for (int i = 0; i < items.size(); i++) {
    items_res[i] = items[number_index[i].second];
  }

  items = items_res;
}



