#include "Selector.h"

Selector& Selector::filterByDate() {
  EmplacedStatus es(sStatus.date);
  RecurseItemIterator items = extractDirectory((es.year) ? ((es.month) ? YEAR_MONTH : YEAR) : DEFAULT);

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
  return *this;
}

Selector& Selector::filterByExtension() {
  auto items = extractDirectory(DEFAULT);

  for (auto const& item : items) {
    if (item.path().extension().string() == sStatus.input)
      mPaths.emplace_back(item.path());
  }

  mPaths.shrink_to_fit();
  return *this;
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
