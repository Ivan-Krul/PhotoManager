#include "AppFrame.h"

AppFrame::AppFrame() : wxFrame(nullptr, wxID_ANY, "Hello World") {
  PinMenu();

  CreateStatusBar();
  SetStatusText("Welcome to wxWidgets!");

  BindFrames();
}

void AppFrame::PinMenu() {
  wxMenu* menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl+H", "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu* menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);
}

void AppFrame::BindFrames() {
  Bind(wxEVT_MENU, &AppFrame::OnHello, this, ID_Hello);
  Bind(wxEVT_MENU, &AppFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &AppFrame::OnExit, this, wxID_EXIT);
}

void AppFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void AppFrame::OnAbout(wxCommandEvent& event) {
  wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void AppFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from wxWidgets!");
}


