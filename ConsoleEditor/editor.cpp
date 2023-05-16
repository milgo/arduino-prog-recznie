#include "editor.h"

/**
 * 1. Ignore PROGMEM from messages.h (try overriding #define PROGMEM with some other attribute)
 * 2. Implement new pgm_read_word and pgm_read_byte (try "#define pgm_read_word(x) x" to ignore)
 * 3. Add level paremater to showMenu to draw child menu to the right 
 */

int menuPosition = 0;
int selectedPosition = 0;
char editorBufStr[16];

void enterCurrentOption(int newMenuPosition){
  menuPosition = (newMenuPosition + 1) * 4;
  selectedPosition = menuPosition;
  //Serial.println(menuPosition);
}

void exitCurrentMenu(int currentMenuPos){
  menuPosition = (currentMenuPos) / 4 - 1;// / 4 - modulo;
  int modulo = menuPosition % 4;
  menuPosition -= modulo;
  if(menuPosition<0)menuPosition=0;
  if(modulo<0)modulo=0;
  selectedPosition = menuPosition + modulo;
}

int showMenu(const char * const *menu, const char *const *descMenu, int from, int to){
  int pos = 0, start = 0; int len = to - from;
  unsigned char newButtons = 0;
  while(true){
    displayClear();
    displaySetCursor(0, 0);

    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return -1;
    if(IS_PRESSED(newButtons, BUTTON_ENTER)) return from+pos;
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<len-1 && IS_PRESSED(newButtons, BUTTON_DOWN)) pos++;

    if(pos<start && start>0)start--;
    else if(pos>start+3 && start<len-4)start++;

    for(int i=start; i<start+4; i++){

      if(from+i>=to)break;
      
      if(pos == i){
        displaySetTextInvert();
      }
  
      displaySetCursor(0, (i-start)*8);

      strcpy_P(editorBufStr, (char*)pgm_read_word(&(menu[from+i])));
      displayPrint(editorBufStr);
      

      if(descMenu){
        printA(message, COLON); //display.print(F(":"));
        strcpy_P(editorBufStr, (char*)pgm_read_word(&(descMenu[from+i])));
        displayPrintln(editorBufStr);
      }else{
        displayPrintln();
      }
        
      if(pos == i){
        displaySetTextNormal();
      }
    }

    displayDisplay();
    newButtons = getButtonsBlocking();

    //Serial.println(newButtons, BIN);
    
    delay(100);
  }
}

int32_t enterValue(int msg, long int curVal, bool isSigned, int len, int maxDigit){
  unsigned char newButtons = 0;
  int32_t retVal = 0;
  unsigned char v[10];

  //get sign
  v[0]=0;
  if(curVal<0L){
    if(isSigned)v[0]=1;
    curVal*=-1;
  }

  //convert to array
  long int tmp = curVal;
  for(int j=len;j>0;j--){
    v[j]=tmp%10;
    tmp/=10;
  }

  //edit
  int pos = len;
  int maxVal = 1;
  while(true){
    displayClear();
    displaySetCursor(0, 0);

    strcpy_P(editorBufStr, (char*)pgm_read_word(&(message[msg])));
    displayPrint(editorBufStr);
    
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_LEFT)) pos--;
    if(pos<len && IS_PRESSED(newButtons, BUTTON_RIGHT)) pos++;

    if(pos>0){maxVal = maxDigit;}
    else {
      if(isSigned)maxVal = 1;
      else maxVal = 0;
    }

    if(IS_PRESSED(newButtons, BUTTON_UP)){ if(v[pos]<maxVal)v[pos]++; }
    if(IS_PRESSED(newButtons, BUTTON_DOWN)){ if(v[pos]>0)v[pos]--; }
    if(IS_PRESSED(newButtons, BUTTON_ENTER)){ break; }
    
    for(int i=0; i<len+1; i++){
      
      if(pos==i){
        displaySetCursor(i*6, 16);
        displayPrint("^ ");
        //display.print("]");
      }

      displaySetCursor(i*6, 8);
      if(i==0){
        if(v[0]==0)displayPrint("+");
        else displayPrint("-");
      }else{
        strcpy_P(editorBufStr, (char*)pgm_read_word(&(numbers[v[i]])));
        displayPrint(editorBufStr);
      }
    }

    displayDisplay();
    newButtons = getButtonsBlocking();
    delay(100);
  }

  displayClear();
  displaySetCursor(0, 0);

  //convert from array
  for(int j=1;j<len+1;j++){
    retVal *= 10;
    retVal += v[j];
  }

  //set sign
  if(v[0]>0)retVal*=-1;

  return retVal;
}

