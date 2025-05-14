#include "Application.h"

#include "AppFrame.h"

#include <wx/splitter.h>
#include <wx/dirdlg.h> 

bool Application::OnInit() {
#ifdef SHOW_DIR_SELECTION_AT_BEGINNING
  if (!SetDirectoryRoot()) return false;
#else
  mStatus.branch.path = "D:/Cellphone";
  mStatus.branch = initDirList(mStatus.branch.path);
#endif

  AppFrame* frame = new AppFrame(wxT("Photo Manager"),&mStatus);
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

