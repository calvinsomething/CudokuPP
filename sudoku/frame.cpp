#include "frame.h"
#include <iostream>
#include "logic.h"

#define COLOR1 30, 125, 50
#define COLOR2 125, 200, 150

wxImage Frame::images[9];

Frame::Frame() : wxFrame(NULL, id::WINDOW, "SUDOKU")
{
	SetMinSize(wxSize(400, 468));
	SetSize(wxSize(400, 468));
	wxMenu* menuFile = new wxMenu;
	wxMenu* newGame = new wxMenu;
	newGame->Append(id::EASY, "Easy", "Start a game of easy difficulty.");
	newGame->Append(id::MEDIUM, "Medium", "Start a game of medium difficulty.");
	newGame->Append(id::HARD, "Hard", "Start a game of hard difficulty.");
	menuFile->Append(id::HINT, "&Hint\tCtrl-H", "Solve one square.");
	menuFile->Append(id::SOLVE, "&Solve\tCtrl-S", "Solve entire puzzle.");
	menuFile->Append(id::CLEAR, "&Clear\tCtrl-C", "Clear grid.");
	menuFile->AppendSeparator();
	menuFile->AppendSubMenu(newGame, "&New Game\tCtrl-N", "Select the difficulty of your next game.");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to Sudoku Player!");

	wxBoxSizer* OuterSizer = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(OuterSizer);

	InnerFrame = new wxPanel(this, wxID_ANY);
	InnerFrame->SetBackgroundColour(wxColor(150, 150, 150));
	OuterSizer->AddStretchSpacer(.1);
	OuterSizer->Add(InnerFrame, 1, wxSHAPED | wxALIGN_CENTER);
	OuterSizer->AddStretchSpacer(.1);

	wxGridSizer* boxGrid = new wxGridSizer(3, 3, 5, 5);
	LoadImages();

	for (int i = 0; i < 9; i++)
	{
		boxes[i] = new wxPanel(InnerFrame, id::BOXES + i);
		if (i == 0 || i % 2 == 0)
			boxes[i]->SetBackgroundColour(wxColor(COLOR1));
		else
			boxes[i]->SetBackgroundColour(wxColor(COLOR2));
		boxGrid->Add(boxes[i], 1, wxEXPAND);

		squareGrids[i] = new wxGridSizer(3, 3, 2, 2);

		for (int j = 0; j < 9; j++)
		{
			int iD = id::SQUARES + i * 9 + j;
			squares[i][j] = new Square(boxes[i], iD);
			squares[i][j]->SetBackgroundColour(wxColor(255, 255, 255));
			squareGrids[i]->Add(squares[i][j], 1, wxSHAPED);
		}
		boxes[i]->SetSizer(squareGrids[i]);
		squareGrids[i]->Layout();
	}
	
	Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &Frame::OnClear, this, id::CLEAR);
	Bind(wxEVT_MENU, &Frame::OnSolve, this, id::SOLVE);
	Bind(wxEVT_MENU, &Frame::OnHint, this, id::HINT);
	Bind(wxEVT_MENU, &Frame::NewEasyGame, this, id::EASY);
	Bind(wxEVT_MENU, &Frame::NewMediumGame, this, id::MEDIUM);
	Bind(wxEVT_MENU, &Frame::NewHardGame, this, id::HARD);
	Bind(wxEVT_MENU, [=](wxCommandEvent&) { Close(true); }, wxID_EXIT);
	Bind(wxEVT_SIZE, &Frame::OnResize, this, wxID_ANY);

	InnerFrame->SetSizer(boxGrid);
	InnerFrame->Center();
	boxGrid->Layout();
	OuterSizer->Layout();

	Logic::Init();
}

Frame::~Frame() {}

void Frame::SetGame()
{
	bool pencil;
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (Square::values[i][j] << 16)
				pencil = false;
			else
				pencil = true;
			squares[i][j]->SetValue(Square::values[i][j], pencil);
		}
	}
}

void Frame::Update()
{
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			squares[i][j]->SetValue(Square::values[i][j], squares[i][j]->pencil);
		}
	}
}

void Frame::OnSolve(wxCommandEvent&)
{
	Logic::Solve();
	SetGame();
}

void Frame::OnHint(wxCommandEvent& event)
{
	Logic::Hint();
	Update();
}

void Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("Made by Calvin Storoschuk.", "About Sudoku Player:", wxOK | wxICON_INFORMATION);
}

void Frame::NewEasyGame(wxCommandEvent& event)
{
	OnClear(event);
	Logic::NewGame('E');
	SetGame();
}

void Frame::NewMediumGame(wxCommandEvent& event)
{
	OnClear(event);
	Logic::NewGame('M');
	SetGame();
}

void Frame::NewHardGame(wxCommandEvent& event)
{
	OnClear(event);
	Logic::NewGame('H');
	SetGame();
}

void Frame::OnClear(wxCommandEvent& event)
{
	for (size_t box = 0; box < 9; box++)
	{
		for (size_t sq = 0; sq < 9; sq++)
		{
			Square::values[box][sq] = 0;
			squares[box][sq]->SetValue(Square::values[box][sq], true);
		}
	}
}

void Frame::OnResize(wxSizeEvent& event)
{
	int w = event.GetSize().GetWidth();
	SetSize(wxSize(w, w+68));

	event.Skip();
}

void Frame::LoadImages()
{
	char file[13];
	for (int i = 0; i < 9; i++)
	{
		sprintf_s(file, "images/%d.png", i+1);
		images[i].LoadFile(file, wxBITMAP_TYPE_PNG);
	}
}