void insertProgramLine(int number, bool edit){
  int32_t command = 0, mem = -1;
  uint8_t var_pos = 0, bit_pos = 0;
  int8_t comGroup = showMenu(commandGroupMenu, NULL, 0, COMM_MENU_SIZE);
  int32_t value = 0;
  if(comGroup>=0){
    command = showMenu(comStr, comDesc, comGroups[comGroup*2], comGroups[comGroup*2+1]);

    uint8_t memPtrFrom, memPtrTo;
    memPtrFrom = pgm_read_byte(&(memGroups[command*2]));
    memPtrTo = pgm_read_byte(&(memGroups[command*2+1]));
    
    if(command>=0 && memPtrFrom>=0){

      if(memPtrFrom>0){ //if operation with mem selection
        mem = showMenu(memStr, memDesc, memPtrFrom, memPtrTo);
        if(mem >= 0){
          
          /*casting int16_t to char ???*/
          char sig = pgm_read_word(&memValidationRules[mem*5]); 
          char len = pgm_read_word(&memValidationRules[mem*5+1]);
          char dig = pgm_read_word(&memValidationRules[mem*5+2]);
          int16_t minimum = pgm_read_word(&memValidationRules[mem*5+3]);
          int16_t maximum = pgm_read_word(&memValidationRules[mem*5+4]);

          uint8_t enterMsg = pgm_read_word(&memValueAquireMsg[mem]);
          value = enterValue(enterMsg, 0, sig, len, dig);
          
          if(value < minimum || value > maximum){
            printMessageAndWaitForButton(MUST_BE_IN_RANGE, minimum, maximum);
            return;
          }
          
          if(mem != CS && mem != AD)//const
            var_pos = value;

          uint8_t enterBit = pgm_read_word(&memBitAquireEnabled[mem]);
          if(enterBit == 1){
            bit_pos = enterValue(ENTER_BIT_POS_MSG, 0, 0, 1, 7);
          }
  
        }
      }else{
        mem = 0;
      }

      if(mem >= 0){
        if(PS>=MAX_PROGRAM_SIZE){
          printMessageAndWaitForButton(LIMIT_MSG);
        }else {
          if(!edit){
            if(number < PS){        
              for(int i=PS;i>number;i--){
                program[i] = program[i-1];
              }
            }
            if(PS<MAX_PROGRAM_SIZE)PS++;
          }

          if(mem == CS || mem == AD){ //constant or address
            program[number] = s_stll_v(command, mem, value);
            programChanged = 0;
          }
          else{
            program[number] = s_stll_m(command, mem, var_pos, bit_pos);
            programChanged = 0;
          }
        }
      }
    }
  } 
}

void removeProgramLine(int number){
  for(int i=number;i<PS;i++){
    program[i] = program[i+1];
  }
  PS--;
  programChanged = 0;
}

void editProgram(){
  int pl = 0; int j=0;
  int pos = 0; 
  
  unsigned char newButtons = 0;
  displaySetTextNormal();
  
  while(true){
    displayClear();

    if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos < PS) {
      int res = showMenu(editMenu, NULL, 0, 3);
      switch(res){
        case 0: insertProgramLine(pos, false);break;
        case 1: insertProgramLine(pos, true);break;
        case 2: removeProgramLine(pos);if(pos>0)pos--;if(pl>0)pl--;break;
        default: break;
      }
    }else if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos == PS && PS<MAX_PROGRAM_SIZE) {
      insertProgramLine(pos, false);
    }
    
    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return;
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<PS && IS_PRESSED(newButtons, BUTTON_DOWN))pos++;

    if(pos<pl && pl>0)pl--;
    else if(pos>pl+3 && pl<PS-3)pl++;

    for(int i=pl; i<pl+4 && i<=PS; i++){
      displaySetCursor(0, (i-pl)*8);
      uint8_t func_id = program[i] >> FUNC_BIT_POS;
      uint16_t param = program[i] & FUNC_PARAM_MASK;
      uint8_t mem_pos = (program[i] >> MEM_BIT_POS) & 0xFF;
      uint8_t var_pos = param >> 4 & 0xFF;
      uint8_t bit_pos = param & 0xF;
      int value = param & FUNC_PARAM_MASK;

      if(pos == i){
          displaySetTextInvert();
        }

      if(i<PS){
        displayPrint(i+1);displayPrint(": ");
        printA(comStr, func_id);
        displayPrint(" ");
        if(mem_pos>0){

          printA(memStr, mem_pos);
          if(mem_pos == CS || mem_pos == AD)//constant
            displayPrint((long int)value);
          else
            displayPrint((long)var_pos);
            
          uint8_t enterBit = pgm_read_word(&memBitAquireEnabled[mem_pos]);
          if(enterBit == 1){
            displayPrint(".");
            displayPrint((long)bit_pos);
          }
        }
      }else {
        if(PS<MAX_PROGRAM_SIZE){
          printA(message, ADD_LINE_MSG);
        }
        else{
          printA(message, LIMIT_MSG);
        }
      }

      if(pos == i){
          displaySetTextNormal();
        }
    }

    displayDisplay();
    newButtons = getButtonsBlocking();
    delay(100);
  }
}
