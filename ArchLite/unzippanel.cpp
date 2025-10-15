#include "UnzipPanel.h"

UnzipPanel::UnzipPanel(wxWindow* parent)
    : wxPanel(parent)
{
    createControls();
    SetupSizers();
}

void UnzipPanel::createControls()
{
    headlineText = new wxStaticText(this, wxID_ANY, "Unzip Files");
}

void UnzipPanel::SetupSizers()
{
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(headlineText, 0, wxALL | wxALIGN_CENTER, 10);
    SetSizer(mainSizer);
}
