#include <stdlib.h>
#include "editor.h"

/**
 * 1. Ignore PROGMEM from messages.h (try overriding #define PROGMEM with some other attribute)
 * 2. Implement new pgm_read_word and pgm_read_byte (try "#define pgm_read_word(x) x" to ignore)
 * 3. Add level paremater to showMenu to draw child menu to the right 
 */

#define MENU_ROWS_VISIBLE 28
#define MENU_ROWS_LENGTH 20

#define BUTTON_DOWN 80
#define BUTTON_LEFT 75
#define BUTTON_RIGHT 77
#define BUTTON_UP 72
#define BUTTON_ENTER 13
#define EXTENDED 0
#define F1 59
#define F2 60
#define F3 61
#define F4 62
#define F5 63
#define IS_PRESSED(BUTTONS, BUTTON) (BUTTONS==BUTTON)

int menuPosition = 0;
int selectedPosition = 0;
char editorBufStr[20];

void fillStr(char *dest, char c, int len){
	//int fill = MENU_ROWS_LENGTH - rowlen;
	if(len>0){
		memset(dest, ' ', len);
		*(editorBufStr + len - 1) = '\0';
	}else{
		*(editorBufStr) = '\0';
	}
}

void editorNewFile(){}
void editorOpenFile(){}
void editorSaveFile(){}
void editorUploadFile(){}
void editorDownloadFile(){}

/*memset(editorBufStr, ' ', MENU_ROWS_LENGTH - rowlen);
		*(editorBufStr + MENU_ROWS_LENGTH - rowlen - 1) = '\0';
		displayPrint(editorBufStr);*/

