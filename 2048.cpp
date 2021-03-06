// 2048.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>

int main()
{
	game* gameController = new game();

	// игровой цикл будет повторяться, пока ответственный флаг в true
	while (gameController->isGameActive) {
		switch (int(_getch()))
		{
		case 113: // нажата клавиша "q", инициируем завершение игры
			gameController->isGameActive = false;
			break;
		case 72:    // нажата клавиша "вверх"
			gameController->shiftTo(gameController->Arrow_Up);
			break;
		case 80:    // нажата клавиша "вниз"
			gameController->shiftTo(gameController->Arrow_Down);
			break;
		case 77:    // нажата клавиша "вправо"
			gameController->shiftTo(gameController->Arrow_Right);
			break;
		case 75:    // нажата клавиша "влево"
			gameController->shiftTo(gameController->Arrow_Left);
			break;
		}
	}

	puts("\nPress any key to exit...                ");
	_getch();

	return 0;
}
