#ifndef _STL_H
#define _STL_H

#include <EEPROM.h>
#include <stdint.h>
#include <avr/io.h>

#define FUNC_BIT_POS 24
#define MEM_BIT_POS 16
#define FUNC_PARAM_MASK 0xFFFFUL
#define MAX_PROGRAM_SIZE 64

#define DI 1UL
#define TIM 2UL
#define M 3UL
#define DO 4UL
#define CS 5UL
#define AI 6UL
#define MB 7UL
#define MW 8UL
#define MD 9UL
#define AO 10UL
#define CN 11UL
#define AD 12UL

#define s_stll_v(a1, a2, a3) ((a1<<FUNC_BIT_POS) | ((a2) << (MEM_BIT_POS)) | (a3 & FUNC_PARAM_MASK))
#define s_stll_m(a1, a2, a3, a4) ((a1<<FUNC_BIT_POS) | ((a2) << (MEM_BIT_POS)) | (a3<<4) | (a4))
#define s_stll_s(a1) (a1<<FUNC_BIT_POS)
#define set_b(m, p, b) *memMap[m][p] |= _BV(b);
#define reset_b(m, p, b) *memMap[m][p] &= ~_BV(b);

extern uint32_t program[MAX_PROGRAM_SIZE];
extern int8_t volatile PC;
extern int8_t volatile PS;
extern uint8_t volatile m[64];
extern uint8_t volatile buttons;
extern uint8_t programChanged;

void _and(uint32_t param);
void _or(uint32_t param);
void _nand(uint32_t param);
void _nor(uint32_t param);
void _not(uint32_t param);
void _assign(uint32_t param);
void _s(uint32_t param);
void _r(uint32_t param);
void _fp(uint32_t param);
void _fn(uint32_t param);
void _l(uint32_t param);
void _t(uint32_t param);

void _sp(uint32_t param);
void _se(uint32_t param);
void _sd(uint32_t param);
void _ss(uint32_t param);
void _sf(uint32_t param);
void _rt(uint32_t param);

void _cu(uint32_t param);
void _cd(uint32_t param);
void _cs(uint32_t param);
void _cr(uint32_t param);
void _cl(uint32_t param);
void _clc(uint32_t param);

void _addI(uint32_t param);
void _subI(uint32_t param);
void _mulI(uint32_t param);
void _divI(uint32_t param);

void _eqI(uint32_t param);
void _diffI(uint32_t param);
void _gtI(uint32_t param);
void _ltI(uint32_t param);
void _gteqI(uint32_t param);
void _lteqI(uint32_t param);

void _ju(uint32_t param);
void _jc(uint32_t param);
void _jcn(uint32_t param);

extern void setupMem();
extern void onLoopEnd();
extern void executeCommandAt(int pl);
extern void executeCommand(uint32_t param);
extern void timersRoutine();
extern void readAnalog();
extern void writeAnalog();
extern void writeProgramToEeprom();
extern void readProgramFromEeprom();
extern void clearProgramLocal();
#endif //_STL_H
