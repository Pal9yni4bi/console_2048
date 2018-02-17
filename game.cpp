#include "stdafx.h"
#include "time.h"  // ��� ��������� ����������� srand
#include "consoleColor.h"

game::game()
{
	system("cls"); // ������� ������� �� ������, ���� ����� ��� �����
	srand(time(0)); // ������������ ��������� ��������� ������� rand
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // �������� ���������� ������� � �������
	// ������� �������� ������� �����
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(consoleHandle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &structCursorInfo);

	isGameActive = true; // ������������� ���� ���� � �������� ���������

	// ������� ������: ����� � �����
	puts("-------------------------");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("-------------------------");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("-------------------------");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("-------------------------");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("|     |     |     |     |");
	puts("-------------------------\n\n");
	puts("Score: 0\n\n");
	puts("Press arrows for play, press Q for exit.");

	// ���������� 2 ��������� �������� � ������ ������
	game::spawn();
	game::spawn();

	// ������������ ������������ ���������
	game::render();
}

void game::shiftTo(unsigned char direction) {
	int i, y, inner; // ���������
	bool isSmthChanged = false; // ����������, ���������� �� �������� �� ���� � ���������� ����

	switch (direction)
	{
	case Arrow_Up:
		// i - ����� ������, y - ����� ������� �������
		for (i = 0; i < FIELD_SIZE; i++) {
			// ����� �������� �������������� ����������������� ��������������� �����, 
			// �������� ��������������� � ����������� �� 2 ����������� �����
			for (y = 0; y < FIELD_SIZE; y++) {
				// ������������ ������ �� ����� � ������������� �������, ���� ��� ���������
				if (field[i][y] > 0) {
					inner = i;
					while (inner < FIELD_SIZE - 1) {
						inner++;
						if (game::checkAndCollapse(&field[inner][y], &field[i][y], &isSmthChanged)) {
							break; // ���� ����������� ������ ���������������, ��������� � ���������
						}
					}
				}
			}
			// ���� ������ �� ������ - �������� � �� ����� �����
			for (y = 0; y < FIELD_SIZE; y++) {
				if (field[i][y] > 0) {
					inner = i;
					while (inner > 0) {
						game::alignToBorder(&field[inner][y], &field[inner - 1][y], &isSmthChanged);
						inner--;
					}
				}
			}
		}
		break;
	case Arrow_Right:
		for (i = FIELD_SIZE - 1; i >= 0; i--) {
			// ����� �������� �������������� ����������������� ��������������� �����, 
			// �������� ��������������� � ����������� �� 2 ����������� �����
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				// ������������ ������ �� ����� � ������������� �������, ���� ��� ���������
				if (field[i][y] > 0) {
					inner = y;
					while (inner > 0) {
						inner--;
						if (game::checkAndCollapse(&field[i][inner], &field[i][y], &isSmthChanged)) {
							break; // ���� ����������� ������ ���������������, ��������� � ���������
						}
					}
				}
			}
			// ���� ������ �� ������ - �������� � �� ����� ������
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				if (field[i][y] > 0) {
					inner = y;
					while (inner < FIELD_SIZE - 1) {
						game::alignToBorder(&field[i][inner], &field[i][inner + 1], &isSmthChanged);
						inner++;
					}
				}
			}
		}
		break;
	case Arrow_Down:
		// i - ����� ������, y - ����� ������� �������
		for (i = FIELD_SIZE - 1; i >= 0; i--) {
			// ����� �������� �������������� ����������������� ��������������� �����, 
			// �������� ��������������� � ����������� �� 2 ����������� �����
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				// ������������ ������ �� ����� � ������������� �������, ���� ��� ���������
				if (field[i][y] > 0) {
					inner = i;
					while (inner > 0) {
						inner--;
						if (game::checkAndCollapse(&field[inner][y], &field[i][y], &isSmthChanged)) {
							break; // ���� ����������� ������ ���������������, ��������� � ���������
						}
					}
				}
			}
			// ���� ������ �� ������ - �������� � �� ����� ����
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				if (field[i][y] > 0) {
					inner = i;
					while (inner < FIELD_SIZE - 1) {
						game::alignToBorder(&field[inner][y], &field[inner + 1][y], &isSmthChanged);
						inner++;
					}
				}
			}
		}
		break;
	case Arrow_Left:
		// i - ����� ������, y - ����� ������� �������
		for (i = 0; i < FIELD_SIZE; i++) {
			// ����� �������� �������������� ����������������� ��������������� �����, 
			// �������� ��������������� � ����������� �� 2 ����������� �����
			for (y = 0; y < FIELD_SIZE; y++) {
				// ������������ ������ �� ����� � ������������� �������, ���� ��� ���������
				if (field[i][y] > 0) {
					inner = y;
					while (inner < FIELD_SIZE - 1) {
						inner++;
						if (game::checkAndCollapse(&field[i][inner], &field[i][y], &isSmthChanged)) {
							break; // ���� ����������� ������ ���������������, ��������� � ���������
						}
					}
				}
			}
			// ���� ������ �� ������ - �������� � �� ����� �����
			for (y = 0; y < FIELD_SIZE; y++) {
				if (field[i][y] > 0) {
					inner = y;
					while (inner > 0) {
						game::alignToBorder(&field[i][inner], &field[i][inner - 1], &isSmthChanged);
						inner--;
					}
				}
			}
		}
		break;
	}

	if (isSmthChanged) {
		// ����� �������� �� ����� ����������, ������ ���� ���� ���� �����-�� ��������� �� ������� ����
		game::spawn();
		// ��������� ����� ����� ������ ���� ���� ���� �����-�� ���������
		game::render();
	}

}

