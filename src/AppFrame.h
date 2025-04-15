#pragma once

#include <wx/wx.h>

class AppFrame : public wxFrame {
public:
  AppFrame(const wxString& title);

private:
  void PinMenu();
  void BindHandles();

  void SetLayout();

#ifdef DEBUG
  void TestSetSideSplitter();
  void TestSetGroupSideSplitter();
#endif

  void OnHello(wxCommandEvent& event);
  void OnButton(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnFind(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

  enum IDs : int {
    ID_Hello = 1,
    ID_Button
  };

};
