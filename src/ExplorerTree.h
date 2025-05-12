#pragma once
#include <string>
#include <vector>

#include "Status.h"

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/artprov.h>

class ExplorerTree : public wxFrame { // initialized as a frame to handle the further logic & events there
public:
  enum IconId : int {
    ICONID_Folder = 0,
    ICONID_DefaultFile
  };

  void InitLayout(wxPanel* panel, const Status* status);

  inline wxTreeItemId getId(size_t index) const { return maTreeItems[index]; }

private:
  wxImageList* CreateImageList();

private:
  std::vector<wxTreeItemId> maTreeItems;
};

