//Made by Ferdi Helvensteijn for the personal project: 4x4 handheld w/ Yves Kersten

//This is a simple showcase of how to create a minesweeper game with the Minesweeper library provided in this project.
//It is not intended for actual use, it only exists to test the library out before an implementation is created for the
//4x4 handheld.

#include <iostream>
#include <cstdlib>
#include "MinesweeperLibAF.cpp"

using namespace std;

int main() {
	int width;
	int height;
	int mod;
	int selWidth;
	int selHeight;
	char selFlag = 'N';

	cout << "\nchoose board width: ";
	cin >> width;
	cout << "\nchoose board height: ";
	cin >> height;

	// Put minesweeper here once the methods have been created
	cout << "\nchoose bomb amount: ";
	cin >> mod;
	if (width * height <= mod) {
		cout << "\ntoo many mines!!";
		return -1;
	}
	//fill surface with @
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			cout << "@";
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "choose row: ";
	cin >> selWidth;
	cout << "\nchoose column: ";
	cin >> selHeight;

	Minesweeper game = Minesweeper(width, height, mod, selWidth, selHeight);

	while (!game.exploded) {
		if (selFlag == 'N') {
			game.DigTile(selWidth, selHeight, true);
		}
		else {
			game.PlantFlag(selWidth, selHeight);
		}

		cout << "\n";
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				cout << game.surface[i][j];
			}
			cout << "\n";
		}

		cout << "\n";

		//shows the underground layer for debug
		//for (int i = 0; i < width; i++) {
		//	for (int j = 0; j < height; j++) {
		//		cout << game.underground[i][j];
		//	}
		//	cout << "\n";
		//}

		//cout << "\n";

		if (game.exploded) {
			cout << "kaboom";
			return 0;
		}
		if (game.checkWin()) {
			cout << "you win!";
			return 0;
		}

		cout << "choose row: ";
		cin >> selWidth;
		cout << "\nchoose column: ";
		cin >> selHeight;
		cout << "\nflag? Y/N : ";
		cin >> selFlag;
	}
	return 0;
}
