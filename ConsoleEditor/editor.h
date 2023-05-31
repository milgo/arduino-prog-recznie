#ifndef __EDITOR_H
#define __EDITOR_H

#include <string.h>
#include "gui.h"

#define strcpy_P strcpy
#define pgm_read_byte(x) (*x)
#define pgm_read_word(x) (*x)
#define delay(x) ;
#define bool unsigned char
#define true 1
#define false 0


int showMenu(int x, int y, const char * const *menu, const char *const *descMenu, int from, int to, uint8_t *selPos);
int32_t enterValue(int x, int y, int msg, long int curVal, bool isSigned, int len, int maxDigit);

void insertProgramLine(int lpos, int number, int row, bool edit);
void removeProgramLine(int number);
void editProgram();

#endif
