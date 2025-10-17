#include "UnzipPanel.h"
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/progdlg.h>
#include <wx/zipstrm.h> 
#include <wx/wfstream.h>



UnzipPanel::UnzipPanel(wxWindow* parent)
    : wxPanel(parent)
{
    SetupSections();
}

void UnzipPanel::SetupSections()
{
    mainSizer = new wxBoxSizer(wxVERTICAL);
    SetupUNzipsection();
    SetSizer(mainSizer);
}

void UnzipPanel::SetupUNzipsection()
{
    // --- Title label ---
    wxStaticText* unziptitlelabel = new wxStaticText(this, wxID_ANY, "Unzip File");
    unziptitlelabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // --- Zip file selection ---
    zipFileText = new wxTextCtrl(this, wxID_ANY);
    zipFileText->SetEditable(false);

    wxButton* zipFileBrowseButton = new wxButton(this, wxID_ANY, "Browse...");

    // --- Output directory ---
    wxStaticText* OutDirLabel = new wxStaticText(this, wxID_ANY, "Output Directory");
    OutDirLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    OutDirText = new wxTextCtrl(this, wxID_ANY);
    OutDirText->SetEditable(false);

    wxButton* outFileBrowseButton = new wxButton(this, wxID_ANY, "Browse...");

    // --- Single file option ---
    singleFileCheckBox = new wxCheckBox(this, wxID_ANY, "Only one file");
    singleFileText = new wxTextCtrl(this, wxID_ANY);
    singleFileText->SetEditable(false);

    wxButton* UNzipButton = new wxButton(this, wxID_ANY, "Unzip");

    // --- Layout sections ---
    wxBoxSizer* unzipSizer = new wxBoxSizer(wxHORIZONTAL);
    unzipSizer->Add(zipFileText, 1, wxEXPAND | wxRIGHT, FromDIP(8));
    unzipSizer->Add(zipFileBrowseButton, 0, wxEXPAND, FromDIP(8));

    wxBoxSizer* outDirSizer = new wxBoxSizer(wxHORIZONTAL);
    outDirSizer->Add(OutDirText, 1, wxEXPAND | wxRIGHT, FromDIP(8));
    outDirSizer->Add(outFileBrowseButton, 0, wxEXPAND, FromDIP(8));

    // --- Add all to main sizer ---
    mainSizer->AddSpacer(FromDIP(8));
    mainSizer->AddSpacer(FromDIP(8));
    mainSizer->AddSpacer(FromDIP(8));



    mainSizer->Add(unziptitlelabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(unzipSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->Add(OutDirLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(outDirSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->AddSpacer(FromDIP(8));

    mainSizer->Add(singleFileCheckBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(singleFileText, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->AddSpacer(FromDIP(8));

    mainSizer->Add(UNzipButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));


    zipFileBrowseButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        wxFileDialog dialog(this, "open zip file", wxEmptyString, wxEmptyString,
            "Zip files (*.zip)|*.zip", wxFD_FILE_MUST_EXIST | wxFD_OPEN);
        if (dialog.ShowModal() == wxID_CANCEL) {
            return;
        }
        zipFileText->SetValue(dialog.GetPath());
        });


    outFileBrowseButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        wxDirDialog dialog(this, "Select output directory", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);
        if (dialog.ShowModal() == wxID_CANCEL) {
            return;
        }
        OutDirText->SetValue(dialog.GetPath());
        });

    singleFileCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& evt) {
        if (evt.IsChecked())
        {
            singleFileText->SetFocus();
            singleFileText->SetEditable(true);
        }
        else
        {
            singleFileText->SetEditable(false);
            singleFileText->Clear();
        }
        });


    UNzipButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        performUNzip();
        });
}

