#pragma once
#include <iostream>
#include <string>
#include <functional>

#include "CommandBase.h"

class CommandInputDateStamp : public CommandBase {
public:
  CommandInputDateStamp(Status& status);

  bool a(short b);
private:
  bool setNullDateAspectWhenX(short x_count, DateAspect date_aspect, char sep);
  short calculateDaysInMonth();

  void extractIntoBuffer(char sep = ' ');
  void cleanFollowingPadding(char sep = ' ');

  bool inputDateElement(DateAspect aspect, char separator, const char* element_name, std::function<bool(short)> cond);


  inline bool inputYear();
  inline bool inputMonth();
  inline bool inputDay();
  inline bool inputHours();
  inline bool inputMinutes();
  inline bool inputSeconds();

private:
  std::string mBuffer;

};

