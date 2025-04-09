#include "Application.h"

#include "AppFrame.h"

bool Application::OnInit() {
  AppFrame* frame = new AppFrame();
  frame->Show();
  return true;
}

