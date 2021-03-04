#pragma once
#include <wx/wxprec.h>
class Label : public wxStaticText
{
	void OnFocus(wxMouseEvent& event)
	{
		GetParent()->SetFocus();
		event.Skip();
	}
public:
	Label()
	{
		Bind(wxEVT_LEFT_DOWN, &Label::OnFocus, this);
	}
};