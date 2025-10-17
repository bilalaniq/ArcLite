#include "aboutPanel.h"
#include <wx/hyperlink.h>
#include <wx/statline.h>

aboutPanel::aboutPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    // Set main background
    SetBackgroundColour(wxColour(43, 26, 26)); // Dark maroon background
    CreateLayout();
}

void aboutPanel::CreateLayout()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // --- Title ---
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "ArcLite");
    title->SetFont(wxFontInfo(22).Bold().FaceName("Segoe UI Semibold"));
    title->SetForegroundColour(wxColour(255, 214, 214)); // light pinkish
    mainSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 25);

    // --- Version ---

#ifdef _WIN64
    const char* arch = "64-bit";
#else 
    const char* arch = "32-bit";
#endif

    wxString versionstr = wxString::Format("Version 1.0.0 (%s)", arch);
    wxStaticText* version = new wxStaticText(this, wxID_ANY, versionstr);
    version->SetFont(wxFontInfo(11).FaceName("Segoe UI"));
    version->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(version, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);

    // --- Description ---
    wxStaticText* desc = new wxStaticText(
        this,
        wxID_ANY,
        "A fast and reliable ZIP compression tool\n"
        "built with wxWidgets for secure file management.",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER
    );
    desc->SetFont(wxFontInfo(10).FaceName("Segoe UI"));
    desc->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(desc, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 15);

    // --- Separator ---
    wxStaticLine* line = new wxStaticLine(this, wxID_ANY);
    line->SetBackgroundColour(wxColour(120, 30, 30));
    mainSizer->Add(line, 0, wxEXPAND | wxALL, 15);

    // --- About Me ---
    wxStaticText* aboutMe = new wxStaticText(this, wxID_ANY, "Developed by NullSect");
    aboutMe->SetFont(wxFontInfo(14).Bold().FaceName("Segoe UI Semibold"));
    aboutMe->SetForegroundColour(wxColour(255, 214, 214));
    mainSizer->Add(aboutMe, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);

    wxStaticText* bio = new wxStaticText(this, wxID_ANY,
        "Cybersecurity student & developer passionate about\n"
        "software security, binary exploitation, and open-source tools." ,
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER);
    bio->SetFont(wxFontInfo(10).FaceName("Segoe UI"));
    bio->SetForegroundColour(wxColour(229, 191, 191));
    mainSizer->Add(bio, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // --- Hyperlink ---



    wxBoxSizer* linksizer = new wxBoxSizer(wxHORIZONTAL);

    wxHyperlinkCtrl* link = new wxHyperlinkCtrl(
        this, wxID_ANY,
        "Visit GitHub",
        "https://github.com/bilalaniq"
    );

    wxHyperlinkCtrl* sourcecodelink = new wxHyperlinkCtrl(
        this, wxID_ANY,
        "Visit Sourcecode",
        "https://github.com/bilalaniq/ArcLite"
    );

    link->SetNormalColour(wxColour(176, 48, 48));
    link->SetHoverColour(wxColour(200, 60, 60));
    link->SetVisitedColour(wxColour(176, 48, 48));

    sourcecodelink->SetNormalColour(wxColour(176, 48, 48));
    sourcecodelink->SetHoverColour(wxColour(200, 60, 60));
    sourcecodelink->SetVisitedColour(wxColour(176, 48, 48));

    linksizer->Add(link, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(15));
    linksizer->Add(sourcecodelink, 0, wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(linksizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

    SetSizerAndFit(mainSizer);
}
