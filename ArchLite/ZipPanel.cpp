#include "ZipPanel.h"



ZipPanel::ZipPanel(wxWindow* parent)
    : wxPanel(parent)
{
    SetupSections();
}

void ZipPanel::SetupSections()
{
    // Create the main vertical sizer for the panel
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add all sections
    SetupLoadDirectorySection();
    SetupLoadFileSection();
    SetupCompressSection();

    // Attach the sizer to this panel
    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}



void ZipPanel::SetupLoadDirectorySection()
{
    // Section title
    wxStaticText* zipTitleLabel = new wxStaticText(this, wxID_ANY, "Zip Directory");
    zipTitleLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Text field + browse button
    DirToCompressText = new wxTextCtrl(this, wxID_ANY);
    wxButton* dirButton = new wxButton(this, wxID_ANY, "Browse...");

    // Horizontal sizer for directory input area
    wxBoxSizer* dirSectionSizer = new wxBoxSizer(wxHORIZONTAL);
    dirSectionSizer->Add(DirToCompressText, wxSizerFlags(1).Expand().Border(wxALL, FromDIP(8)));
    dirSectionSizer->Add(dirButton, wxSizerFlags(0).CenterVertical().Border(wxALL, FromDIP(8)));

    // Add to main vertical sizer
    mainSizer->Add(zipTitleLabel, wxSizerFlags(0).Border(wxALL, FromDIP(8)));
    mainSizer->Add(dirSectionSizer, wxSizerFlags(0).Expand());

    // Bind the "Browse" button click event
    dirButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&)
        {
            wxDirDialog dialog(this, "Choose a directory", wxGetCwd()
                );
            if (dialog.ShowModal() == wxID_OK)
            {
                DirToCompressText->SetValue(dialog.GetPath());
                LoadFilesToCompress();
            }
        });
}


void ZipPanel::SetupLoadFileSection()
{
    wxStaticText* filesLabel = new wxStaticText(this, wxID_ANY, "Files to compress");
    FilesList = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize ,
        wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);

    FilesList->AppendColumn("File");

    wxStaticText* ignoreLabel = new wxStaticText(this, wxID_ANY, "Ignored directories: ");
    IgnoredList = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);

    IgnoredList->AppendColumn("Ignored");

    wxButton* ignoreAddbutton = new wxButton(this, wxID_ANY, "Add...");
    wxButton* ignoreRemovebutton = new wxButton(this, wxID_ANY, "Remove");



    wxGridBagSizer* sizer = new wxGridBagSizer(FromDIP(8), FromDIP(8));

    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(1);


    sizer->Add(filesLabel , {0 , 0} , {1 , 1} , wxEXPAND);

    sizer->Add(FilesList, { 1, 0 }, { 1, 1 }, wxEXPAND);

    sizer->Add(ignoreLabel, { 0, 1 }, { 1, 2 }, wxEXPAND);
    sizer->Add(IgnoredList, { 1, 1 }, { 1, 2 }, wxEXPAND);


    sizer->Add(ignoreAddbutton, { 2, 1 }, { 1, 1 }, wxEXPAND);
    sizer->Add(ignoreRemovebutton, { 2, 2 }, { 1, 1 }, wxEXPAND);

    mainSizer->Add(sizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    std::function<void(wxSizeEvent&)> headerResize = [this](wxSizeEvent& event) 
        {
            wxListView* list = dynamic_cast<wxListView*>(event.GetEventObject());
            if (list)
            {
                list->SetColumnWidth(0, list->GetClientSize().GetWidth());
            }

            event.Skip();
        };

    FilesList->Bind(wxEVT_SIZE, headerResize);
    IgnoredList->Bind(wxEVT_SIZE, headerResize);


    ignoreAddbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        wxTextEntryDialog dialog(this, "Enter the directory name to ignore", "Add ignored directory");
        if (dialog.ShowModal() == wxID_OK) {
            IgnoredList->InsertItem(IgnoredList->GetItemCount(), dialog.GetValue());
            LoadFilesToCompress();
        }
        });

    ignoreRemovebutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        long item = IgnoredList->GetFirstSelected();
        if (item >= 0)
        {
            IgnoredList->DeleteItem(item);

            LoadFilesToCompress();
        }
        });


    ignoreRemovebutton->Disable();

    IgnoredList->Bind(wxEVT_LIST_ITEM_SELECTED, [this, ignoreRemovebutton](wxListEvent& event)
        { ignoreRemovebutton->Enable(); });

    IgnoredList->Bind(wxEVT_LIST_ITEM_DESELECTED, [this, ignoreRemovebutton](wxListEvent& event)
        { ignoreRemovebutton->Disable(); });

    



}

void ZipPanel::SetupCompressSection()
{
    wxStaticText* outFileLabel = new wxStaticText(this, wxID_ANY, "Output File");
    outFileLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    zipFileText = new wxTextCtrl(this, wxID_ANY);
    zipFileText->SetEditable(false);

    wxButton* outFileButton = new wxButton(this, wxID_ANY, "Change...");

    wxButton* StartButton = new wxButton(this, wxID_ANY, "Start!");

    wxBoxSizer* outFileSizer = new wxBoxSizer(wxHORIZONTAL);

    outFileSizer->Add(zipFileText, wxSizerFlags(1).Expand().Border(wxLEFT | wxBOTTOM, FromDIP(8)));
    outFileSizer->Add(outFileButton, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8)));


    mainSizer->Add(outFileLabel, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8)));
    mainSizer->Add(outFileSizer, wxSizerFlags(0).Expand());
    mainSizer->Add(StartButton, wxSizerFlags(0).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8)));

    outFileButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        wxFileDialog dialog(this, "Choose a file", wxGetCwd(),
            "out.zip", "Zip files (*.zip)|*.zip", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (dialog.ShowModal() == wxID_OK) {
            zipFileText->SetValue(dialog.GetPath());
        }


        });

    StartButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        if (FilesList->GetItemCount() == 0) {
            wxMessageBox("no file to commpress", "Error", wxOK | wxICON_ERROR);
        }
        else if (zipFileText->GetValue().empty()) {
            wxMessageBox("no output file specified", "Error", wxOK | wxICON_ERROR);
        }
        else
        {
            this->PerformCompression();
        }
        
        });





}

void ZipPanel::LoadFilesToCompress()
{
    static constexpr int PulseInterval = 100;

    if (!wxDirExists(DirToCompressText->GetValue())) {
        return;
    }

    wxProgressDialog dialog("Loading Files", "Loading files to compress...",
        100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);


}

void ZipPanel::PerformCompression()
{
    wxMessageBox("ok", "ok", wxOK);
}