// --- Placeholder methods ---
void UnzipPanel::performUNzip() {

    if (zipFileText->GetValue().IsEmpty() || OutDirText->GetValue().IsEmpty()) {
        wxMessageBox("plz fill all the fields", "Error", wxOK | wxICON_ERROR);
        return;
    }
    if (singleFileCheckBox->IsChecked()) {
        UNzipsingleFile();
    }
    else
    {
        UNzipAllFiles();
    }
}
void UnzipPanel::UNzipAllFiles() {

    wxProgressDialog dialog("Unzipping", "Unziping files...", 100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

    wxFileInputStream inStream(zipFileText->GetValue());
    wxZipInputStream ZipIn(inStream);
    std::unique_ptr<wxZipEntry> entry(ZipIn.GetNextEntry());

    while (entry)
    {
        dialog.Pulse();
        wxString entryName = entry->GetName();

        wxFileName destFileName = OutDirText->GetValue() + wxFileName::GetPathSeparator() + entryName;

        bool isFile = !entry->IsDir();

        if (!wxDirExists(destFileName.GetPath())) {
            wxFileName::Mkdir(destFileName.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        }

        if (isFile) {
            if (!ZipIn.CanRead()) {
                wxLogError("Couldn't read the zip entry %s", entry->GetName());
                return;
            }

            wxFileOutputStream outStream(destFileName.GetFullPath());
            if (!outStream.IsOk()) {
                wxLogError("Couldn't create  the zip entry %s", destFileName.GetFullPath());
                return;
            }

            ZipIn.Read(outStream);

            ZipIn.CloseEntry();
            outStream.Close();
        }
        
        entry.reset(ZipIn.GetNextEntry());
    }
}


//void UnzipPanel::UNzipsingleFile() {
//    if (singleFileText->GetValue().IsEmpty()) {
//        wxMessageBox("Please enter a file name to extract.", "Error", wxOK | wxICON_ERROR);
//        return;
//    }
//
//    wxFileSystem fs;  // wxFileSystem is a powerful class that can handle virtual file systems (ZIPs, HTTP, etc).
//    std::unique_ptr<wxFSFile> zip(fs.OpenFile(zipFileText->GetValue() + "#zip:" + singleFileText->GetValue()));
//    //"path/to/archive.zip#zip:filename"
//    //means “open filename inside that ZIP file.”
//
//    //but we canot get dir using this this is why we can not extract empty directories using this but its ok we are 
//    //any way extracting single file here
//
//
//    if (zip) {
//
//        wxInputStream* in = zip->GetStream();
//
//        if (in) {
//
//            wxFileName destFileName = OutDirText->GetValue() + wxFileName::GetPathSeparator() + singleFileText->GetValue();
//
//            if (!wxDirExists(destFileName.GetPath())) {
//                wxFileName::Mkdir(destFileName.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
//            }
//
//            wxFileOutputStream outStream(destFileName.GetFullPath());
//            outStream.Write(*in);
//            outStream.Close();
//        }
//    }
//    else
//    {
//        wxMessageBox("File not found in zip.", "Error", wxOK | wxICON_ERROR);
//    }
//}



void UnzipPanel::UNzipsingleFile()
{
    wxString zipPath = zipFileText->GetValue();
    wxString fileToExtract = singleFileText->GetValue();
    wxString outputDir = OutDirText->GetValue();

    if (fileToExtract.IsEmpty()) {
        wxMessageBox("Please enter a file name to extract.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!wxFileExists(zipPath)) {
        wxMessageBox("ZIP file not found.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Open the ZIP file for reading
    wxFFileInputStream in(zipPath);
    if (!in.IsOk()) {
        wxMessageBox("Unable to open ZIP file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxZipInputStream zip(in);
    std::unique_ptr<wxZipEntry> entry;
    bool found = false;

    // Iterate over each entry in the zip
    while ((entry.reset(zip.GetNextEntry()), entry.get() != nullptr)) {
        wxString name = entry->GetName();

        if (name == fileToExtract) {
            found = true;

            wxFileName destFileName(outputDir, name);
            if (!wxDirExists(destFileName.GetPath())) {
                wxFileName::Mkdir(destFileName.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
            }

            wxFFileOutputStream out(destFileName.GetFullPath());
            if (!out.IsOk()) {
                wxMessageBox("Failed to create output file: " + destFileName.GetFullPath(), "Error", wxOK | wxICON_ERROR);
                return;
            }

            zip.Read(out);
            out.Close();

            wxMessageBox("File extracted successfully to:\n" + destFileName.GetFullPath(), "Success", wxOK | wxICON_INFORMATION);
            break;
        }
    }

    if (!found) {
        wxMessageBox("Specified file not found inside the ZIP archive.", "Error", wxOK | wxICON_ERROR);
    }
}