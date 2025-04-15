#include "Application.h"

#include "AppFrame.h"

#include <wx/splitter.h>

bool Application::OnInit() {
  AppFrame* frame = new AppFrame(wxT("Photo Manager"));
  frame->Show();
  return true;
}

