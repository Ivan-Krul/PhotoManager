#pragma once
#include "define.h"
#include "Date.h"
#include "Branch.h"

struct Status {
  std::string input;
  Branch branch;
  Date date = cNullDate;
};
