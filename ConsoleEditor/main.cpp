#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include "editor.h"
#include "messages.h"

#define WHITE_ON_BLACK 15
#define BLACK_ON_WHITE 240



/*#define BUTTON_DOWN 80
#define BUTTON_LEFT 75
#define BUTTON_RIGHT 77
#define BUTTON_UP 72
#define BUTTON_ENTER 13
#define IS_PRESSED(BUTTONS, BUTTON) (BUTTONS == BUTTON)*/

//typedef boolean bool;

HANDLE hConsole;

WORD consoleAttributes;


uint32_t program[MAX_PROGRAM_SIZE];
int8_t volatile PC;
int8_t volatile PS;
uint8_t volatile m[64];
uint8_t volatile buttons;
uint8_t programChanged;
	
void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GetConsoleColour(WORD* Attributes)
{
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    *Attributes = Info.wAttributes;
}

void SetConsoleColour(WORD Attributes)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

int main()
{
	PS=0;
	PC=0;
	//printf("hello");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, WHITE_ON_BLACK);
	//printf("hello");
	editProgram();

	return 0;
}
