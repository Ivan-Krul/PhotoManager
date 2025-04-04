#pragma once
#include <string>

#define NOT_IMPLEMENTED(function) (throw (std::string("not implemented ") + function).c_str())
