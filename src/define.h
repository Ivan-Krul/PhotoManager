#pragma once
#include <string>
#include <cassert>

#define NOT_IMPLEMENTED(function) (throw (std::string("not implemented ") + function).c_str())

#pragma warning(disable : 4996)

//#define CONSOLE
