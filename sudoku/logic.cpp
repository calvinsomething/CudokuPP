#include "logic.h"
#include "frame.h"
#include <stdlib.h>
#include <time.h>

const unsigned int ALLPOSS = 511 << 16;
const unsigned int FINISHED = 1 << 9;

void Logic::Init()
{
	srand(time(NULL));
}

void Logic::Reset()
{
	for (char box = 0; box < 9; box++)
	{
		for (char sq = 0; sq < 9; sq++)
		{
			if (!(Square::values[box][sq] << 16))
				Square::values[box][sq] = ALLPOSS;
		}
	}
}

unsigned int& Logic::FindLeastPoss()
{
	unsigned int* currentLeast = nullptr;
	char leastQuantity = 0;
	char thisQuantity;
	for (char box = 0; box < 9; box++)
	{
		for (char sq = 0; sq < 9; sq++)
		{
			// get blank square
			if (!(Square::values[box][sq] << 16))
			{
				// if there's a blank without any possibilities, no solution exists
				if (Square::values[box][sq] == 0)
				{
					return Square::values[box][sq];
				}
				// initialize lowest possibilities comparison
				if (currentLeast == nullptr)
				{
					currentLeast = &Square::values[box][sq];
					for (char i = 0; i < 9; i++)
					{
						if ((*currentLeast >> (16 + i)) & 1)
							leastQuantity += 1;
					}
					continue;
				}
				// compare possibilities to the current lowest
				else
				{
					thisQuantity = 0;
					for (char i = 0; i < 9; i++)
					{
						if ((Square::values[box][sq] >> (16 + i)) & 1)
							thisQuantity += 1;
					}
					if (thisQuantity < leastQuantity)
					{
						currentLeast = &Square::values[box][sq];
						leastQuantity = thisQuantity;
					}
				}
			}
		}
	}
	if (currentLeast == nullptr)
	{
		Square::values[0][0] |= FINISHED;
		return Square::values[0][0];
	}
	return *currentLeast;
}


bool Logic::Solve()
{
	Reset();
	unsigned int& currentSquare = Naive();
	// if least possibilities is zero, no solution exists
	if (!currentSquare)
	{
		return false;
	}
	if ((currentSquare & FINISHED) == FINISHED)
	{
		currentSquare -= FINISHED;
		return true;
	}
	// force square with least possibilities, trying one possibility at a time
	if (!(currentSquare << 16))
	{
		unsigned int record = currentSquare;
		for (char i = 0; i < 9; i++)
		{
			if ((currentSquare >> (16 + i)) & 1)
			{
				currentSquare = 1 << i;
				if (Solve())
					return true;
				else
					currentSquare = record;
			}
		}
		// if none work, reset value and backtrack
		currentSquare = 0;
		return false;
	}
	// if naive logic operation does not lead to a solution, back track
	if (Solve())
		return true;
	else
	{
		currentSquare = 0;
		return false;
	}
}

unsigned int& Logic::Naive()
{
	unsigned int toRemove;
	unsigned int val;
	// per box
	for (char box = 0; box < 9; box++)
	{
		toRemove = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			val = Square::values[box][sq] << 16;
			if (val)
				// add value of square to toRemove bitset
				toRemove |= val;
		}
		for (char sq = 0; sq < 9; sq++)
		{
			//check if square is blank
			if (!(Square::values[box][sq] << 16))
			{
				// subtract any bits in toRemove from each square's possibilities set
				Square::values[box][sq] -= toRemove & Square::values[box][sq];
				// if square only has one possibility, set value
				if (!(Square::values[box][sq] & (Square::values[box][sq] - 1)))
				{
					Square::values[box][sq] >>= 16;
					return Square::values[box][sq];
				}
			}
		}
	}
	// per row
	char box;
	char square;
	for (char row = 0; row < 9; row++)
	{
		toRemove = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			box = row / 3 * 3 + sq / 3;
			square = sq % 3 + row % 3 * 3;
			val = Square::values[box][square] << 16;
			if (val)
				toRemove |= val;
		}
		for (char sq = 0; sq < 9; sq++)
		{
			box = row / 3 * 3 + sq / 3;
			square = sq % 3 + row % 3 * 3;
			if (!(Square::values[box][square] << 16))
			{
				Square::values[box][square] -= toRemove & Square::values[box][square];
				if (!(Square::values[box][square] & (Square::values[box][square] - 1)))
				{
					Square::values[box][square] >>= 16;
					return Square::values[box][square];
				}
			}
		}
	}
	// per col
	for (char col = 0; col < 9; col++)
	{
		toRemove = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			box = col / 3 + sq / 3 * 3;
			square = (col % 3) + (sq % 3) * 3;
			val = Square::values[box][square] << 16;
			if (val)
				toRemove |= val;
		} 
		for (char sq = 0; sq < 9; sq++)
		{
			box = col / 3 + sq / 3 * 3;
			square = (col % 3) + (sq % 3) * 3;
			if (!(Square::values[box][square] << 16))
			{
				Square::values[box][square] -= toRemove & Square::values[box][square];
				if (!(Square::values[box][square] & (Square::values[box][square] - 1)))
				{
					Square::values[box][square] >>= 16;
					return Square::values[box][square];
				}
			}
		}
	}
	return UniquePoss();
}

