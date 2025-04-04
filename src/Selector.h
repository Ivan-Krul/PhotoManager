#pragma once
#include <string>

#include "Status.h"
  
using RecurseItemIterator = std::filesystem::recursive_directory_iterator;
using Item = std::filesystem::directory_entry;

class Selector {
private:
  enum ExtractIteratorMode {
    DEFAULT,
    YEAR,
    YEAR_MONTH
  };
public:
  inline Selector(Status& status) : sStatus(status) {}

  Selector& getPaths();
  Selector& getPaths(short year);
  Selector& getPaths(const Path& path);
  Selector& filterFiles();
  Selector& filterDirectories();
  Selector& filterByDate();
  Selector& filterByExtension();

  inline size_t size() const { return mItems.size(); }
  inline const Path operator[](size_t index) const { return mItems.at(index).path(); }

  inline std::vector<Item>::iterator begin() { return mItems.begin(); }
  inline std::vector<Item>::iterator end() { return mItems.end(); }

private:
  template<short StrLen>
  typename std::enable_if<StrLen == sizeof(short), short>::type convertStrToShort(const char* chr) {
    short numstr = *(short*)(chr);
    return ((numstr & 0x0f00) >> 8) + (numstr & 0x0f) * 10;
  }

  template<short StrLen>
  typename std::enable_if<StrLen == sizeof(int), short>::type convertStrToShort(const char* chr) {
    int numstr = *(int*)(chr);
    return ((numstr >> 24) & 0x0f)
      + ((numstr >> 16) & 0x0f) * 10
      + ((numstr >> 8) & 0x0f) * 100
      + (numstr & 0x0f) * 1000;
  }

  void getItemsInList(ExtractIteratorMode eim = DEFAULT);

  bool isDateRelevant(const Item& item);

  RecurseItemIterator extractDirectory(ExtractIteratorMode eim);

private:
  std::vector<Item> mItems;
  Status& sStatus;
  EmplacedStatus mEmplaceDateStatus;
};

