#include "Selector.h"

size_t Selector::filterByDate() {
  std::filesystem::recursive_directory_iterator items;

  EmplacedStatus es(sStatus.date);

  if (es.year) {
    if (es.month)
      items = std::filesystem::recursive_directory_iterator(sStatus.branch.path / std::to_string(sStatus.date.year) / std::to_string(sStatus.date.month));
    else
      items = std::filesystem::recursive_directory_iterator(sStatus.branch.path / std::to_string(sStatus.date.year));
  } else
    items = std::filesystem::recursive_directory_iterator(sStatus.branch.path);

  std::string buf;
  size_t underline = 0;

  char* ptr;

  for (auto const& item : items) {
    if (item.is_directory()) continue;
    buf = item.path().filename().string();

    underline = buf.find('_');
    if (underline == -1) continue;

    ptr = const_cast<char*>(buf.c_str()) + underline + 1;

    try {
      if (es.year) {
        if (sStatus.date.year != convertStrToShort<4>(ptr)) continue;
      }
      if (es.month) {
        if (sStatus.date.month != convertStrToShort<2>(ptr + 4)) continue;
      }
      if (es.day) {
        if (sStatus.date.day != convertStrToShort<2>(ptr + 6)) continue;
      }
      if (es.hours) {
        if (sStatus.date.hours != convertStrToShort<2>(ptr + 9)) continue;
      }
      if (es.minutes) {
        if (sStatus.date.minutes != convertStrToShort<2>(ptr + 11)) continue;
      }
      if (es.seconds) {
        if (sStatus.date.seconds != convertStrToShort<2>(ptr + 13)) continue;
      }
    } catch (...) {
      // error of conversion could result of other files
      continue;
    }

    mPaths.emplace_back(item.path());
  }

  mPaths.shrink_to_fit();
  return mPaths.size();
}

size_t Selector::filterByExtension() {
  auto items = std::filesystem::recursive_directory_iterator(sStatus.branch.path);

  for (auto const& item : items) {
    if (item.path().extension().string() == sStatus.input)
      mPaths.emplace_back(item.path());
  }

  mPaths.shrink_to_fit();
  return mPaths.size();
}
