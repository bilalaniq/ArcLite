#pragma once


#include <wx/dir.h>
#include <wx/filename.h>

class IgnoringTraverser : public wxDirTraverser
{
public:
	std::function<bool(const std::string&)> ShouldIgnoreDir;  
	std::function<void(const std::string&)> FileEnterCallback;


	virtual wxDirTraverseResult OnFile(const wxString& filename) {  // It’s automatically called by wxWidgets for every file found when you use wxDir::Traverse() to walk through a directory.
		FileEnterCallback(filename.ToStdString());
		return wxDIR_CONTINUE;  // wxDIR_CONTINUE tells wxWidgets to keep scanning the rest of the files
	}


	virtual wxDirTraverseResult OnDir(const wxString& dirname) {  // wxWidgets automatically calls it for every directory (folder) it encounters during a recursive directory traversal.

		wxString lastPathComponent = wxFileName(dirname).GetFullName();

		return ShouldIgnoreDir(lastPathComponent.ToStdString()) ? wxDIR_IGNORE : wxDIR_CONTINUE;
	}
};
