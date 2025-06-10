#include <vector>
#include <time.h>
#include <stack>
#include "MinesweeperLibAF.h"

//Made by Ferdi Helvensteijn for the personal project: 4x4 handheld w/ Yves Kersten

//This class and its methods can be used to create an implementation of the puzzle game Minesweeper
//For an example of how to implement check out the in console implementation created in main.cpp

Minesweeper::Minesweeper(int x, int y, int mines, int selX, int selY) {

	//create initialization variables and set random seed
	std::vector<std::vector<char>> initSurface(x, std::vector<char>(y, '@'));
	std::vector<std::vector<char>> initUnderground(x, std::vector<char>(y, '0'));
	srand(time(NULL));
	int randX;
	int randY;

	//allign selected X and Y with the array
	selX--;
	selY--;

	//fill underground with mines
	int tempMines = mines;
	while (tempMines > 0) {
		randX = rand() % x;
		randY = rand() % y;
		if (!(randX == selX && randY == selY) && initUnderground[randX][randY] != 'X') {
			initUnderground[randX][randY] = 'X';
			//increment the values around the mine by 1 (if not already a mine)
			for (int i = randX-1; i < randX+2; i++) {
				for (int j = randY-1; j < randY+2; j++) {
					if (i >= 0 && j >= 0 && i < x && j < y && initUnderground[i][j] != 'X') {
						initUnderground[i][j] = static_cast<char> (static_cast<int>(initUnderground[i][j] + 1));		
					}
				}
			}
			tempMines--;
		}
	}

	//fill class values with set variables
	surface = initSurface;
	underground = initUnderground;
	width = x;
	height = y;
	bombs = mines;
	exploded = false;
}


//allows all unflagged cells around a number to be opened when the correct amount of flags are around it
void Minesweeper::Chording(int x, int y) {
	//Init variables required for function
	int danger = static_cast<int>(this->surface[x][y] - '0'); //amount of bombs around the tile
	std::vector<int> spots(16);								  //Array to hold undug tile positions around the chorded tile
	int k = 0;												  //Keeps track of the amount of undug tiles around the chorded tile
	int flags = 0;											  //Keeps track of flags around the chorded tile

	//Iterate around the chorded tile
	for (int i = x-1; i < x+2; i++) {
		for (int j = y-1; j < y+2; j++) {
			if (!OutOfBound(i,j)) {
				//store position of undug tiles
				if (this->surface[i][j] == '@') {
					spots[k * 2] = i;
					spots[(k * 2) + 1] = j;
					k++;
				}
				//count amount of flagged tiles
				if (this->surface[i][j] == 'F') flags++;
			}
		}
	}

	//If flags are equal to the amount of bombs around the tile dig up all the stored tile positions
	if (flags == danger) {
		for (int i = 0; i <= k; i++) {
			DigTile(spots[i*2]+1, spots[(i*2)+1]+1, false);
		}
	}
}

bool Minesweeper::OutOfBound(int x, int y) {
	return (x < 0 || y < 0 || x >= this->width || y >= this->height);
}

//Digs up a tile on position x-1, y-1. Bool chording controls if chording should be allowed to occur
void Minesweeper::DigTile(int x, int y, bool chording) {

	//shift input position to array position
	x--;
	y--;

	//check if position within board
	if (OutOfBound(x, y)) return;


	//If flagged do nothing and return
	if (this->surface[x][y] == 'F') return;

	//If clicked on a number and chording is on attempt chording
	if (this->surface[x][y] != '@' && this->surface[x][y] != 'F' && chording) {
		Chording(x, y);
	}

	//Build a stack, if tile dug up is 0 this will be used to add the undug tiles around the 0 tile to be dug up
	std::stack<int> stack;

	stack.push(x);
	stack.push(y);
			
	//Keep looping until the stack is empty
	while (!stack.empty()) {

		y = stack.top();
		stack.pop();
		x = stack.top();
		stack.pop();

		switch (this->underground[x][y]) {
			//set gamestate to exploded, you lose
			case 'X': {
				this->surface[x][y] = 'X';
				exploded = true;
				break;
			}
			//add tiles around to the stack, this lets them be checked later
			case '0': {
				this->surface[x][y] = '0';
				for (int i = x - 1; i < x + 2; i++) {
					for (int j = y - 1; j < y + 2; j++) {
						if (!OutOfBound(i,j)) { 
							if (this->surface[i][j] == '@') { 
								this->surface[i][j] = 'Q';								//change to Q to avoid it being thrown on the stack multiple times
								stack.push(i);
								stack.push(j);
							}
						}
					}
				}
				break;
			}
			//if its a number just add the number to the surface
			default: {
				this->surface[x][y] = this->underground[x][y];
				break;
			}
		}
	}
}

//Plants a flag on a tile if it is undug, removes a flag on a tile if there is a flag
void Minesweeper::PlantFlag(int x, int y) {
	x--;
	y--;

	if (OutOfBound(x,y)) return;

	if (this->surface[x][y] == '@') this->surface[x][y] = 'F';
	else if (this->surface[x][y] == 'F') this->surface[x][y] = '@';
}

//checks if a board has been won
bool Minesweeper::CheckWin() {
	int undug = 0;

	//count undug tiles
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (this->surface[i][j] == '@' || this->surface[i][j] == 'F') undug++;
		}
	}

	//if undug tilles == bombs then the game has been won
	return (undug == this->bombs);
}