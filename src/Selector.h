#pragma once
#include <string>

#include "Status.h"

class Selector {
public:
  inline Selector(Status& status) : sStatus(status) {}

  size_t filterByDate();
  size_t filterByExtension();

  inline size_t size() const { return mPaths.size(); }
  inline const Path operator[](size_t index) const { return mPaths.at(index); }

  inline std::vector<Path>::iterator begin() { return mPaths.begin(); }
  inline std::vector<Path>::iterator end() { return mPaths.end(); }

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

private:
  std::vector<Path> mPaths;
  Status& sStatus;
};

