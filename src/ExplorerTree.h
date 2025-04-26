#pragma once
#include "Status.h"

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/artprov.h>

class ExplorerTree {
public:
  enum IconId : int {
    ICONID_Folder = 0,
    ICONID_DefaultFile
  };

  inline void InitLayout(wxPanel* panel, const Status* status); // can't recognize element from AppFrame::SetLayout

private:
  wxImageList* CreateImageList();

private:
  std::vector<wxTreeItemId> maTreeItems;
};

