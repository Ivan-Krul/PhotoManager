#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>

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

constexpr Date cNullDate = { std::numeric_limits<short>::min(), 0,0,0xff,0xff,0xff };

struct Branch {
  Path path;
  std::vector<short> years;
  bool has_research = false;
  bool has_buffer_dir = false;
};

Branch initDirList(Path path) {
  Branch branch;
  branch.path = path;

  std::filesystem::directory_iterator items;

  try {
    items = std::filesystem::directory_iterator(path);
  }
  catch (std::exception& e) {
    std::cout << "exception with input access\n";
    std::cout << e.what() << '\n';
    exit(1);
  }

  for (const auto& item : items) {
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
  Date date = cNullDate;
};

class CommandBase {
public:
  CommandBase(Status& status) : sStatus(status) {}

  bool isExceptionTriggered() const { return mExceptionTriggered; }

protected:
  Status& sStatus;

  std::string mBuffer;
  bool mExceptionTriggered = false;
};

class CommandInputDateStamp : public CommandBase {
public:
  CommandInputDateStamp(Status& status) : CommandBase(status) {
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

private:
  bool setNullDateAspectWhenX(short x_count, DateAspect date_aspect, char sep) {
    short x_iters = 0;
    for (x_iters; x_iters < x_count; x_iters++) {
      if (mBuffer[x_iters] != 'X') break;
    }
    if (x_iters != x_count) return false;

    switch (date_aspect) {
    case DateAspect::Year:    sStatus.date.year    = cNullDate.year   ; break;
    case DateAspect::Month:   sStatus.date.month   = cNullDate.month  ; break;
    case DateAspect::Day:     sStatus.date.day     = cNullDate.day    ; break;
    case DateAspect::Hours:   sStatus.date.hours   = cNullDate.hours  ; break;
    case DateAspect::Minutes: sStatus.date.minutes = cNullDate.minutes; break;
    case DateAspect::Seconds: sStatus.date.seconds = cNullDate.seconds; break;
    }
    cleanFollowingPadding(sep);
    return true;
  } 

  short calculateDaysInMonth() {
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

  void cleanFollowingPadding(char sep = ' ') {
    short ind = sStatus.input.find_first_not_of(sep);
    if (ind == -1) {
      sStatus.input.clear();
      return;
    }
    sStatus.input.erase(0, ind);
  }

#pragma region inputs for each element of the date in the class
  bool inputYear() {
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

  bool inputMonth() {
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

  bool inputDay() {
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

  bool inputHours() {
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

  bool inputSexagesimal(DateAspect min_or_sec) {
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

};

class Selector {
public:
  inline Selector(Status& status) : sStatus(status) {}

  size_t filterByDate() {
    std::filesystem::recursive_directory_iterator items;
    if (sStatus.date.year != cNullDate.year) {
      if (sStatus.date.month != cNullDate.month)
        items = std::filesystem::recursive_directory_iterator(sStatus.branch.path / std::to_string(sStatus.date.year) / std::to_string(sStatus.date.month));
      else
        items = std::filesystem::recursive_directory_iterator(sStatus.branch.path / std::to_string(sStatus.date.year));
    }
    else
      items = std::filesystem::recursive_directory_iterator(sStatus.branch.path);

    std::string buf;

    for (auto const& item : items) {
      if (item.is_directory()) continue;
      buf = item.path().filename().string();
      try {
        buf = buf.substr(buf.find('_') + 5, 11);
      
        if (sStatus.date.month != cNullDate.month) {
          if (sStatus.date.month != std::stoi(buf.substr(0, 2))) continue;
        }
        if (sStatus.date.day != cNullDate.day) {
          if (sStatus.date.day != std::stoi(buf.substr(2, 2))) continue;
        }
        if (sStatus.date.hours != cNullDate.hours) {
          if (sStatus.date.hours != std::stoi(buf.substr(5, 2))) continue;
        }
        if (sStatus.date.minutes != cNullDate.minutes) {
          if (sStatus.date.minutes != std::stoi(buf.substr(7, 2))) continue;
        }
        if (sStatus.date.seconds != cNullDate.seconds) {
          if (sStatus.date.seconds != std::stoi(buf.substr(9, 2))) continue;
        }
      }
      catch (...) {
        // error of conversion could result of other files
        continue;
      }

      mPaths.emplace_back(item.path());
    }

    mPaths.shrink_to_fit();
    return mPaths.size();
  }

  size_t filterByExtension() {
    auto items = std::filesystem::recursive_directory_iterator(sStatus.branch.path);

    for (auto const& item : items) {
      if(
      if (item.path().extension().string() == sStatus.input)
        mPaths.emplace_back(item.path());
    }

    mPaths.shrink_to_fit();
    return mPaths.size();
  }

  inline size_t size() const { return mPaths.size(); }
  inline const Path operator[](size_t index) const {
    return mPaths.at(index);
  }

  inline std::vector<Path>::iterator begin() { return mPaths.begin(); }
  inline std::vector<Path>::iterator end() { return mPaths.end(); }

private:
  std::vector<Path> mPaths;
  Status& sStatus;
};

class CommandCompressWebp : public CommandBase {
public:
  CommandCompressWebp(Status& status) : CommandBase(status) {
    mPath = status.branch.path / std::to_string(status.date.year);
  }

private:

  Path mPath;
};

#pragma region operators of overloading for std::ostream
std::ostream& operator<<(std::ostream& out, Date date) {
  out << std::setfill('0');
  if (date.year == cNullDate.year) out << "XXXX";
  else out << std::setw(4) << date.year;

  out << '.';

  if (date.month == cNullDate.month) out << "XX";
  else out << std::setw(2) << date.month;

  out << '.';

  if (date.day == cNullDate.day) out << "XX";
  else out << std::setw(2) << date.day;

  out << ' ';

  if (date.hours == cNullDate.hours) out << "XX";
  else out << std::setw(2) << date.hours;

  out << ':';

  if (date.minutes == cNullDate.minutes) out << "XX";
  else out << std::setw(2) << date.minutes;

  out << ':';

  if (date.seconds == cNullDate.seconds) out << "XX";
  else out << std::setw(2) << date.seconds;

  return out;
}
#pragma endregion

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

  auto begt = std::chrono::steady_clock::now();

  while (status.input != "exit") {
    std::cout << "["<<status.date<<"]: ";
    std::getline(std::cin, status.input);

    if (status.input.substr(0,4) == "exit") continue;

    begt = std::chrono::steady_clock::now();

    if (status.input.substr(0, 10) == "date stamp") {
      status.input.erase(0, status.input.find_first_not_of(' ', 10));
      CommandInputDateStamp cmd(status);
      if (cmd.isExceptionTriggered())
        std::cout << "exception has invoked!\n";
    }
    else if (status.input.substr(0, 14) == "list by dates") {
      status.input.erase(0, status.input.find_first_not_of(' ', 11));
      Selector s(status);
      std::cout << "There's " << s.filterByDate() << " elements\n";;
      for (auto& item : s) {
        std::cout <<'\t' <<item.filename().string() << '\n';
      }
    }
    else if (status.input.substr(0, 18) == "list by extension") { // fix there, it accepts only no extension
      status.input.erase(0, status.input.find_first_not_of(' ', 18));
      Selector s(status);
      std::cout << "There's " << s.filterByExtension() << " elements\n";;
      for (auto& item : s) {
        std::cout << '\t' << item.filename().string() << '\n';
      }
    }
    else if (status.input.substr(0, 21) == "compress images webp") {
      status.input.erase(0, status.input.find_first_not_of(' ', 21));
      CommandCompressWebp cmd(status);
      if (cmd.isExceptionTriggered())
        std::cout << "exception has invoked!\n";
    }
    
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begt).count() / 1000.f << "ms\n";
  }

  std::cout << "exiting... \n";
}
