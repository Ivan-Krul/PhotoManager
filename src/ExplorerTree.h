#pragma once
#include "Status.h"

#include <wx/wx.h>
#include <wx/treectrl.h>

namespace explorer_tree {
  void InitLayout(wxPanel* panel, Status* status) {
    wxBoxSizer* treeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTreeCtrl* tree = new wxTreeCtrl(panel, ID_ExplorerTree);

    auto rootId = tree->AddRoot("ROOOT");
    wxTreeItemData* treeData1 = new wxTreeItemData;
    wxTreeItemData* treeData2 = new wxTreeItemData;
    tree->AppendItem(rootId, "child1");
    tree->AppendItem(rootId, "child2");

    tree->ExpandAll();
    treeSizer->Add(tree, 1, wxEXPAND);
    panel->SetSizerAndFit(treeSizer);
  }
}
