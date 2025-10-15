#include "App.h"
#include "Mainframe.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	Mainframe* mainframe = new Mainframe("ArchLite");
	mainframe->SetClientSize(800, 600);
	mainframe->Center();
	mainframe->Show();
	return true;
}
