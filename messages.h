#ifndef _MESSAGES_H
#define _MESSAGES_H

#include <avr/pgmspace.h>
#include "stl.h"

#define ENTER_VARIABLE_POS_MSG 0
#define ENTER_VALUE_MSG 1
#define LIMIT_MSG 2
#define ADD_LINE_MSG 3
#define RUNNING_MSG 4
#define NOPROGRAM_MSG 5
#define ENTER_BIT_POS_MSG 6
#define MUST_BE_IN_RANGE 7
#define PROGRAMMING_EEPROM 8
#define READING_EEPROM 9
#define RESERVED_0 10
#define NO_CHANGES 11
#define COLON 12
#define ENTER_INPUT_NR_MSG 13
#define ENTER_OUTPUT_NR_MSG 14
#define ENTER_BYTE_NR_MSG 15
#define ENTER_WORD_NR_MSG 16
#define ENTER_DWORD_NR_MSG 17
#define ENTER_TIMER_NR_MSG 18
#define ENTER_COUNTER_NR_MSG 19
#define ENTER_PROGRAM_LINE_NR_MSG 20
#define CLEAR_LOCAL_PROGRAM_MSG 21

const char _0[] PROGMEM = "0"; 
const char _1[] PROGMEM = "1"; 
const char _2[] PROGMEM = "2"; 
const char _3[] PROGMEM = "3"; 
const char _4[] PROGMEM = "4"; 
const char _5[] PROGMEM = "5"; 
const char _6[] PROGMEM = "6"; 
const char _7[] PROGMEM = "7"; 
const char _8[] PROGMEM = "8"; 
const char _9[] PROGMEM = "9"; 

const char *const numbers[] PROGMEM = {_0, _1, _2, _3, _4, _5, _6, _7, _8, _9};

const char enterVariablePositionMsgStr[] PROGMEM = "Enter var position:";
const char enterBitPositionMsgStr[] PROGMEM = "Enter bit position:";
const char enterValueMsgStr[] PROGMEM = "Enter value:";
const char limitMsgStr[] PROGMEM = "LIMIT";
const char addLineMsgStr[] PROGMEM = "[+]";
const char runningMsgStr[] PROGMEM = "Running";
const char noProgramMsgStr[] PROGMEM = "No program";
const char mustBeInRange[] PROGMEM = "Must be in range: ";
const char programmingEepromMsgStr[] PROGMEM = "Programming eeprom...";
const char clearMsgStr[] PROGMEM = "Erasing local program...";
const char readingEepromMsgStr[] PROGMEM = "Reading from eeprom...";
const char reserved0MsgStr[] PROGMEM = "";
const char noChangesMsgStr[] PROGMEM = "No changes!";
const char colon[] PROGMEM = ": ";

const char enterInputNrMsgStr[] PROGMEM = "Enter input nr:";
const char enterOutputNrMsgStr[] PROGMEM = "Enter output nr:";
const char enterByteNrMsgStr[] PROGMEM = "Enter byte nr:";
const char enterWordNrMsgStr[] PROGMEM = "Enter word nr:";
const char enterDWordNrMsgStr[] PROGMEM = "Enter dword nr:";
const char enterTimerNrMsgStr[] PROGMEM = "Enter timer nr:";
const char enterCounterNrMsgStr[] PROGMEM = "Enter counter nr:";
const char enterProgramLineMsgStr[] PROGMEM = "Enter program line:";

const char *const message[] PROGMEM = {
  enterVariablePositionMsgStr, 
  enterValueMsgStr,
  limitMsgStr,
  addLineMsgStr,
  runningMsgStr,
  noProgramMsgStr,
  enterBitPositionMsgStr,
  mustBeInRange,
  programmingEepromMsgStr,
  readingEepromMsgStr,
  reserved0MsgStr,
  noChangesMsgStr,
  colon,
  enterInputNrMsgStr,
  enterOutputNrMsgStr,
  enterByteNrMsgStr,
  enterWordNrMsgStr,
  enterDWordNrMsgStr,
  enterTimerNrMsgStr,
  enterCounterNrMsgStr,
  enterProgramLineMsgStr,
  clearMsgStr
  };

const char runMenuSaveAndRunStr[] PROGMEM = "Save&Run";
const char runMenuDiscardStr[] PROGMEM = "Discard";
const char runMenuCanceStr[] PROGMEM = "Cancel";

