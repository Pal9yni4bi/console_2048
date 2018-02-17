#include "stdafx.h"
#include "time.h"  // для настоящей случайности srand
#include "consoleColor.h"

game::game()
{
	system("cls"); // очистим консоль на случай, если ранее был вывод
	srand(time(0)); // обеспечиваем случайный результат функции rand
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // получаем управление выводом в консоль
	// убираем моргание каретки ввода
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(consoleHandle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &structCursorInfo);

	isGameActive = true; // устанавливаем флаг игры в активное положение

	// выводим каркас: сетку и текст
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

	// размезщаем 2 стартовых элемента в пустые ячейки
	game::spawn();
	game::spawn();

	// отрисовываем произошедшие изменения
	game::render();
}

void game::shiftTo(unsigned char direction) {
	int i, y, inner; // итераторы
	bool isSmthChanged = false; // запоминаем, изменилась ли ситуация на поле в результате хода

	switch (direction)
	{
	case Arrow_Up:
		// i - номер строки, y - номер столбца матрицы
		for (i = 0; i < FIELD_SIZE; i++) {
			// чтобы избежать множественного последовательного коллапсирования ячеек, 
			// разделим коллапсирование и перемещение на 2 независимых цикла
			for (y = 0; y < FIELD_SIZE; y++) {
				// коллапсируем ячейки на линии с положительной ячейкой, если это допустимо
				if (field[i][y] > 0) {
					inner = i;
					while (inner < FIELD_SIZE - 1) {
						inner++;
						if (game::checkAndCollapse(&field[inner][y], &field[i][y], &isSmthChanged)) {
							break; // если проверяемая ячейка сколлапсировала, переходим к следующей
						}
					}
				}
			}
			// если ячейка не пустая - сдвигаем её до упора вверх
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
			// чтобы избежать множественного последовательного коллапсирования ячеек, 
			// разделим коллапсирование и перемещение на 2 независимых цикла
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				// коллапсируем ячейки на линии с положительной ячейкой, если это допустимо
				if (field[i][y] > 0) {
					inner = y;
					while (inner > 0) {
						inner--;
						if (game::checkAndCollapse(&field[i][inner], &field[i][y], &isSmthChanged)) {
							break; // если проверяемая ячейка сколлапсировала, переходим к следующей
						}
					}
				}
			}
			// если ячейка не пустая - сдвигаем её до упора вправо
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
		// i - номер строки, y - номер столбца матрицы
		for (i = FIELD_SIZE - 1; i >= 0; i--) {
			// чтобы избежать множественного последовательного коллапсирования ячеек, 
			// разделим коллапсирование и перемещение на 2 независимых цикла
			for (y = FIELD_SIZE - 1; y >= 0; y--) {
				// коллапсируем ячейки на линии с положительной ячейкой, если это допустимо
				if (field[i][y] > 0) {
					inner = i;
					while (inner > 0) {
						inner--;
						if (game::checkAndCollapse(&field[inner][y], &field[i][y], &isSmthChanged)) {
							break; // если проверяемая ячейка сколлапсировала, переходим к следующей
						}
					}
				}
			}
			// если ячейка не пустая - сдвигаем её до упора вниз
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
		// i - номер строки, y - номер столбца матрицы
		for (i = 0; i < FIELD_SIZE; i++) {
			// чтобы избежать множественного последовательного коллапсирования ячеек, 
			// разделим коллапсирование и перемещение на 2 независимых цикла
			for (y = 0; y < FIELD_SIZE; y++) {
				// коллапсируем ячейки на линии с положительной ячейкой, если это допустимо
				if (field[i][y] > 0) {
					inner = y;
					while (inner < FIELD_SIZE - 1) {
						inner++;
						if (game::checkAndCollapse(&field[i][inner], &field[i][y], &isSmthChanged)) {
							break; // если проверяемая ячейка сколлапсировала, переходим к следующей
						}
					}
				}
			}
			// если ячейка не пустая - сдвигаем её до упора влево
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
		// новые значения на карте появляются, только если были хоть какие-то изменения на игровом поле
		game::spawn();
		// отрисовка имеет смысл только если были хоть какие-то изменения
		game::render();
	}

}

bool game::checkAndCollapse(unsigned short *from, unsigned short *to, bool *isSmthChanged) {
	if (*from == *to) {
		*isSmthChanged = true; // фиксируем факт изменений на игровом поле
		*to *= 2; // фактически суммируем значения соседних ячеек в целевую
		*from = 0; // убираем значение из ячейки-донора
		score += *to; // увеличиваем игровой счёт
		game::checkForWin(*to); // проверяем, достигнуто ли условие победы
		return true; // предотвращаем повторное коллапсирование текущей ячейки, сообщая, что дальнейший поиск не нужен
	}
	else if (*from > 0) { // соседняя ячейка не равна проверяемой и не пуста: дальнейший поиск пары лишён смысла
		return true;
	}

	return false; // возврат false свидетельствует о том, что можно продолжать поиск ячейки для объединения
}

void game::alignToBorder(unsigned short *from, unsigned short *to, bool *isSmthChanged) {
	if (*to == 0) {
		*isSmthChanged = true; // фиксируем факт изменений на игровом поле
		*to = *from; // перемещаем значение
		*from = 0; // очищаем исходную ячейку
	}
}

void game::render() {

	COORD cursorCoord;

	int i, y; // итераторы

	for (i = 0; i < FIELD_SIZE; i++) {
		for (y = 0; y < FIELD_SIZE; y++) {
			// устанавливаем курсор в нужную ячейку, выбираем цвет и выводим число
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
			default: // будет отрабатывать, когда ячейка пуста, затирая старое значение
				puts("    ");
				break;
			}
		}
	}

	// выводим счёт:
	cursorCoord.X = 7;
	cursorCoord.Y = 19;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
	printf("%d", score);

	if (isFail) {
		// если наступило поражение — сообщаем об этом
		cursorCoord.X = 7;
		cursorCoord.Y = 20;
		SetConsoleCursorPosition(consoleHandle, cursorCoord);
		puts("The end, you lose!");
	} else if (isWin) {
		// если достигнута победа — сообщаем об этом
		cursorCoord.X = 7;
		cursorCoord.Y = 20;
		SetConsoleCursorPosition(consoleHandle, cursorCoord);
		puts("You won!");
	}
}

/*
* Функция для добавления элемента на поле
*/
void game::spawn() {

	COORD emptyCells[FIELD_SIZE * FIELD_SIZE] = { 0 }; // количество свободных ячеек не может превышать количества всех ячеек
	short i, y, position, counter = 0;

	for (i = 0; i < FIELD_SIZE; i++) {
		for (y = 0; y < FIELD_SIZE; y++) {
			if (field[i][y] == 0) {
				// запоминаем координаты пустой ячейки
				emptyCells[counter].X = i;
				emptyCells[counter++].Y = y; // и увеличиваем счётчик пустых ячеек
			}
		}
	}

	// выбираем координаты случайной ячейки из числа свободных...
	position = rand() % counter;

	// ... и помещаем в неё число 2 или 4 с вероятностью 1 к 9
	field[emptyCells[position].X][emptyCells[position].Y] = (rand() % 9 > 8 ? 4 : 2);

	if (counter == 1) {
		// была 1 свободная ячейка, после генерации в неё числа, 
		// возможно, не осталось свободных ходов, то есть достигнуто 
		// условие проигрыша, проверяем это:
		game::checkForFail();
	}

}

/*
* Функция для проверки достижения условий победы
*/
void game::checkForWin(unsigned short number) {

	if (number == 2048) { // условием победы является достижение значения 2048
		isWin = true; // поднимаем флаг, указывающий на достижение победы
		isGameActive = false; // останавливаем игру
	}

}

/*
* Функция для проверки доступных ходов
*/
void game::checkForFail() {

	unsigned short i, y; // итераторы

	// проверяем наличие свобоных ходов, сравнивая значения всех соседних ячеек
	for (i = 0; i < FIELD_SIZE; i++) { 
		for (y = 0; y < FIELD_SIZE; y++) {
			if (i < FIELD_SIZE - 1 && field[i][y] == field[i + 1][y]
				|| y < FIELD_SIZE - 1 && field[i][y] == field[i][y + 1]) {
				return; // достаточно одного возможного хода, прерываем дальнейший поиск
			}
		}
	}

	isFail = true; // ходов не найдено, условия поражения соблюдены
	isGameActive = false; // если ходов не найдено, останавливаем игру
}