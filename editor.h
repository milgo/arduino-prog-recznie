#ifndef __EDITOR_H
#define __EDITOR_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "gui.h"

int showMenu(const char * const *menu, const char *const *descMenu, int from, int to);
int32_t enterValue(int msg, long int curVal, bool isSigned, int len, int maxDigit);

void insertProgramLine(int number, bool edit);
void removeProgramLine(int number);
void editProgram();

#endif
