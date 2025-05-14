#include "ExplorerTree.h"
#include "WidgetIds.h"
#include "DirectoryFetcher.h"

#include <filesystem>

#include <wx/wx.h>

void ExplorerTree::InitLayout(wxFrame* parent_frame, wxPanel* panel, const Status* status) {
  pParentFrame = parent_frame;
  pParentPanel = panel;

  wxBoxSizer* treeSizer = new wxBoxSizer(wxHORIZONTAL);
  pTree = new wxTreeCtrl(pParentPanel, ID_ExplorerTree);

  //    tree->Connect(10, wxEVT_COMMAND_BUTTON_CLICKED, wxTreeEventHandler(nullptr));
  pTree->AssignImageList(CreateImageList());

  auto rootId = pTree->AddRoot(status->branch.path.filename().c_str(), ICONID_Folder);

  if (status->branch.has_research)
    maTreeItems.push_back(pTree->AppendItem(rootId, ".research", ICONID_Folder));

  auto childId = rootId;

  for (const auto year : status->branch.years) {
    maTreeItems.push_back(pTree->AppendItem(rootId, wxString::Format(wxT("%i"), year), ICONID_Folder));
    childId = maTreeItems.back();

    auto inner_year = getChildItem(status->branch.path / std::to_string(year));

    if (inner_year.size() > 50) {
      maTreeItems.push_back(pTree->AppendItem(childId, EXPLORERTREE_TOO_MUCH));
      continue;
    }
    
    sortByNumber(inner_year);

    for (const auto& inner_item : inner_year) {
      if (inner_item.is_directory())
        maTreeItems.push_back(pTree->AppendItem(childId, inner_item.path().filename().string(), ICONID_Folder));
      else
        maTreeItems.push_back(pTree->AppendItem(childId, inner_item.path().filename().string()));
    }
  }

  if (status->branch.has_buffer_dir)
    maTreeItems.push_back(pTree->AppendItem(rootId, "buffer", ICONID_Folder));

  pTree->Expand(rootId);
  treeSizer->Add(pTree, 1, wxEXPAND);
  pParentPanel->SetSizerAndFit(treeSizer);
}

void ExplorerTree::OnActivateItem(wxTreeEvent& event) {
  auto baseChildItemId = event.GetItem();

  pTree->GetItemParent(baseChildItemId);

  wxMessageBox(wxString("From the tree:") + ConstructPath(event.GetItem()));
}

std::vector<wxTreeItemId> ExplorerTree::ConstructItemChain(wxTreeItemId id) {
  std::vector<wxTreeItemId> path;

  const wxTreeItemId id_root = pTree->GetRootItem();
  wxTreeItemId id_local = id;

  do {
    path.push_back(id_local);
    id_local = pTree->GetItemParent(id_local);
  } while (id_local != id_root);

  return path;
}

wxString ExplorerTree::ConstructPath(wxTreeItemId id) {
  auto path_ids = ConstructItemChain(id);
  bool is_there_alot_of_items = pTree->GetItemText(id) == EXPLORERTREE_TOO_MUCH;
  wxString res;

  for (int i = path_ids.size() - 1 - is_there_alot_of_items; i >= 0; i--) {
    res += "/" + pTree->GetItemText(path_ids[i]);
  }

  return res;
}

wxImageList* ExplorerTree::CreateImageList() {
  wxIcon icons[2];

  icons[ICONID_Folder] = wxArtProvider::GetIcon(wxART_FOLDER);
  icons[ICONID_DefaultFile] = wxArtProvider::GetIcon(wxART_NORMAL_FILE);

  int sizeOrig = icons[0].GetWidth();
  wxImageList* imageList = new wxImageList(sizeOrig, sizeOrig, true);

  for (size_t i = 0; i < WXSIZEOF(icons); i++) {
    imageList->Add(icons[i]);
  }
  return imageList;
}

