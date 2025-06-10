#pragma once
#include<vector>

class Minesweeper {
public:
	std::vector<std::vector<char>> surface;			//surface layer (what the player sees)
	std::vector<std::vector<char>> underground;		//Underground layer (mines and danger level)
	int width;										//Board width
	int height;										//Board height
	int bombs;										//Amount of bombs
	bool exploded;									//Bool for tracking loss state

	Minesweeper(int x, int y, int mines, int selX, int selY);

	bool OutOfBound(int x, int y);
	
	void Chording(int x, int y);

	void DigTile(int x, int y, bool chording);

	void PlantFlag(int x, int y);

	bool CheckWin();
};