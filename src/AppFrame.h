#pragma once
#include "Status.h"

#include <wx/wx.h>

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

#ifdef DEBUG
  void TestSetSideSplitter();
  void TestSetGroupSideSplitter();
#endif

  Status* pStatus;
};