unsigned int& Logic::UniquePoss()
{
	unsigned int repeats;
	unsigned int unique;
	// per box
	for (char box = 0; box < 9; box++)
	{
		repeats = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			if (!(Square::values[box][sq] << 16))
			{
				repeats |= (Square::values[box][sq] & repeats) >> 16;
				repeats |= Square::values[box][sq];
			}
		}
		repeats <<= 16;
		for (char sq = 0; sq < 9; sq++)
		{
			// if the square has a possibility that is not in any other possibility set for a sector
			if (!(Square::values[box][sq] << 16))
			{
				unique = Square::values[box][sq] - (Square::values[box][sq] & repeats);
				if (unique)
				{
					Square::values[box][sq] = unique >> 16;
					return Square::values[box][sq];
				}
			}
		}
	}
	// per row
	char box;
	char square;
	for (char row = 0; row < 9; row++)
	{
		repeats = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			box = row / 3 * 3 + sq / 3;
			square = sq % 3 + row % 3 * 3;
			if (!(Square::values[box][square] << 16))
			{
				repeats |= (Square::values[box][square] & repeats) >> 16;
				repeats |= Square::values[box][square];
			}
		}
		repeats <<= 16;
		for (char sq = 0; sq < 9; sq++)
		{
			box = row / 3 * 3 + sq / 3;
			square = sq % 3 + row % 3 * 3;
			if (!(Square::values[box][square] << 16))
			{
				unique = Square::values[box][square] - (Square::values[box][square] & repeats);
				if (unique)
				{
					Square::values[box][square] = unique >> 16;
					return Square::values[box][square];
				}
			}
		}
	}
	// per col
	for (char col = 0; col < 9; col++)
	{
		repeats = 0;
		for (char sq = 0; sq < 9; sq++)
		{
			box = col / 3 + sq / 3 * 3;
			square = (col % 3) + (sq % 3) * 3;
			if (!(Square::values[box][square] << 16))
			{
				repeats |= (Square::values[box][square] & repeats) >> 16;
				repeats |= Square::values[box][square];
			}
		}
		repeats <<= 16;
		for (char sq = 0; sq < 9; sq++)
		{
			box = col / 3 + sq / 3 * 3;
			square = (col % 3) + (sq % 3) * 3;
			if (!(Square::values[box][square] << 16))
			{
				unique = Square::values[box][square] - (Square::values[box][square] & repeats);
				if (unique)
				{
					Square::values[box][square] = unique >> 16;
					return Square::values[box][square];
				}
			}
		}
	}
	// if no value can be set with naive logic, use brute force on square with least possibilities
	return FindLeastPoss();
}

void Logic::Hint()
{
	Reset();
	unsigned int& square = Naive();
	unsigned int copy;
	if (!(square << 16))
	{
		copy = square;
		unsigned int gridCopy[9][9];
		for (char i = 0; i < 9; i++)
		{
			if ((square >> (16 + i)) & 1)
			{
				square = 1 << i;
				CopyGrid(Square::values, gridCopy);
				if (Solve())
				{
					Revert(Square::values, gridCopy);
				}
				else
					square = copy;
			}
		}
	}
	if (!(square << 16))
		square = 0;
}

void Logic::CopyGrid(unsigned int grid[9][9], unsigned int copy[9][9]) 
{
	for (char i = 0; i < 9; i++)
	{
		for (char j = 0; j < 9; j++)
		{
			copy[i][j] = grid[i][j];
		}
	}
}

void Logic::Revert(unsigned int grid[9][9], const unsigned int copy[9][9])
{
	for (char i = 0; i < 9; i++)
	{
		for (char j = 0; j < 9; j++)
		{
			grid[i][j] = copy[i][j];
		}
	}
}

