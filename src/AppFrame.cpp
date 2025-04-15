#include "AppFrame.h"

#include <wx/splitter.h>
#include <wx/treectrl.h>

AppFrame::AppFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
  PinMenu();
  BindHandles();

  SetLayout();

  CreateStatusBar();
  SetStatusText("Welcome to Photo Manager!");
}

void AppFrame::PinMenu() {
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl+H", "Hello window");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_SAVE);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu* menuSearch = new wxMenu;
  menuSearch->Append(wxID_FIND);

  wxMenu* menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile,   "&File");
  menuBar->Append(menuSearch, "&Search");
  menuBar->Append(menuHelp,   "&Help");

  SetMenuBar(menuBar);
}

void AppFrame::BindHandles() {
  Bind(wxEVT_MENU, &AppFrame::OnHello, this, ID_Hello);
  Bind(wxEVT_MENU, &AppFrame::OnSave, this, wxID_SAVE);

  Bind(wxEVT_MENU, &AppFrame::OnFind, this, wxID_FIND);

  Bind(wxEVT_MENU, &AppFrame::OnAbout, this, wxID_ABOUT);

  Bind(wxEVT_MENU, &AppFrame::OnExit, this, wxID_EXIT);

  Bind(wxEVT_BUTTON, &AppFrame::OnButton, this, ID_Button);
}

void AppFrame::SetLayout() {
  wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER);

  wxPanel* leftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
  leftPanel->SetBackgroundColour(wxColor(100, 100, 200));

  wxBoxSizer* treeSizer = new wxBoxSizer(wxHORIZONTAL);
  wxTreeCtrl* tree = new wxTreeCtrl(leftPanel, wxID_ANY);
  tree->AddRoot("ROOOT");
  treeSizer->Add(tree, 1, wxEXPAND);
  leftPanel->SetSizerAndFit(treeSizer);

  wxPanel* rightPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
  rightPanel->SetBackgroundColour(wxColor(100, 200, 200));

  splitter->SetMinimumPaneSize(200);
  splitter->SplitVertically(leftPanel, rightPanel);

}

#ifdef DEBUG
void AppFrame::TestSetSideSplitter() {
  wxBoxSizer* sizermain = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow* splittermain = new wxSplitterWindow(this, wxID_ANY);
  splittermain->SetSashGravity(0.5);
  splittermain->SetMinimumPaneSize(20);
  sizermain->Add(splittermain, 1, wxEXPAND, 0);

  wxPanel* pnl1 = new wxPanel(splittermain, wxID_ANY);
  wxButton* btn1 = new wxButton(pnl1, ID_Button, "Button");

  wxPanel* pnl2 = new wxPanel(splittermain, wxID_ANY);
  wxBoxSizer* txt2sizer = new wxBoxSizer(wxVERTICAL);
  wxTextCtrl* txt2 = new wxTextCtrl(pnl2, wxID_ANY);
  txt2sizer->Add(txt2, 1, wxEXPAND, 0);
  pnl2->SetSizer(txt2sizer);

  splittermain->SplitVertically(pnl1, pnl2);

  SetSizer(sizermain);
  sizermain->SetSizeHints(this);
}


/*
*                      FRAME
*                        ^
*      PANEL_GROUP1 > SPLITTER < PANEL_GROUP2
*           ^           HOR           ^
*       TEXT_CTRL1                TEXT_CTRL2
*/
void AppFrame::TestSetGroupSideSplitter() {
  wxBoxSizer* mainsizer = new wxBoxSizer(wxHORIZONTAL);
  wxSplitterWindow* splittermain = new wxSplitterWindow(this, wxID_ANY);
  mainsizer->Add(splittermain, 1, wxEXPAND, 5);

  wxPanel* panelgroup1 = new wxPanel(splittermain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER);
  wxStaticBoxSizer* group1 = new wxStaticBoxSizer(wxVERTICAL, panelgroup1, wxT(""));
  group1->SetMinSize(550, -1);
  wxTextCtrl* txt1 = new wxTextCtrl(panelgroup1, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  group1->Add(txt1, 1, wxEXPAND, 5);
  panelgroup1->SetSizer(group1);

  wxPanel* panelgroup2 = new wxPanel(splittermain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER);
  wxStaticBoxSizer* group2 = new wxStaticBoxSizer(wxVERTICAL, panelgroup2, wxT(""));
  group2->SetMinSize(550, -1);
  wxTextCtrl* txt2 = new wxTextCtrl(panelgroup2, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  group2->Add(txt2, 1, wxEXPAND, 5);
  panelgroup2->SetSizer(group2);

  splittermain->SetSashGravity(1.0);
  splittermain->SplitHorizontally(panelgroup2, panelgroup1);

  SetSizer(mainsizer);
  mainsizer->SetSizeHints(this);
}
#endif

void AppFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void AppFrame::OnSave(wxCommandEvent& event) {
  SetStatusText("Saved (nothing to do)");
}

void AppFrame::OnFind(wxCommandEvent& event) {
  wxMessageBox("There should be a find tab", "Find", wxOK);
}

void AppFrame::OnAbout(wxCommandEvent& event) {
  wxMessageBox("Photo Manager is a program, that helps to manage photos", "About Photo Manager", wxOK | wxICON_INFORMATION);
}

void AppFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from Photo Manager!");
}

void AppFrame::OnButton(wxCommandEvent& event) {
  wxLogMessage("HAii!!");
}


