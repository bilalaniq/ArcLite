#include "Mainframe.h"
#include <wx/notebook.h>
#include "ZipPanel.h"
#include "UnzipPanel.h"

Mainframe::Mainframe(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title)
{

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxNotebook* tabs = new wxNotebook(this, wxID_ANY);
    tabs->AddPage(new ZipPanel(tabs), "Zip");
    tabs->AddPage(new UnzipPanel(tabs), "Unzip");

    sizer->Add(tabs, 1, wxEXPAND | wxALL, FromDIP(10));
    SetSizer(sizer);
    this->SetBackgroundColour(tabs->GetBackgroundColour());
}
