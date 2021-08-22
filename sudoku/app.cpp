#include "app.h"

wxIMPLEMENT_APP(App);

App::App() {}

App::~App() {}

bool App::OnInit()
{
	wxInitAllImageHandlers();
	Frame* window = new Frame;
	window->Show(true);
	return true;
}