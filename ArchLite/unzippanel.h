#pragma once
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/progdlg.h>
#include <wx/zipstrm.h> 
#include <wx/wfstream.h>
#include <wx/filesys.h>

class UnzipPanel : public wxPanel
{
public:
    UnzipPanel(wxWindow* parent);

private:

    void SetupSections();

    void SetupUNzipsection();

    void performUNzip();
    void UNzipAllFiles();
    void UNzipsingleFile();


    
    wxBoxSizer* mainSizer;
    wxTextCtrl* zipFileText;
    wxTextCtrl* OutDirText;
    wxTextCtrl* singleFileText;

    wxCheckBox* singleFileCheckBox;
};
