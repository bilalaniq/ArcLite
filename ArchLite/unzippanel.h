#pragma once
#include <wx/wx.h>

class UnzipPanel : public wxPanel
{
public:
    UnzipPanel(wxWindow* parent);

private:
    void createControls();
    void SetupSizers();

    wxStaticText* headlineText;
    wxBoxSizer* mainSizer;
};
