#include "CommandInputDateStamp.h"

CommandInputDateStamp::CommandInputDateStamp(Status& status) : CommandBase(status) {
  if (status.input.size() < 4) {
    std::cout << "Nothing to process\n";
    return;
  }
  if (mExceptionTriggered = inputYear()) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputMonth()) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputDay()) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputHours()) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputSexagesimal(DateAspect::Minutes)) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputSexagesimal(DateAspect::Seconds)) return;
}

bool CommandInputDateStamp::setNullDateAspectWhenX(short x_count, DateAspect date_aspect, char sep) {
  short x_iters = 0;
  for (x_iters; x_iters < x_count; x_iters++) {
    if (mBuffer[x_iters] != 'X') break;
  }
  if (x_iters != x_count) return false;

  switch (date_aspect) {
  case DateAspect::Year:    sStatus.date.year = cNullDate.year; break;
  case DateAspect::Month:   sStatus.date.month = cNullDate.month; break;
  case DateAspect::Day:     sStatus.date.day = cNullDate.day; break;
  case DateAspect::Hours:   sStatus.date.hours = cNullDate.hours; break;
  case DateAspect::Minutes: sStatus.date.minutes = cNullDate.minutes; break;
  case DateAspect::Seconds: sStatus.date.seconds = cNullDate.seconds; break;
  }
  cleanFollowingPadding(sep);
  return true;
}

short CommandInputDateStamp::calculateDaysInMonth() {
  if (sStatus.date.month == cNullDate.month) return 0;

  switch (sStatus.date.month) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
  default:
    return 31;
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  case 2:
    if (sStatus.date.year == cNullDate.year) return 28;
    if (sStatus.date.year % 400 == 0) return 29;
    if (sStatus.date.year % 100 == 0) return 28;
    return 28 + (sStatus.date.year % 4 == 0);
  }
}

void CommandInputDateStamp::extractIntoBuffer(char sep) {
  short ind = sStatus.input.find(sep);
  if (ind == -1) {
    mBuffer = sStatus.input;
    sStatus.input.clear();
  } else {
    mBuffer = sStatus.input.substr(0, ind);
    sStatus.input.erase(0, ind);
  }
}

void CommandInputDateStamp::cleanFollowingPadding(char sep) {
  short ind = sStatus.input.find_first_not_of(sep);
  if (ind == -1) {
    sStatus.input.clear();
    return;
  }
  sStatus.input.erase(0, ind);
}

#pragma region inputs for each element of the date in the class
bool CommandInputDateStamp::inputYear() {
  short year = 0;
  extractIntoBuffer('.');

  if (setNullDateAspectWhenX(4, DateAspect::Year, '.')) return false;

  try {
    year = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid year input: " << e.what() << '\n';
    return true;
  }

  bool similar = false;
  for (size_t i = 0; i < sStatus.branch.years.size(); i++) {
    similar = sStatus.branch.years[i] == year;
    if (similar) break;
  }

  if (!similar) {
    std::cout << "The year isn't not represented with folders\n";
    return true;
  }
  sStatus.date.year = year;
  cleanFollowingPadding('.');

  return false;
}

bool CommandInputDateStamp::inputMonth() {
  short month;
  extractIntoBuffer('.');

  if (setNullDateAspectWhenX(2, DateAspect::Month, '.')) return false;

  try {
    month = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid month input: " << e.what() << '\n';
    return true;
  }

  if (month && month > 12) {
    std::cout << "The month is invalid\n";
    return true;
  }
  sStatus.date.month = month;
  cleanFollowingPadding('.');

  return false;
}

bool CommandInputDateStamp::inputDay() {
  short day;
  extractIntoBuffer(' ');

  if (setNullDateAspectWhenX(2, DateAspect::Day, ' ')) return false;

  try {
    day = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid day input: " << e.what() << '\n';
    return true;
  }

  if (day == 0 || (day > calculateDaysInMonth() && calculateDaysInMonth() != 0)) {
    std::cout << "The day is invalid\n";
    return true;
  }
  sStatus.date.day = day;
  cleanFollowingPadding(' ');

  return false;
}

bool CommandInputDateStamp::inputHours() {
  short hours;
  extractIntoBuffer(':');

  if (setNullDateAspectWhenX(2, DateAspect::Hours, ':')) return false;

  try {
    hours = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid hours input: " << e.what() << '\n';
    return true;
  }

  if (hours == 0 || hours > 23) {
    std::cout << "The hours is invalid\n";
    return true;
  }
  sStatus.date.hours = hours;
  cleanFollowingPadding(':');

  return false;
}

bool CommandInputDateStamp::inputSexagesimal(DateAspect min_or_sec) {
  short num;
  extractIntoBuffer(':');

  if (setNullDateAspectWhenX(2, min_or_sec, ':')) return false;

  try {
    num = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid " << (min_or_sec == DateAspect::Minutes ? "minutes" : "seconds") << " input: " << e.what() << '\n';
    return true;
  }

  if (num == 0 || num > 60) {
    std::cout << "The " << (min_or_sec == DateAspect::Minutes ? "minutes" : "seconds") << " is invalid\n";
    return true;
  }
  (min_or_sec == DateAspect::Minutes ? sStatus.date.minutes : sStatus.date.seconds) = num;
  cleanFollowingPadding(':');

  return false;
}
#pragma endregion

