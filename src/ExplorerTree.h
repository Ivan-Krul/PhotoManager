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

  void InitLayout(wxPanel* panel, const Status* status);

private:
  wxImageList* CreateImageList();

private:
  std::vector<wxTreeItemId> maTreeItems;
};