const char mainMenuRunStr[] PROGMEM = "Run";
const char mainMenuEditStr[] PROGMEM = "Edit";
const char mainMenuProgramStr[] PROGMEM = "Program";
const char mainMenuClearStr[] PROGMEM = "Clear";

const char comMenuBasicStr[] PROGMEM = "Logic";
const char comMenuMoveStr[] PROGMEM = "Move";
const char comMenuAritmeticStr[] PROGMEM = "Aritmetic";
const char comMenuCompareStr[] PROGMEM = "Compare";
const char comMenuTimersStr[] PROGMEM = "Timers";
const char comMenuCounterStr[] PROGMEM = "Counters";
const char comMenuJumpStr[] PROGMEM = "Jump";

const char editMenuInsertStr[] PROGMEM = "Insert";
const char editMenuEditStr[] PROGMEM = "Edit";
const char editMenuRemoveStr[] PROGMEM = "Remove";

const char comNOPStr[] PROGMEM = "NOP";
const char comBasicAStr[] PROGMEM = "A  ";
const char comBasicOStr[] PROGMEM = "O  ";
const char comBasicANStr[] PROGMEM = "AN ";
const char comBasicONStr[] PROGMEM = "ON ";
const char comBasicAssignStr[] PROGMEM = "=  ";
const char comBasicSStr[] PROGMEM = "S  ";
const char comBasicRStr[] PROGMEM = "R  ";
const char comBasicFPStr[] PROGMEM = "FP ";
const char comBasicFNStr[] PROGMEM = "FN ";
const char comMoveLStr[] PROGMEM = "L  ";
const char comMoveTStr[] PROGMEM = "T  ";
const char comAritAddIStr[] PROGMEM = "+I ";
const char comAritSubIStr[] PROGMEM = "-I ";
const char comAritMulIStr[] PROGMEM = "*I ";
const char comAritDivIStr[] PROGMEM = "/I ";
const char comCompEqIStr[] PROGMEM = "==I";
const char comCompDiffIStr[] PROGMEM = "<>I";
const char comCompGtIStr[] PROGMEM = ">I ";
const char comCompLtIStr[] PROGMEM = "<I ";
const char comCompGtEqIStr[] PROGMEM = ">=I";
const char comCompLtEqIStr[] PROGMEM = "<=I";
const char comTimerSPStr[] PROGMEM = "SP ";
const char comTimerSEStr[] PROGMEM = "SE ";
const char comTimerSDStr[] PROGMEM = "SD ";
const char comTimerSSStr[] PROGMEM = "SS ";
const char comTimerSFStr[] PROGMEM = "SF ";
const char comTimerRStr[] PROGMEM = "R  ";
const char comCounterCUStr[] PROGMEM = "CU ";
const char comCounterCDStr[] PROGMEM = "CD ";
const char comCounterSStr[] PROGMEM = "S  ";
const char comCounterRStr[] PROGMEM = "R  ";
const char comCounterLStr[] PROGMEM = "L  ";
const char comCounterLCStr[] PROGMEM = "LC ";
const char comJumpUncStr[] PROGMEM = "JU ";
const char comJumpCondStr[] PROGMEM = "JC ";
const char comJumpCondNotStr[] PROGMEM = "JCN";

const char comDescNOPStr[] PROGMEM = "No operation";
const char comDescBasicAStr[] PROGMEM = "And";
const char comDescBasicOStr[] PROGMEM = "Or";
const char comDescBasicANStr[] PROGMEM = "Not And";
const char comDescBasicONStr[] PROGMEM = "Not Or";
const char comDescBasicAssignStr[] PROGMEM = "Assign";
const char comDescBasicSStr[] PROGMEM = "Set";
const char comDescBasicRStr[] PROGMEM = "Reset";
const char comDescBasicFPStr[] PROGMEM = "Positive edge";
const char comDescBasicFNStr[] PROGMEM = "Negative edge";
const char comDescMoveLStr[] PROGMEM = "Load";
const char comDescMoveTStr[] PROGMEM = "Transfer";
const char comDescAritAddIStr[] PROGMEM = "Add";
const char comDescAritSubIStr[] PROGMEM = "Substract";
const char comDescAritMulIStr[] PROGMEM = "Multiply";
const char comDescAritDivIStr[] PROGMEM = "Divide";
const char comDescCompEqIStr[] PROGMEM = "Equals";
const char comDescCompDiffIStr[] PROGMEM = "Diffrent";
const char comDescCompGtIStr[] PROGMEM = "Greater";
const char comDescCompLtIStr[] PROGMEM = "Less";
const char comDescCompGtEqIStr[] PROGMEM = "Greater or equal";
const char comDescCompLtEqIStr[] PROGMEM = "Less or equal";
const char comDescTimerSPStr[] PROGMEM = "Pulse timer";
const char comDescTimerSEStr[] PROGMEM = "Ext. pulse timer";
const char comDescTimerSDStr[] PROGMEM = "On-Delay timer";
const char comDescTimerSSStr[] PROGMEM = "Ret. On-Delay tim.";
const char comDescTimerSFStr[] PROGMEM = "Off-Delay timer";
const char comDescTimerRStr[] PROGMEM = "Reset timer";
const char comDescCounterCUStr[] PROGMEM = "Increment counter";
const char comDescCounterCDStr[] PROGMEM = "Decrement counter";
const char comDescCounterStr[] PROGMEM = "Set counter";
const char comDescCounterRStr[] PROGMEM = "Reset counter";
const char comDescCounterLStr[] PROGMEM = "Load counter";

