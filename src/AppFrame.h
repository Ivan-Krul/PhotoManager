#pragma once
#include "Status.h"
#include "ExplorerTree.h"

#include <vector>

#include <wx/wx.h>
#include <wx/treectrl.h>

class AppFrame : public wxFrame {
public:
  AppFrame(const wxString& title, Status* status);

private:
  void SetLayout();

  void BindEvents();

  void OnHello(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnFind(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnExplorerTreeActivateItem(wxTreeEvent& event);

#ifdef ENABLE_TESTS_IN_APPFRAME
  void TestSetSideSplitter();
  void TestSetGroupSideSplitter();
#endif

  ExplorerTree mTree;
  
  Status* pStatus;
};
