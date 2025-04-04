#pragma once
#include <limits>

struct Date {
  short year;
  unsigned short month : 4;
  unsigned short day : 5;
  unsigned short hours : 5;
  unsigned short minutes : 6;
  unsigned short seconds : 6;

  //unsigned short frac_sec; // (1 / 2^16)
};

constexpr Date cNullDate = { std::numeric_limits<short>::min(), 0,0,0xff,0xff,0xff };

struct EmplacedStatus {
  inline EmplacedStatus() = default;

  inline void emplace(Date date) {
    year = date.year != cNullDate.year;
    month = date.month != cNullDate.month;
    day = date.day != cNullDate.day;
    hours = date.hours != cNullDate.hours;
    minutes = date.minutes != cNullDate.minutes;
    seconds = date.seconds != cNullDate.seconds;
  }

  inline EmplacedStatus(Date date) {
    emplace(date);
  }

  bool year : 1;
  bool month : 1;
  bool day : 1;
  bool hours : 1;
  bool minutes : 1;
  bool seconds : 1;
};

enum class DateAspect {
  Year, Month, Day, Hours, Minutes, Seconds
};

