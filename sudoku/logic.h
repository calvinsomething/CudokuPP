#pragma once

namespace Logic
{
	void Init();
	void Reset();
	unsigned int& Naive();
	unsigned int& UniquePoss();
	unsigned int& FindLeastPoss();
	bool Solve();
	void Hint();
	void NewGame(char&&);
	bool OneSolution(unsigned int&);
	void CopyGrid(unsigned int[9][9], unsigned int[9][9]);
	void Revert(unsigned int[9][9], const unsigned int[9][9]);
	template <char N> void Randomize(char(&)[N]);
	void Erase(char&, char&, char&);
}

