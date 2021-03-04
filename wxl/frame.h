#pragma once
#include <wx/wxprec.h>
#include "Square.h"
#include "id.h"



class Frame : public wxFrame
{
public:
	Frame();
	~Frame();
	static wxImage images[9];
private:
	void SetGame();
	void Update();
	void OnAbout(wxCommandEvent&);
	void OnSolve(wxCommandEvent&);
	void OnClear(wxCommandEvent&);
	void OnHint(wxCommandEvent&);
	void NewEasyGame(wxCommandEvent&);
	void NewMediumGame(wxCommandEvent&);
	void NewHardGame(wxCommandEvent&);
	void LoadImages();
	void OnResize(wxSizeEvent&);
	wxPanel* InnerFrame;
	wxPanel* boxes[9];
	Square* squares[9][9];
	wxGridSizer* squareGrids[9];
};