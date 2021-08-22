#pragma once
#include <wx/wxprec.h>
#include "Label.h"

class Square : public wxPanel
{
	enum
	{
		one = 1,
		two,
		three = 4,
		four = 8,
		five = 16,
		six = 32,
		seven = 64,
		eight = 128,
		nine = 256
	};
	int notes = 0;
	char value = 0;
	Label text;
	wxFont font;
public:
	bool pencil;
	static unsigned int values[9][9];
	Square(wxWindow*, int);
	~Square();
	void Render(wxDC&);
	void OnPaint(wxPaintEvent&);
	void EnterNumber(wxKeyEvent&);
	void SetValue(unsigned int&, bool);
	void OnClick(wxMouseEvent&);
};