int showMenu(int x, int y, 
		const char * const *menu, 
		const char *const *descMenu, 
		int from, int to, 
		uint8_t *selPosX,
		uint8_t *selPosY){
  int pos = 0, start = 0; 
  int len = to - from;
  int rowlen = 0, mrowlen = 0;
  unsigned char newButtons = 0;
  char *buf = editorBufStr;
  //displayClear();
  
  //max row size
  while(true){
    //displayClear();
    displaySetCursor(x*MENU_ROWS_LENGTH, y);

    if(IS_PRESSED(newButtons, BUTTON_LEFT)) 
	{
		displayClear();
        return -1;
	}
    if(IS_PRESSED(newButtons, BUTTON_ENTER)) {
    	//displayClear();
    	(*selPosX) = x + mrowlen;
    	(*selPosY) = (*selPosY)+pos;
        return from+pos;	
	}
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<len-1 && IS_PRESSED(newButtons, BUTTON_DOWN)) pos++;

    if(pos<start && start>0)start--;
    else if(pos>start+MENU_ROWS_VISIBLE-1 && start<len-MENU_ROWS_VISIBLE)start++;

    for(int i=start; i<start+MENU_ROWS_VISIBLE; i++){

		if(from+i>=to)break;
      
      	if(pos == i){
        	displaySetTextInvert();
      	}
  
	    displaySetCursor(x/**MENU_ROWS_LENGTH*/, y+(i-start));
	
		char *buf = editorBufStr;
	    strcpy_P(buf, (char*)pgm_read_word(&(menu[from+i])));
	    //displayPrint(editorBufStr);
	    //rowlen = strlen(buf);
		buf += strlen(buf);
		
	    if(descMenu != NULL){
	    	//printA(message, COLON); //display.print(F(":"));
	    	strcpy_P(buf, ":"); buf++;
	    	strcpy_P(buf, (char*)pgm_read_word(&(descMenu[from+i])));
	        //displayPrint(editorBufStr);
	        //rowlen += strlen(editorBufStr);

	    }/*else{
	        displayPrint();
	    }*/
	
		//fillStr(editorBufStr, ' ', MENU_ROWS_LENGTH - rowlen);
	    displayPrint(editorBufStr);
		rowlen = strlen(editorBufStr)+2;
		if(mrowlen < rowlen)
			mrowlen = rowlen;
		
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

int32_t enterValue(int x, int y, int msg, long int curVal, bool isSigned, int len, int maxDigit){
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
    //displayClear();
    displaySetCursor(x/**MENU_ROWS_LENGTH*/, y);

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
        displaySetCursor(x/**MENU_ROWS_LENGTH*/+i, y+2);
        displayPrint("^ ");
        //display.print("]");
      }

      displaySetCursor(x/**MENU_ROWS_LENGTH*/+i, y+1);
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

  //displayClear();
  //displaySetCursor(level*MENU_ROWS_LENGTH, 0);

  //convert from array
  for(int j=1;j<len+1;j++){
    retVal *= 10;
    retVal += v[j];
  }

  //set sign
  if(v[0]>0)retVal*=-1;

  return retVal;
}

void insertProgramLine(int lpos, int numLine, bool edit){
  int32_t command = 0, mem = -1;
  uint8_t var_pos = 0, bit_pos = 0, selPosX = lpos, selPosY = numLine;
  int8_t comGroup = showMenu(selPosX, selPosY, commandGroupMenu, NULL, 0, COMM_MENU_SIZE, &selPosX, &selPosY);
  int32_t value = 0;
  if(comGroup>=0){
    command = showMenu(selPosX, selPosY, comStr, comDesc, comGroups[comGroup*2], comGroups[comGroup*2+1], &selPosX, &selPosY);

    uint8_t memPtrFrom, memPtrTo;
    memPtrFrom = pgm_read_byte(&(memGroups[command*2]));
    memPtrTo = pgm_read_byte(&(memGroups[command*2+1]));
    
    if(command>=0 && memPtrFrom>=0){

      if(memPtrFrom>0){ //if operation with mem selection
        mem = showMenu(selPosX, selPosY, memStr, memDesc, memPtrFrom, memPtrTo, &selPosX, &selPosY);
        if(mem >= 0){
          
          /*casting int16_t to char ???*/
          char sig = pgm_read_word(&memValidationRules[mem*5]); 
          char len = pgm_read_word(&memValidationRules[mem*5+1]);
          char dig = pgm_read_word(&memValidationRules[mem*5+2]);
          int16_t minimum = pgm_read_word(&memValidationRules[mem*5+3]);
          int16_t maximum = pgm_read_word(&memValidationRules[mem*5+4]);

          uint8_t enterMsg = pgm_read_word(&memValueAquireMsg[mem]);
          value = enterValue(selPosX, selPosY, enterMsg, 0, sig, len, dig);
          
          if(value < minimum || value > maximum){
            printMessageAndWaitForButton(MUST_BE_IN_RANGE, minimum, maximum);
            return;
          }
          
          if(mem != CS && mem != AD)//const
            var_pos = value;

          uint8_t enterBit = pgm_read_word(&memBitAquireEnabled[mem]);
          if(enterBit == 1){
            bit_pos = enterValue(selPosX, selPosY, ENTER_BIT_POS_MSG, 0, 0, 1, 7);
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
            if(numLine < PS){        
              for(int i=PS;i>numLine;i--){
                program[i] = program[i-1];
              }
            }
            if(PS<MAX_PROGRAM_SIZE)PS++;
          }

          if(mem == CS || mem == AD){ //constant or address
            program[numLine] = s_stll_v(command, mem, value);
            programChanged = 0;
          }
          else{
            program[numLine] = s_stll_m(command, mem, var_pos, bit_pos);
            programChanged = 0;
          }
        }
      }
      
      displayClear();
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
  int rowlen = 0, mrowlen = 0;
  unsigned char newButtons = -1;
  displaySetTextNormal();

  while(true){

    if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos < PS) {
      uint8_t selPosX = mrowlen, selPosY = pos-pl;
      int res = showMenu(selPosX, selPosY, editMenu, NULL, 0, 3, &selPosX, &selPosY);
      switch(res){
        case 0: insertProgramLine(selPosX, selPosY, false);break;
        case 1: insertProgramLine(selPosX, selPosY, true);break;
        case 2: removeProgramLine(pos);if(pos>0)pos--;if(pl>0)pl--;displayClear();break;
        default: break;
      }
    }else if(IS_PRESSED(newButtons, BUTTON_ENTER) && pos == PS && PS<MAX_PROGRAM_SIZE) {
      insertProgramLine(mrowlen, pos-pl, false);
    }
    
    //display additional menu on bottom of the screen
  	displaySetCursor(0, 29);
	displayPrint("F1 New F2 Open F3 Save F4 Upload F5 Download");
    
    if(pos>0 && IS_PRESSED(newButtons, BUTTON_UP)) pos--;
    if(pos<PS && IS_PRESSED(newButtons, BUTTON_DOWN))pos++;
    
    if(IS_PRESSED(newButtons, BUTTON_LEFT)) return;
    
    if(IS_PRESSED(newButtons, EXTENDED)){
    	switch(getButtonsBlocking()){
    		case F1:editorNewFile();break;
    		case F2:editorOpenFile();break;
    		case F3:editorSaveFile();break;
    		case F4:editorUploadFile();break;
    		case F5:editorDownloadFile();break;
		}
    }

    if(pos<pl && pl>0)pl--;
    else if(pos>pl+MENU_ROWS_VISIBLE-1 && pl<PS-MENU_ROWS_VISIBLE-1)pl++;

    for(int i=pl; i<pl+MENU_ROWS_VISIBLE && i<=PS; i++){
      displaySetCursor(0, (i-pl));
      uint8_t func_id = program[i] >> FUNC_BIT_POS;
      uint16_t param = program[i] & FUNC_PARAM_MASK;
      uint8_t mem_pos = (program[i] >> MEM_BIT_POS) & 0xFF;
      uint8_t var_pos = param >> 4 & 0xFF;
      uint8_t bit_pos = param & 0xF;
      int value = param & FUNC_PARAM_MASK;

      if(pos == i){
          displaySetTextInvert();
        }

      if(i<PS){//display line of code
      	/**/
      	char* buf = editorBufStr;
      	itoa(i+1, buf, 10);
      	buf += strlen(buf);
      	strcpy_P(buf, ": ");
      	buf += 2;
		strcpy_P(buf, (char*)pgm_read_word(&(comStr[func_id])));
		buf += strlen(comStr[func_id]);
		strcpy_P(buf, " "); buf++;
		if(mem_pos>0){
			strcpy_P(buf, (char*)pgm_read_word(&(memStr[mem_pos])));
			buf += strlen(memStr[mem_pos]);
			
          	if(mem_pos == CS || mem_pos == AD){
            	itoa(value, buf, 10);
            }
          	else{
          		itoa(var_pos, buf, 10);
			}
			buf += strlen(buf);//?
            
          	uint8_t enterBit = pgm_read_word(&memBitAquireEnabled[mem_pos]);
          	if(enterBit == 1){
            	strcpy_P(buf, "."); buf++;
            	itoa(bit_pos, buf, 10);
			}
		}
      	rowlen = strlen(editorBufStr) + 2;
      	if(mrowlen < rowlen)
      		mrowlen = rowlen;
      	displayPrint(editorBufStr);
          
        /*TODO*/
        //fillStr(editorBufStr, ' ', MENU_ROWS_LENGTH - rowlen);
	    //displayPrint(editorBufStr);
	    
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
