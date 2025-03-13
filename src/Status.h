#pragma once
#include "Date.h"
#include "Branch.h"

struct Status {
  std::string input;
  Branch branch;
  Date date = cNullDate;
};
