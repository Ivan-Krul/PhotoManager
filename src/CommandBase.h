#pragma once
#include "Status.h"

class CommandBase {
public:
  CommandBase(Status& status) : sStatus(status) {}

  bool isExceptionTriggered() const { return mExceptionTriggered; }

protected:
  Status& sStatus;

  std::string mBuffer;
  bool mExceptionTriggered = false;
};

