#include "Application.h"

#include "AppFrame.h"

#include <wx/splitter.h>
#include <wx/dirdlg.h> 

bool Application::OnInit() {
  wxLogWindow* log = new wxLogWindow(NULL, "Log window", true, true);

  wxMessageBox("Choose a directory of your photos");
  if (!SetDirectoryRoot()) return false;

  AppFrame* frame = new AppFrame(wxT("Photo Manager"),&mStatus, log);
  frame->Show();
  return true;
}

bool Application::SetDirectoryRoot() {
  const wxString& dir = wxDirSelector("Choose a directory a your photos");
  if (dir.empty()) return false;

  mStatus.branch.path = dir.utf8_string();
  mStatus.branch = initDirList(mStatus.branch.path);

  return true;
}

