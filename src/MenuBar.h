#pragma once
#include "WidgetIds.h"

#include <wx/wx.h>

namespace menu_bar {
  void InitLayout(wxFrame* frame) {
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
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuSearch, "&Search");
    menuBar->Append(menuHelp, "&Help");

    frame->SetMenuBar(menuBar);
  }
}
