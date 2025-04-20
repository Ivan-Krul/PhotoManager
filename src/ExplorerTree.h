#pragma once
#include "Status.h"

#include <wx/wx.h>
#include <wx/treectrl.h>

namespace explorer_tree {
  void InitLayout(wxPanel* panel, Status* status) {
    wxBoxSizer* treeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTreeCtrl* tree = new wxTreeCtrl(panel, ID_ExplorerTree);

    auto rootId = tree->AddRoot(status->branch.path.filename().c_str());

    for (const auto year : status->branch.years) {
      tree->AppendItem(rootId, wxString::Format(wxT("%i"),year));
    }

    tree->Expand();
    treeSizer->Add(tree, 1, wxEXPAND);
    panel->SetSizerAndFit(treeSizer);
  }
}
