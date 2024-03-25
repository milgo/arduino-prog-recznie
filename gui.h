#ifndef _GUI_H
#define _GUI_H

#include <stdint.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define MAIN_MENU_SIZE 5 
#define COMM_MENU_SIZE 8
#define MAX_STRING_SIZE 10
#define FONT_HEIGHT 8

#define BUTTON_DOWN 4
#define BUTTON_LEFT 1
#define BUTTON_RIGHT 3
#define BUTTON_UP 0
#define BUTTON_ENTER 2
#define IS_PRESSED(BUTTONS, BUTTON) (((BUTTONS) & (1<<(BUTTON))) == (1<<(BUTTON)))

void setupGUI();
void printA(const char *const* arr, int id);
void printAtoBuf(const char *const* arr, int id, char* buf);

uint8_t printMessageAndWaitForButton(int msg);
uint8_t printMessageAndWaitForButton(int msg, int v1, int v2);
void displaySetTextNormal();
void displaySetTextInvert();
void displayClear();
void displayDisplay();
void displaySetCursor(uint8_t x, uint8_t y);
void displayPrint(const char* msg);
void displayPrint(long i);
void displayPrintln(const char* msg);
void displayPrintln();

uint8_t getButtonsBlocking();
uint8_t getButtonsNoneBlocking();

#endif //_GUI_H
