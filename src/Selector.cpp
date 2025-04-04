#include "Selector.h"

#include <exception>
#include <algorithm>
#include <iostream>

Selector& Selector::getPaths() {
  auto items = RecurseItemIterator(sStatus.branch.path);
  for (auto const& item : items)
    mItems.emplace_back(item);
  return *this;
}

Selector& Selector::getPaths(short year) {
  auto items = RecurseItemIterator(sStatus.branch.path / std::to_string(year));
  for (auto const& item : items)
    mItems.emplace_back(item);
  return *this;
}

Selector& Selector::getPaths(const Path& path) {
  auto items = RecurseItemIterator(sStatus.branch.path / path);
  for (auto const& item : items)
    mItems.emplace_back(item);
  return *this;
}

Selector& Selector::filterFiles() {
  std::remove_if(mItems.begin(), mItems.end(), [=](const Item& item) { return !item.is_directory(); });
  return *this;
}

Selector& Selector::filterDirectories() {
  std::remove_if(mItems.begin(), mItems.end(), [=](const Item& item) { return item.is_directory(); });
  return *this;
}

Selector& Selector::filterByDate() {
  mEmplaceDateStatus.emplace(sStatus.date);

  std::vector<Item> items;
  items.reserve(mItems.size() / 2);

  for (const auto& item : mItems) {
    if (isDateRelevant(item)) items.emplace_back(item);
  }

  mItems = items;

  mItems.shrink_to_fit();
  return *this;
}

Selector& Selector::filterByExtension() {
  getItemsInList();

  if (sStatus.input[0] != '.') sStatus.input.insert(sStatus.input.begin(), '.');
  
  std::vector<Item> items;
  items.reserve(mItems.size() / 2);

  for (const auto& item : mItems) {
    if (item.path().extension().string() == sStatus.input) items.emplace_back(item);
  }

  mItems = items;

  mItems.shrink_to_fit();
  return *this;
}

void Selector::getItemsInList(ExtractIteratorMode eim) {
  auto items = extractDirectory(eim);
  for (auto const& item : items) {
    mItems.emplace_back(item);
  }
}

bool Selector::isDateRelevant(const Item& item) {
  if (item.is_directory()) return false;
  sStatus.input = item.path().filename().string();

  size_t underline = sStatus.input.find('_');
  if (underline == -1) return false;

  char* ptr = const_cast<char*>(sStatus.input.c_str()) + underline + 1;

  if (mEmplaceDateStatus.year) {
    if (sStatus.date.year != convertStrToShort<4>(ptr)) return false;
  }
  if (mEmplaceDateStatus.month) {
    if (sStatus.date.month != convertStrToShort<2>(ptr + 4)) return false;
  }
  if (mEmplaceDateStatus.day) {
    if (sStatus.date.day != convertStrToShort<2>(ptr + 6)) return false;
  }
  if (mEmplaceDateStatus.hours) {
    if (sStatus.date.hours != convertStrToShort<2>(ptr + 9)) return false;
  }
  if (mEmplaceDateStatus.minutes) {
    if (sStatus.date.minutes != convertStrToShort<2>(ptr + 11)) return false;
  }
  if (mEmplaceDateStatus.seconds) {
    if (sStatus.date.seconds != convertStrToShort<2>(ptr + 13)) return false;
  }
  return true;
}

RecurseItemIterator Selector::extractDirectory(ExtractIteratorMode eim) {
  switch (eim) {
  case YEAR:
    return RecurseItemIterator(sStatus.branch.path / std::to_string(sStatus.date.year));
  case YEAR_MONTH:
    return RecurseItemIterator(sStatus.branch.path / std::to_string(sStatus.date.year) / std::to_string(sStatus.date.month));
  case DEFAULT:
  default:
    return RecurseItemIterator(sStatus.branch.path);
  }
}
