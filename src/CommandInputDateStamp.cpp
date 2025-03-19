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
  if (mExceptionTriggered = inputMinutes()) return;
  if (status.input.size() < 2) return;
  if (mExceptionTriggered = inputSeconds()) return;
}

bool CommandInputDateStamp::setNullDateAspectWhenX(short x_count, DateAspect date_aspect, char sep) {
  short x_iters = 0;
  for (x_iters; x_iters < x_count; x_iters++) {
    if (mBuffer[x_iters] != 'X') break;
  }
  if (x_iters != x_count) return false;

  switch (date_aspect) {
  case DateAspect::Year:    sStatus.date.year    = cNullDate.year;    break;
  case DateAspect::Month:   sStatus.date.month   = cNullDate.month;   break;
  case DateAspect::Day:     sStatus.date.day     = cNullDate.day;     break;
  case DateAspect::Hours:   sStatus.date.hours   = cNullDate.hours;   break;
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

bool CommandInputDateStamp::inputDateElement(DateAspect aspect, char separator, const char* element_name, std::function<bool(short)> cond) {
  short elem;
  extractIntoBuffer(separator);

  if (setNullDateAspectWhenX(2 + ((aspect == DateAspect::Year) * 2), aspect, separator)) return false;

  try {
    elem = std::stoi(mBuffer);
  } catch (std::exception& e) {
    std::cout << "Invalid " << element_name << " input: " << e.what() << '\n';
    return true;
  }

  if (cond(elem)) {
    std::cout << "The " << element_name << " is invalid\n";
    return true;
  }

  switch (aspect) {
  case DateAspect::Year: sStatus.date.year = elem; break;
  case DateAspect::Month: sStatus.date.month = elem; break;
  case DateAspect::Day: sStatus.date.day = elem; break;
  case DateAspect::Hours: sStatus.date.hours = elem; break;
  case DateAspect::Minutes: sStatus.date.minutes = elem; break;
  case DateAspect::Seconds: sStatus.date.seconds = elem; break;
  }
  
  cleanFollowingPadding(separator);

  return false;
}

bool CommandInputDateStamp::inputYear() {
  return inputDateElement(
    DateAspect::Year,
    '.',
    "year",
    [=](short num) {
      bool similar = false;
      for (size_t i = 0; i < sStatus.branch.years.size(); i++) {
        similar = sStatus.branch.years[i] == num;
        if (similar) break;
      }
      return !similar;
    }
  );
}

bool CommandInputDateStamp::inputMonth() {
  return inputDateElement(
    DateAspect::Month,
    '.',
    "month",
    [=](short num) { return num && num > 12; }
  );
}

bool CommandInputDateStamp::inputDay() {
  return inputDateElement(
    DateAspect::Day,
    ' ',
    "day",
    [=](short num) { return num == 0 || (num > calculateDaysInMonth() && calculateDaysInMonth() != 0); }
  );
}

bool CommandInputDateStamp::inputHours() {
  return inputDateElement(
    DateAspect::Hours,
    ':',
    "hours",
    [=](short num) { return num > 23; }
  );
}

bool CommandInputDateStamp::inputMinutes() {
  return inputDateElement(
    DateAspect::Minutes,
    ':',
    "minutes",
    [=](short num) { return num > 59; }
  );
}

bool CommandInputDateStamp::inputSeconds() {
  return inputDateElement(
    DateAspect::Seconds,
    ':',
    "seconds",
    [=](short num) { return num > 59; }
  );
}
#pragma endregion

