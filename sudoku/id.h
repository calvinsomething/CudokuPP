#pragma once
#include <wx/wxprec.h>

enum id
{
	WINDOW = 0,
	HINT,
	SOLVE,
	CLEAR,
	EASY,
	MEDIUM,
	HARD,
	IMAGES,
	BOXES = IMAGES + 9,
	SQUARES = BOXES * 9,
	NOTES = SQUARES + 91
};