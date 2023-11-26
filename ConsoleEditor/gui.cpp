#include <conio.h>
#include <stdlib.h>
#include "../gui.h"
#include "windows.h"

#define strcpy_P strcpy
#define pgm_read_byte(x) (*x)
#define pgm_read_word(x) (*x)
#define WHITE_ON_BLACK 15
#define BLACK_ON_GRAY 112
#define BLACK_ON_WHITE 240

extern HANDLE hConsole;
DWORD written;
char guiBufferStr[16];

void setupGUI(){
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}
void printA(const char *const* arr, int id){
	strcpy_P(guiBufferStr, (char*)pgm_read_word(&(arr[id])));
  	displayPrint(guiBufferStr);//display.print(" ");
}
void printAtoBuf(const char *const* arr, int id, char* buf){
}

uint8_t printMessageAndWaitForButton(int msg){
}
uint8_t printMessageAndWaitForButton(int msg, int v1, int v2){
}
void displaySetTextNormal(){SetConsoleTextAttribute(hConsole, WHITE_ON_BLACK);}
void displaySetTextInvert(){SetConsoleTextAttribute(hConsole, BLACK_ON_WHITE);}

void displayClear(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

void displayDisplay(){}

void displaySetCursor(uint8_t x, uint8_t y){
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
  
void displayPrint(const char* msg){ WriteConsole(hConsole, msg, strlen(msg), &written, NULL);}
void displayPrint(long i){itoa(i, guiBufferStr, 10);displayPrint(guiBufferStr);}
void displayPrintln(const char* msg){}
void displayPrintln(){}

uint8_t getButtonsBlocking(){ uint8_t c = getch(); return c; }
uint8_t getButtonsNoneBlocking(){}
