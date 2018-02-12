#pragma once

#include <windows.h>

class game
{
public:

	bool isGameActive = false;
	bool isWin = false;
	bool isFail = false;

	enum Arrows
	{
		Arrow_Up = 0,
		Arrow_Right = 1,
		Arrow_Down = 2,
		Arrow_Left = 3
	};

	static const unsigned char FIELD_SIZE = 4;

	game();
	void shiftTo(unsigned char direction);

protected:
	HANDLE consoleHandle = 0;

	unsigned short field[FIELD_SIZE][FIELD_SIZE] = { 0 }; // игровое поле, инициируем нул€ми, т.е. все €чейки пусты
	unsigned int score = 0; // счЄт игры

	bool checkAndCollapse(unsigned short *from, unsigned short *to, bool *isSmthChanged);
	void alignToBorder(unsigned short *from, unsigned short *to, bool *isSmthChanged);
	void render();
	void spawn();
	void checkForWin(unsigned short number);
	void checkForFail();
};

