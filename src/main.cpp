#include <iostream>
#include <string>
#include <chrono>

#include "Status.h"
#include "Selector.h"
#include "CommandInputDateStamp.h"
#include "CommandCompressWebp.h"
#include "OstreamOverloads.h"
#include "Init.h"

#ifndef CONSOLE
#include "Application.h"

DECLARE_APP(Application)
wxIMPLEMENT_APP(Application);

#else
int main(int args, char* argv[]) {
  Status status = {};

  if (init(args, argv, status)) return 1;

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
    else if (status.input.substr(0, 13) == "list by dates") {
      status.input.erase(0, status.input.find_first_not_of(' ', 11));
      Selector s(status);
      std::cout << "There's " << s.getPaths().filterByDate().size() << " elements\n";;
      if (s.size() < 20) {
        for (auto& item : s) {
          std::cout << '\t' << item.path().filename().string() << '\n';
        }
      }
    }
    else if (status.input.substr(0, 17) == "list by extension") { // fix there, it accepts only no extension
      status.input.erase(0, status.input.find_first_not_of(' ', 18));
      Selector s(status);
      std::cout << "There's " << s.getPaths().filterByExtension().size() << " elements\n";;
      if (s.size() < 20) {
        for (auto& item : s) {
          std::cout << '\t' << item.path().filename().string() << '\n';
        }
      }
    }
    else if (status.input.substr(0, 20) == "compress images webp") {
      status.input.erase(0, status.input.find_first_not_of(' ', 21));
      CommandCompressWebp cmd(status);
      if (cmd.isExceptionTriggered())
        std::cout << "exception has invoked!\n";
    }
    
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begt).count() / 1000.f << "ms\n";
  }

  std::cout << "exiting... \n";
}
#endif
