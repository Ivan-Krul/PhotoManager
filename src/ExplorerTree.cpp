#include "ExplorerTree.h"
#include "WidgetIds.h"
#include "DirectoryFetcher.h"

#include <filesystem>

#include <wx/wx.h>

void ExplorerTree::InitLayout(wxPanel* panel, const Status* status) {
   wxBoxSizer* treeSizer = new wxBoxSizer(wxHORIZONTAL);
   wxTreeCtrl* tree = new wxTreeCtrl(panel, ID_ExplorerTree);

   //    tree->Connect(10, wxEVT_COMMAND_BUTTON_CLICKED, wxTreeEventHandler(nullptr));
   tree->AssignImageList(CreateImageList());

   auto rootId = tree->AddRoot(status->branch.path.filename().c_str(), ICONID_Folder);

   if (status->branch.has_research)
     maTreeItems.push_back(tree->AppendItem(rootId, ".research", ICONID_Folder));

   auto childId = rootId;

   for (const auto year : status->branch.years) {
     maTreeItems.push_back(tree->AppendItem(rootId, wxString::Format(wxT("%i"), year), ICONID_Folder));
     childId = maTreeItems.back();

     auto inner_year = getChildItem(status->branch.path / std::to_string(year));
     for (const auto& inner_item : inner_year) {
       if (inner_item.is_directory())
         maTreeItems.push_back(tree->AppendItem(childId, inner_item.path().filename().string(), ICONID_Folder));
       else
         maTreeItems.push_back(tree->AppendItem(childId, inner_item.path().filename().string()));
     }
   }

   if (status->branch.has_buffer_dir)
     maTreeItems.push_back(tree->AppendItem(rootId, "buffer", ICONID_Folder));

   tree->Expand(rootId);
   treeSizer->Add(tree, 1, wxEXPAND);
   panel->SetSizerAndFit(treeSizer);
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

