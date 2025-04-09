#pragma once

#include <wx/wx.h>

class AppFrame : public wxFrame {
public:
  AppFrame();

private:
  void PinMenu();

  void BindFrames();

  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

  enum IDs : int {
    ID_Hello = 1
  };
};
