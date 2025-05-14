#pragma once
#include <string>
#include <vector>

#include "Status.h"

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/artprov.h>

class ExplorerTree { // initialized as a frame to handle the further logic & events there
public:
  enum IconId : int {
    ICONID_Folder = 0,
    ICONID_DefaultFile
  };

  void InitLayout(wxFrame* parent_frame, wxPanel* panel, const Status* status);

  inline wxTreeItemId getId(size_t index) const { return maTreeItems[index]; }

  void OnActivateItem(wxTreeEvent& event);

private:
  std::vector<wxTreeItemId> ConstructItemChain(wxTreeItemId id);
  wxString ConstructPath(wxTreeItemId id);

  wxImageList* CreateImageList();

private:
  std::vector<wxTreeItemId> maTreeItems;

  wxTreeCtrl* pTree;
  wxPanel* pParentPanel;
  wxFrame* pParentFrame;
};