template <char N>
void Logic::Randomize(char (&numbers)[N])
{
	for (char i = 0; i < N; i++)
	{
		numbers[i] = i;
	}
	char temp;
	char rIndex;
	for (char i = 0; i < N; i++)
	{
		rIndex = rand() % N;
		temp = numbers[i];
		numbers[i] = numbers[rIndex];
		numbers[rIndex] = temp;
	}
}

void Logic::NewGame(char&& difficulty)
{
	char top[9];
	char bottom[9];
	char squares[81];
	Randomize(top);
	Randomize(bottom);
	Randomize(squares);
	for (char i = 0; i < 9; i++)
	{
		Square::values[0][i] = 1 << top[i];
		Square::values[8][i] = 1 << bottom[i];
	}
	Solve();
	char box, sq, forces = 0;
	if (difficulty == 'M')
		forces = 15;
	if (difficulty == 'H')
		forces = 81;
	for (char i = 0; i < 81; i++)
	{
		box = squares[i] / 9;
		sq = squares[i] - (box * 9);
		Erase(box, sq, forces);
	}
}

void Logic::Erase(char& box, char& sq, char& forces)
{
	unsigned int copy = Square::values[box][sq];
	Square::values[box][sq] = 0;
	Reset();
	unsigned int vals = 0;
	for (char i = 0; i < 9; i++)
	{
		if (Square::values[box][i] << 16)
		{
			vals |= Square::values[box][i] << 16;
		}
	}
	Square::values[box][sq] -= vals;
	if (!(Square::values[box][sq] & (Square::values[box][sq] - 1)))
	{
		Square::values[box][sq] = 0;
		return;
	}
	vals = 0;
	char row = box / 3 * 3 + sq / 3;
	char b, s;
	for (char i = 0, b; i < 9; i++)
	{
		b = row / 3 * 3 + i / 3;
		s = i % 3 + (row % 3) * 3;
		if (Square::values[b][s] << 16)
		{
			vals |= Square::values[b][s] << 16;
		}
	}
	Square::values[box][sq] -= Square::values[box][sq] & vals;
	if (!(Square::values[box][sq] & (Square::values[box][sq] - 1)))
	{
		Square::values[box][sq] = 0;
		return;
	}
	vals = 0;
	char col = (box % 3) * 3 + sq % 3;
	for (char i = 0; i < 9; i++)
	{
		b = col / 3 + i / 3 * 3;
		s = col % 3 + (i % 3) * 3;
		if (Square::values[b][s] << 16)
		{
			vals |= Square::values[b][s] << 16;
		}
	}
	Square::values[box][sq] -= Square::values[box][sq] & vals;
	if (!(Square::values[box][sq] & (Square::values[box][sq] - 1)))
	{
		Square::values[box][sq] = 0;
		return;
	}
	// check for unique possibility
	unsigned int repeats = 0;
	for (char i = 0; i < 9; i++)
	{
		if (i != sq)
			repeats |= Square::values[box][i];
		
	}
	if (Square::values[box][sq] - (Square::values[box][sq] & repeats))
	{
		Square::values[box][sq] = 0;
		return;
	}
	repeats = 0;
	for (char i = 0, b; i < 9; i++)
	{
		b = row / 3 * 3 + i / 3;
		s = i % 3 + (row % 3) * 3;
		if (b != box || s != sq)
			repeats |= Square::values[box][i];
	}
	if (Square::values[box][sq] - (Square::values[box][sq] & repeats))
	{
		Square::values[box][sq] = 0;
		return;
	}
	repeats = 0;
	for (char i = 0, b; i < 9; i++)
	{
		b = col / 3 + i / 3 * 3;
		s = col % 3 + (i % 3) * 3;
		if (b != box || s != sq)
			repeats |= Square::values[box][i];
	}
	if (Square::values[box][sq] - (Square::values[box][sq] & repeats))
	{
		Square::values[box][sq] = 0;
		return;
	}

	if (forces)
	{
		if (OneSolution(Square::values[box][sq]))
		{
			Square::values[box][sq] = 0;
			forces--;
			return;
		}
	}

	Square::values[box][sq] = copy;
}

bool Logic::OneSolution(unsigned int& square)
{
	char solutions = 0;
	unsigned int gridCopy[9][9];
	CopyGrid(Square::values, gridCopy);
	for (char i = 0; i < 9; i++)
	{
		if ((square >> (16 + i)) & 1)
		{
			square = 1 << i;
			if (Solve())
			{
				Revert(Square::values, gridCopy);
				solutions += 1;
				if (solutions > 1)
					return false;
				continue;
			}
			else
				Revert(Square::values, gridCopy);
		}
	}
	return solutions == 1;
}
