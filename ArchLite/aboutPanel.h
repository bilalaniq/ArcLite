#pragma once
#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/statline.h>  // ? Needed for wxStaticLine


class aboutPanel : public wxPanel
{
public:
    aboutPanel(wxWindow* parent);

private:
    void CreateLayout();
};
