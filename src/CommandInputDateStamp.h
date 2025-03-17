#pragma once
#include <iostream>
#include <string>

#include "CommandBase.h"

class CommandInputDateStamp : public CommandBase {
public:
  CommandInputDateStamp(Status& status);

private:
  bool setNullDateAspectWhenX(short x_count, DateAspect date_aspect, char sep);
  short calculateDaysInMonth();

  void extractIntoBuffer(char sep = ' ');
  void cleanFollowingPadding(char sep = ' ');

  bool inputYear();
  bool inputMonth();
  bool inputDay();
  bool inputHours();
  bool inputSexagesimal(DateAspect min_or_sec);

private:
  std::string mBuffer;

};