bool game::checkAndCollapse(unsigned short *from, unsigned short *to, bool *isSmthChanged) {
	if (*from == *to) {
		*isSmthChanged = true; // ��������� ���� ��������� �� ������� ����
		*to *= 2; // ���������� ��������� �������� �������� ����� � �������
		*from = 0; // ������� �������� �� ������-������
		score += *to; // ����������� ������� ����
		game::checkForWin(*to); // ���������, ���������� �� ������� ������
		return true; // ������������� ��������� ��������������� ������� ������, �������, ��� ���������� ����� �� �����
	}
	else if (*from > 0) { // �������� ������ �� ����� ����������� � �� �����: ���������� ����� ���� ����� ������
		return true;
	}

	return false; // ������� false ��������������� � ���, ��� ����� ���������� ����� ������ ��� �����������
}

void game::alignToBorder(unsigned short *from, unsigned short *to, bool *isSmthChanged) {
	if (*to == 0) {
		*isSmthChanged = true; // ��������� ���� ��������� �� ������� ����
		*to = *from; // ���������� ��������
		*from = 0; // ������� �������� ������
	}
}

void game::render() {

	COORD cursorCoord;

	int i, y; // ���������

	for (i = 0; i < FIELD_SIZE; i++) {
		for (y = 0; y < FIELD_SIZE; y++) {
			// ������������� ������ � ������ ������, �������� ���� � ������� �����
			cursorCoord.X = 2 + y * 6;
			cursorCoord.Y = 2 + i * 4;
			SetConsoleCursorPosition(consoleHandle, cursorCoord);
			switch (field[i][y]) 
			{
			case 2:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_Green);
				puts(" 2  ");
				break;
			case 4:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
				puts(" 4  ");
				break;
			case 8:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkGreen);
				puts(" 8  ");
				break;
			case 16:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkCyan);
				puts(" 16 ");
				break;
			case 32:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkRed);
				puts(" 32 ");
				break;
			case 64:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkMagenta);
				puts(" 64 ");
				break;
			case 128:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkYellow);
				puts("128 ");
				break;
			case 256:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_Blue);
				puts("256 ");
				break;
			case 512:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_Cyan);
				puts("512 ");
				break;
			case 1024:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_Red);
				puts("1024");
				break;
			case 2048:
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_Magenta);
				puts("2048");
				break;
			default: // ����� ������������, ����� ������ �����, ������� ������ ��������
				puts("    ");
				break;
			}
		}
	}

	// ������� ����:
	cursorCoord.X = 7;
	cursorCoord.Y = 19;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
	printf("%d", score);

	if (isFail) {
		// ���� ��������� ��������� � �������� �� ����
		cursorCoord.X = 7;
		cursorCoord.Y = 20;
		SetConsoleCursorPosition(consoleHandle, cursorCoord);
		puts("The end, you lose!");
	} else if (isWin) {
		// ���� ���������� ������ � �������� �� ����
		cursorCoord.X = 7;
		cursorCoord.Y = 20;
		SetConsoleCursorPosition(consoleHandle, cursorCoord);
		puts("You won!");
	}
}

/*
* ������� ��� ���������� �������� �� ����
*/
void game::spawn() {

	COORD emptyCells[FIELD_SIZE * FIELD_SIZE] = { 0 }; // ���������� ��������� ����� �� ����� ��������� ���������� ���� �����
	short i, y, position, counter = 0;

	for (i = 0; i < FIELD_SIZE; i++) {
		for (y = 0; y < FIELD_SIZE; y++) {
			if (field[i][y] == 0) {
				// ���������� ���������� ������ ������
				emptyCells[counter].X = i;
				emptyCells[counter++].Y = y; // � ����������� ������� ������ �����
			}
		}
	}

	// �������� ���������� ��������� ������ �� ����� ���������...
	position = rand() % counter;

	// ... � �������� � �� ����� 2 ��� 4 � ������������ 1 � 9
	field[emptyCells[position].X][emptyCells[position].Y] = (rand() % 9 > 8 ? 4 : 2);

	if (counter == 1) {
		// ���� 1 ��������� ������, ����� ��������� � �� �����, 
		// ��������, �� �������� ��������� �����, �� ���� ���������� 
		// ������� ���������, ��������� ���:
		game::checkForFail();
	}

}

/*
* ������� ��� �������� ���������� ������� ������
*/
void game::checkForWin(unsigned short number) {

	if (number == 2048) { // �������� ������ �������� ���������� �������� 2048
		isWin = true; // ��������� ����, ����������� �� ���������� ������
		isGameActive = false; // ������������� ����
	}

}

/*
* ������� ��� �������� ��������� �����
*/
void game::checkForFail() {

	unsigned short i, y; // ���������

	// ��������� ������� �������� �����, ��������� �������� ���� �������� �����
	for (i = 0; i < FIELD_SIZE; i++) { 
		for (y = 0; y < FIELD_SIZE; y++) {
			if (i < FIELD_SIZE - 1 && field[i][y] == field[i + 1][y]
				|| y < FIELD_SIZE - 1 && field[i][y] == field[i][y + 1]) {
				return; // ���������� ������ ���������� ����, ��������� ���������� �����
			}
		}
	}

	isFail = true; // ����� �� �������, ������� ��������� ���������
	isGameActive = false; // ���� ����� �� �������, ������������� ����
}