const char comDescJumpUncStr[] PROGMEM = "Jump uncond.";
const char comDescJumpCondStr[] PROGMEM = "Jump cond.";
const char comDescJumpCondNotStr[] PROGMEM = "Jump cond. if not";

const char *const comDesc[] PROGMEM = {
    comDescNOPStr,
    comDescBasicAStr,
    comDescBasicOStr,
    comDescBasicANStr,
    comDescBasicONStr,
    comDescBasicAssignStr,
    comDescBasicSStr,
    comDescBasicRStr,
    comDescBasicFPStr,
    comDescBasicFNStr,
    comDescMoveLStr,
    comDescMoveTStr,
    comDescTimerSPStr,
    comDescTimerSEStr,
    comDescTimerSDStr,
    comDescTimerSSStr,
    comDescTimerSFStr,
    comDescTimerRStr,
    comDescCounterCUStr,
    comDescCounterCDStr,
    comDescCounterStr,
    comDescCounterRStr,
    comDescCounterLStr,
    comDescAritAddIStr,
    comDescAritSubIStr,
    comDescAritMulIStr,
    comDescAritDivIStr,
    comDescCompEqIStr,
    comDescCompDiffIStr,
    comDescCompGtIStr,
    comDescCompLtIStr,
    comDescCompGtEqIStr,
    comDescCompLtEqIStr,
    comDescJumpUncStr,
    comDescJumpCondStr,
    comDescJumpCondNotStr
  };

const char memNULLStr[] PROGMEM = "~";
const char memDOStr[] PROGMEM = "D";
const char memDIStr[] PROGMEM = "D";
const char memMStr[] PROGMEM = "M";
const char memTStr[] PROGMEM = "T";
const char memMBStr[] PROGMEM = "MB";
const char memMWStr[] PROGMEM = "MW";
const char memMDStr[] PROGMEM = "MD";
const char memCStr[] PROGMEM = "C";
const char memAIStr[] PROGMEM = "AI";
const char memAOStr[] PROGMEM = "AO";
const char memConstStr[] PROGMEM = "#";
const char memAddrStr[] PROGMEM = "@";

const char memDescNULLStr[] PROGMEM = "";
const char memDescDOStr[] PROGMEM = "Digital output";
const char memDescDIStr[] PROGMEM = "Digital input";
const char memDescMStr[] PROGMEM = "Bit marker";
const char memDescTStr[] PROGMEM = "Timer";
const char memDescMBStr[] PROGMEM = "Memory byte";
const char memDescMWStr[] PROGMEM = "Memory word";
const char memDescMDStr[] PROGMEM = "Memory double word";
const char memDescCStr[] PROGMEM = "Counter";
const char memDescAIStr[] PROGMEM = "Analog input";
const char memDescAOStr[] PROGMEM = "Analog output";
const char memDescConstStr[] PROGMEM = "Constant";
const char memDescAddrStr[] PROGMEM = "Program line number";

const char *const memDesc[] PROGMEM = {
    memDescNULLStr,
    memDescDIStr,
    memDescTStr,
    memDescMStr,
    memDescDOStr,
    memDescConstStr,
    memDescAIStr,
    memDescMBStr,
    memDescMWStr,
    memDescMDStr,
    memDescAOStr,
    memDescCStr,
    memDescAddrStr
};

const char *const runMenu[] PROGMEM = {
  runMenuSaveAndRunStr, 
  runMenuDiscardStr, 
  runMenuCanceStr
  };

