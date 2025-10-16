#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/gbsizer.h>
#include <wx/progdlg.h>
#include <wx/stream.h>
#include <wx/zipstrm.h> 
#include <wx/wfstream.h>
#include "ignoringtraverser.h"


class ZipPanel : public wxPanel
{
public:
    ZipPanel(wxWindow* parent);

private:
    void SetupSections();
    void SetupSizers();
    void SetupLoadDirectorySection();
    void SetupLoadFileSection();
    void SetupCompressSection();


    void LoadFilesToCompress();
    void PerformCompression();

    wxBoxSizer* mainSizer;
    wxTextCtrl* DirToCompressText;
    wxListView* FilesList;
    wxListView* IgnoredList;  

    wxTextCtrl* zipFileText;
};
