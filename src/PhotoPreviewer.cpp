#include "PhotoPreviewer.h"

void PhotoPreviewer::InitLayout(wxFrame* parent_frame, wxPanel* panel, Status* status) {
  pParentFrame = parent_frame;
  pParentPanel = panel;
  pStatus = status;

  pGridSizer = new wxGridSizer(20, cGridWidth, 5, 5);

  wxButton* (btns[100]);

  for (int i = 0; i < 100; i++) {
    btns[i] = new wxButton(pParentPanel, 6004+i, wxString("Beam: ") << i + 1, wxDefaultPosition, wxSize(60, 20));
    pGridSizer->Add(btns[i], wxALL | wxALIGN_CENTER | wxEXPAND);
  }

  pParentPanel->SetSizerAndFit(pGridSizer);
}
