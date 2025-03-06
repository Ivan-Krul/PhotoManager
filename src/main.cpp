#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>

using Path = std::filesystem::path;

struct Date {
  short year;
  unsigned short month : 4;
  unsigned short day : 5;
  unsigned short hours : 5;
  unsigned short minutes : 6;
  unsigned short seconds : 6;
  
  //unsigned short frac_sec; // (1 / 2^16)
};

enum class DateAspect {
  Year, Month, Day, Hours, Minutes, Seconds
};

constexpr Date mNullDate = { std::numeric_limits<short>::min(), 0,0,0xff,0xff,0xff };

struct Branch {
  Path path;
  std::vector<short> years;
  bool has_research = false;
  bool has_buffer_dir = false;
};

Branch initDirList(Path path) {
  Branch branch;
  branch.path = path;

  for (const auto& item : std::filesystem::directory_iterator(path)) {
    if (!item.is_directory()) continue;

    if (isdigit(item.path().filename().string()[0]))
      branch.years.push_back(std::stoi(item.path().filename().string()));
    else if (item.path().filename().string() == ".research")
      branch.has_research = true;
    else if (item.path().filename().string() == "buffer")
      branch.has_buffer_dir = true;
  }

  return branch;
}

struct Status {
  std::string input;
  Branch branch;
  Date date = mNullDate;
};

class CommandInputDateStamp {
public:
  CommandInputDateStamp(Status& status) : sStatus(status) {
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

  bool isExceptionTriggered() const { return mExceptionTriggered; }

private:
  bool setNullDateAspectWhenX(short x_count, DateAspect date_aspect) {
    short x_iters = 0;
    for (x_iters; x_iters < x_count; x_iters++) {
      if (mBuffer[x_iters] != 'X') break;
    }
    if (x_iters != x_count) return false;

    switch (date_aspect) {
    case DateAspect::Year:    sStatus.date.year    = mNullDate.year   ; break;
    case DateAspect::Month:   sStatus.date.month   = mNullDate.month  ; break;
    case DateAspect::Day:     sStatus.date.day     = mNullDate.day    ; break;
    case DateAspect::Hours:   sStatus.date.hours   = mNullDate.hours  ; break;
    case DateAspect::Minutes: sStatus.date.minutes = mNullDate.minutes; break;
    case DateAspect::Seconds: sStatus.date.seconds = mNullDate.seconds; break;
    }
    cleanFollowingPadding();
    return true;
  } 

  short calculateDaysInMonth() {
    if (sStatus.date.month == mNullDate.month) return 0;

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
      if (sStatus.date.year == mNullDate.year) return 28;
      if (sStatus.date.year % 400 == 0) return 29;
      if (sStatus.date.year % 100 == 0) return 28;
      return 28 + (sStatus.date.year % 4 == 0);
    }
  }

  void extractIntoBuffer(char sep = ' ') {
    short ind = sStatus.input.find(sep);
    if (ind == -1) {
      mBuffer = sStatus.input;
      sStatus.input.clear();
    }
    else {
      mBuffer = sStatus.input.substr(0, ind);
      sStatus.input.erase(0, ind);
    }
  }

  void cleanFollowingPadding() {
    short ind = sStatus.input.find_first_not_of(' ');
    if (ind == -1) {
      sStatus.input.clear();
      return;
    }
    sStatus.input.erase(0, ind);
  }

  bool inputYear() {
    short year = 0;
    extractIntoBuffer();

    if (setNullDateAspectWhenX(4, DateAspect::Year)) return false;

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
    cleanFollowingPadding();

    return false;
  }

  bool inputMonth() {
    short month;
    extractIntoBuffer();
    
    if (setNullDateAspectWhenX(2, DateAspect::Month)) return false;

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
    cleanFollowingPadding();

    return false;
  }

  bool inputDay() {
    short day;
    extractIntoBuffer();

    if (setNullDateAspectWhenX(2, DateAspect::Day)) return false;

    try {
      day = std::stoi(mBuffer);
    } catch (std::exception& e) {
      std::cout << "Invalid day input: " << e.what() << '\n';
      return true;
    }

    if (day == 0 || day > calculateDaysInMonth()) {
      std::cout << "The day is invalid\n";
      return true;
    }
    sStatus.date.day = day;
    cleanFollowingPadding();

    return false;
  }

  bool inputHours() {
    short hours;
    extractIntoBuffer();

    if (setNullDateAspectWhenX(2, DateAspect::Hours)) return false;

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
    cleanFollowingPadding();

    return false;
  }

  bool inputSexagesimal(DateAspect min_or_sec) {
    short num;
    extractIntoBuffer();

    if (setNullDateAspectWhenX(2, min_or_sec)) return false;

    try {
      num = std::stoi(mBuffer);
    } catch (std::exception& e) {
      std::cout << "Invalid "<< (min_or_sec == DateAspect::Minutes ? "minutes" : "seconds") <<" input: " << e.what() << '\n';
      return true;
    }

    if (num == 0 || num > 60) {
      std::cout << "The " << (min_or_sec == DateAspect::Minutes ? "minutes" : "seconds") << " is invalid\n";
      return true;
    }
    (min_or_sec == DateAspect::Minutes ? sStatus.date.minutes : sStatus.date.seconds) = num;
    cleanFollowingPadding();

    return false;
  }

private:
  Status& sStatus;
  

  std::string mBuffer;
  bool mExceptionTriggered = false;

};


std::ostream& operator<<(std::ostream& out, Date date) {
  out << std::setfill('0');
  if (date.year == mNullDate.year) out << "XXXX";
  else out << std::setw(4) << date.year;

  out  << '.';

  if (date.month == mNullDate.month) out << "XX";
  else out << std::setw(2) << date.month;

  out << '.';

  if (date.day == mNullDate.day) out << "XX";
  else out << std::setw(2) << date.day;

  out << ' ';

  if (date.hours == mNullDate.hours) out << "XX";
  else out << std::setw(2) << date.hours;

  out << ':';

  if (date.minutes == mNullDate.minutes) out << "XX";
  else out << std::setw(2) << date.minutes;

  out << ':';

  if (date.seconds == mNullDate.seconds) out << "XX";
  else out << std::setw(2) << date.seconds;

  return out;
}


int main(int args, char* argv[]) {
  Status status = {};

  if (args < 2) {
    std::string manual_path;
    std::cout << "Input your desired path: ";
    std::getline(std::cin, manual_path);
    status.branch.path = manual_path;
  } else
    status.branch.path = argv[1];

  status.branch = initDirList(status.branch.path);

  for (size_t y = 0; y < status.branch.years.size(); y++) {
    if (y == 0) std::cout << "Years:";
    std::cout << '\t' << status.branch.years[y];
    if (y + 1 != status.branch.years.size()) std::cout << ',';
  }
  std::cout << '\n';

  while (status.input != "exit") {
    std::cout << "["<<status.date<<"]: ";
    std::getline(std::cin, status.input);

    if (status.input.substr(0,4) == "exit") continue;

    if (status.input.substr(0, 10) == "date stamp") {
      status.input.erase(0, status.input.find_first_not_of(' ', 10));
      CommandInputDateStamp cmd(status);
      if (cmd.isExceptionTriggered()) {
        std::cout << "exception has invoked!\n";
      }
    }
  }

  std::cout << "exiting... \n";
}
