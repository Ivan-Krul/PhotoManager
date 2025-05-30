#pragma once
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <wx/statbmp.h>

#include <vector>

#include "Status.h"
#include "WidgetIds.h"

// combine all those ingredience into this
// yes, this is a crafting table

class PhotoPreviewer {
public:
  void InitLayout(wxFrame* parent_frame, wxPanel* panel, Status* status);

  //void UpdateLayout();


private:
  Status* pStatus;

  wxPanel* pParentPanel;
  wxFrame* pParentFrame;

  wxGridSizer* pGridSizer;

  std::vector<wxImage*> maImages;
  
  const size_t cGridWidth = 5;
};

