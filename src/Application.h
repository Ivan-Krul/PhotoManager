#pragma once
#include "Status.h"
#include "Init.h"

#include <wx/wx.h>

class Application : public wxApp {
public:
  bool OnInit() override;
private:
  bool SetDirectoryRoot();

  Status mStatus;
};
