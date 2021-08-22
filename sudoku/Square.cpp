#include "Square.h"
#include "id.h"
#include "frame.h"

unsigned int Square::values[9][9];

Square::Square(wxWindow* parent, int ID) : wxPanel(parent, ID, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	Bind(wxEVT_PAINT, &Square::OnPaint, this);
	Bind(wxEVT_LEFT_DOWN, &Square::OnClick, this);
	Bind(wxEVT_CHAR, &Square::EnterNumber, this);
	text.Create(this, wxID_ANY, "");
	font = text.GetFont();
}

void Square::Render(wxDC& dc)
{
	int size = GetSize().GetWidth();
	if (value > 0)
	{
		if (pencil)
		{
			text.SetLabel("");
			dc.DrawBitmap(Frame::images[value - 1].Scale(size * 0.8, size * 0.8), size * 0.1, size * 0.1);
		}
		else
		{
			text.SetPosition(wxPoint(size * 0.3, 0));
			font.SetPointSize(size/2);
			text.SetFont(font);
			char temp[2];
			_itoa(value, temp, 10);
			text.SetLabel(temp);
		}
	}
	else if (notes == 0)
	{
		dc.Clear();
		text.SetLabel("");
	}
	else
		for (int note = one, x = 0; note < 257; note *= 2)
		{
			if (note & notes)
				dc.DrawBitmap(Frame::images[x].Scale(size / 3, size / 3), (x % 3) * size / 3, (x / 3) * size / 3);
			x++;
		}
}

void Square::OnPaint(wxPaintEvent& e)
{
	wxPaintDC dc(this);
	Render(dc);
}

void Square::OnClick(wxMouseEvent& event)
{
	if (value == 0 && HasFocus())
	{
		int size = GetSize().GetWidth();
		int sectX = event.GetX() / (size / 3);
		int sectY = event.GetY() / (size / 3);

		// Identify sector based on one number
		int pos = sectX + sectY * 3;

		int note = 1;

		note <<= pos;

		notes ^= note;
		Refresh();
	}
	event.Skip();
}

void Square::EnterNumber(wxKeyEvent& event)
{
	pencil = true;
	int id = event.GetId();
	int i = (id - id::SQUARES) / 9;
	int j = (id - id::SQUARES) % 9;
	wxChar keyPressed = event.GetUnicodeKey();
	if (wxIsdigit(keyPressed))
	{
		values[i][j] = unsigned int(1) << (wxAtoi(keyPressed) - 1);
		SetValue(values[i][j], true);
	}
	else if (keyPressed == WXK_BACK)
	{
		values[i][j] = 0;
		SetValue(values[i][j], true);
	}
	event.Skip();
}

void Square::SetValue(unsigned int& value, bool pencil)
{
	this->pencil = pencil;
	for (size_t i = 0; i < 9; i++)
	{
		if ((value >> i) & 1)
		{
			notes = 0;
			this->value = i + 1;
			Refresh();
			return;
		}
	}
	this->value = 0;
	notes = 0;
	Refresh();
}

Square::~Square()
{
	value = NULL;
}

