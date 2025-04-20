#pragma once
#include "Status.h"

class StatusManager {
public:
  Init(const char* path);

  inline Date GetSelectedDate() const { return mStatus.date; }
  

private:
  Status mStatus;
};