const char *const mainMenu[] PROGMEM = {
  mainMenuRunStr, 
  mainMenuEditStr, 
  mainMenuProgramStr, 
  mainMenuClearStr
  };

const char *const commandGroupMenu[] PROGMEM = {
  comMenuBasicStr, 
  comMenuMoveStr, 
  comMenuTimersStr, 
  comMenuCounterStr,
  comMenuAritmeticStr, 
  comMenuCompareStr,
  comMenuJumpStr
  };

const char comGroups[] = { 1, 10, 10, 12, 12, 18, 18, 23, 23, 27, 27, 33, 33, 36};  
  
const char *const editMenu[] PROGMEM = {
  editMenuInsertStr,
  editMenuEditStr,
  editMenuRemoveStr
};

const char *const comStr[] PROGMEM = {
  comNOPStr,
  comBasicAStr,
  comBasicOStr,
  comBasicANStr,
  comBasicONStr,
  comBasicAssignStr,
  comBasicSStr,
  comBasicRStr,
  comBasicFPStr,
  comBasicFNStr,
  
  comMoveLStr,
  comMoveTStr,
  
  comTimerSPStr,
  comTimerSEStr,
  comTimerSDStr,
  comTimerSSStr,
  comTimerSFStr,
  comTimerRStr,
  
  comCounterCUStr,
  comCounterCDStr,
  comCounterSStr,
  comCounterRStr,
  comCounterLStr,
  
  comAritAddIStr,
  comAritSubIStr,
  comAritMulIStr,
  comAritDivIStr,

  comCompEqIStr,
  comCompDiffIStr,
  comCompGtIStr,
  comCompLtIStr,
  comCompGtEqIStr,
  comCompLtEqIStr,
  comJumpUncStr,
  comJumpCondStr,
  comJumpCondNotStr

  };



const char *const memStr[] PROGMEM = {
  memNULLStr,
  memDIStr,
  memTStr,
  memMStr,
  memDOStr,
  memConstStr,
  memAIStr,
  memMBStr,
  memMWStr,
  memMDStr,
  memAOStr,
  memCStr,
  memAddrStr
};

const char memGroups[] PROGMEM = {0, 0, 
                          1, 4, 
                          1, 4, 
                          1, 4, 
                          1, 4, 
                          3, 5, 
                          3, 5, 
                          3, 5, 
                          3, 4, 
                          3, 4, 
                          
                          5, 10, 
                          7, 11,
                          
                          2, 3, 
                          2, 3, 
                          2, 3, 
                          2, 3, 
                          2, 3, 
                          2, 3, 
                          
                          11, 12, 
                          11, 12, 
                          11, 12, 
                          11, 12, 
                          11, 12, 
                          
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          
                          0, 0,
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          
                          12, 13,
                          12, 13,
                          12, 13,
                          
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0};                          

const int16_t memValidationRules[] PROGMEM = {0, 0, 0, 0, 0, //signed?, how many digits?, max digit, min value, max value 
                                  0, 1, 7, 0, 7, //DI
                                  0, 1, 7, 0, 7, //T
                                  0, 2, 9, 0, 63,//M
                                  0, 2, 9, 8, 13,//DO
                                  1, 5, 9, -32768, 32767,//CONST
                                  0, 1, 7, 0, 7,//AI
                                  0, 2, 9, 0, 63,//MB
                                  0, 2, 9, 0, 31,//MW
                                  0, 1, 9, 0, 9,//MD        
                                  0, 2, 9, 0, 11,//AO            
                                  0, 1, 9, 0, 7,//C
                                  0, 2, 9, 1, MAX_PROGRAM_SIZE};//ADDR
                               
const char memBitAquireEnabled[] PROGMEM = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}; 
const char memValueAquireMsg[] PROGMEM = {0, 
                                          ENTER_INPUT_NR_MSG, 
                                          ENTER_TIMER_NR_MSG, 
                                          ENTER_BYTE_NR_MSG, 
                                          ENTER_OUTPUT_NR_MSG, 
                                          ENTER_VALUE_MSG, 
                                          ENTER_INPUT_NR_MSG, 
                                          ENTER_BYTE_NR_MSG, 
                                          ENTER_WORD_NR_MSG, 
                                          ENTER_DWORD_NR_MSG, 
                                          ENTER_OUTPUT_NR_MSG, 
                                          ENTER_COUNTER_NR_MSG,
                                          ENTER_PROGRAM_LINE_NR_MSG}; 



#endif // _MESSAGES_H
