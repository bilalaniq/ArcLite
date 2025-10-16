#pragma once


#include <wx/dir.h>
#include <wx/filename.h>

class IgnoringTraverser : public wxDirTraverser
{
public:
	std::function<bool(const std::string&)> ShouldIgnoreDir;  
	std::function<void(const std::string&)> FileEnterCallback;
	std::function<void(const std::string&)> DirEnterCallback;


	virtual wxDirTraverseResult OnFile(const wxString& filename) {  // It’s automatically called by wxWidgets for every file found when you use wxDir::Traverse() to walk through a directory.
		FileEnterCallback(filename.ToStdString());
		return wxDIR_CONTINUE;  // wxDIR_CONTINUE tells wxWidgets to keep scanning the rest of the files
	}


    virtual wxDirTraverseResult OnDir(const wxString& dirname) override
    {
        wxString lastPathComponent = wxFileName(dirname).GetFullName();

        if (ShouldIgnoreDir && ShouldIgnoreDir(lastPathComponent.ToStdString()))
            return wxDIR_IGNORE;

        // ? Report this directory to the caller (even if it’s empty)
        if (DirEnterCallback)
            DirEnterCallback(dirname.ToStdString());

        return wxDIR_CONTINUE;
    }
};
