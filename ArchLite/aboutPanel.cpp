#include "aboutPanel.h"
#include <wx/hyperlink.h>
#include <wx/statline.h>
#include <wx/statbmp.h>
#include <wx/mstream.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/msw/bitmap.h>     // For wxBitmap (Windows)
#include <wx/msw/private.h>    // For FindResource/LoadResource helpers
#include "resource.h"          // Your resource IDs (IDB_PNG1, etc.)       // Include your resource IDs (IDI_ICON1, IDB_PNG1, etc.)

aboutPanel::aboutPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    SetBackgroundColour(wxColour(43, 26, 26)); // Dark maroon background
    CreateLayout();
}

void aboutPanel::CreateLayout()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // --- Title ---
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "ArcLite");
    title->SetFont(wxFontInfo(22).Bold().FaceName("Segoe UI Semibold"));
    title->SetForegroundColour(wxColour(255, 214, 214));
    mainSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 15);

    // --- Version ---
#ifdef _WIN64
    const char* arch = "64-bit";
#else
    const char* arch = "32-bit";
#endif

    wxString versionStr = wxString::Format("Version 1.0.0 (%s)", arch);
    wxStaticText* version = new wxStaticText(this, wxID_ANY, versionStr);
    version->SetFont(wxFontInfo(11).FaceName("Segoe UI"));
    version->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(version, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

    // --- Description ---
    wxStaticText* desc = new wxStaticText(
        this, wxID_ANY,
        "A fast and reliable ZIP compression tool\n"
        "built with wxWidgets for secure file management.",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    desc->SetFont(wxFontInfo(10).FaceName("Segoe UI"));
    desc->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(desc, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 15);

    // --- Separator ---
    wxStaticLine* line = new wxStaticLine(this, wxID_ANY);
    line->SetBackgroundColour(wxColour(120, 30, 30));
    mainSizer->Add(line, 0, wxEXPAND | wxALL, 15);

    // --- Developer Info ---
    wxStaticText* aboutMe = new wxStaticText(this, wxID_ANY, "Developed by NullSect");
    aboutMe->SetFont(wxFontInfo(14).Bold().FaceName("Segoe UI Semibold"));
    aboutMe->SetForegroundColour(wxColour(255, 214, 214));
    mainSizer->Add(aboutMe, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);

    wxStaticText* bio = new wxStaticText(
        this, wxID_ANY,
        "Cybersecurity student & developer passionate about\n"
        "software security, binary exploitation, and open-source tools.",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    bio->SetFont(wxFontInfo(10).FaceName("Segoe UI"));
    bio->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(bio, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // --- Hyperlinks ---
    wxBoxSizer* linkSizer = new wxBoxSizer(wxHORIZONTAL);

    wxHyperlinkCtrl* githubLink = new wxHyperlinkCtrl(
        this, wxID_ANY, "Visit GitHub", "https://github.com/bilalaniq");
    wxHyperlinkCtrl* sourceLink = new wxHyperlinkCtrl(
        this, wxID_ANY, "Source Code", "https://github.com/bilalaniq/ArcLite");

    wxColour linkNormal(176, 48, 48);
    wxColour linkHover(200, 60, 60);

    githubLink->SetNormalColour(linkNormal);
    githubLink->SetHoverColour(linkHover);
    githubLink->SetVisitedColour(linkNormal);

    sourceLink->SetNormalColour(linkNormal);
    sourceLink->SetHoverColour(linkHover);
    sourceLink->SetVisitedColour(linkNormal);

    linkSizer->Add(githubLink, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(15));
    linkSizer->Add(sourceLink, 0, wxALIGN_CENTER_VERTICAL);
    mainSizer->Add(linkSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

    // --- Load and Display PNG from Resource ---
    wxImage::AddHandler(new wxPNGHandler());

    HRSRC hRes = FindResource(nullptr, MAKEINTRESOURCE(IDB_PNG1), L"PNG");
    if (hRes)
    {
        HGLOBAL hData = LoadResource(nullptr, hRes);
        if (hData)
        {
            DWORD size = SizeofResource(nullptr, hRes);
            void* pData = LockResource(hData);

            if (pData && size > 0)
            {
                wxMemoryInputStream stream(pData, size);
                wxImage image(stream, wxBITMAP_TYPE_PNG);
                if (image.IsOk())
                {
                    wxBitmap bitmap(image);
                    wxStaticBitmap* logo = new wxStaticBitmap(this, wxID_ANY, bitmap);
                    mainSizer->Add(logo, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 15);
                }
            }
        }
    }

    // --- Finalize Layout ---
    SetSizerAndFit(mainSizer);
}
