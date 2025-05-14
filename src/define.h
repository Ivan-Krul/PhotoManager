#pragma once
#include <string>
#include <cassert>

#define NOT_IMPLEMENTED(function) (throw (std::string("not implemented ") + function).c_str())

#pragma warning(disable : 4996)

#define EXPLORERTREE_TOO_MUCH "..."

#define DEBUG
//#define SHOW_DIR_SELECTION_AT_BEGINNING
//#define ENABLE_TESTS_IN_APPFRAME

//#define CONSOLE
