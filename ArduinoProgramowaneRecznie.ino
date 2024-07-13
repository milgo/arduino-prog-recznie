//#include "progmem.h"
#include "messages.h"
#include "stl.h"
#include "gui.h"
#include "editor.h"

#include "FRAM.h"
#include "TimeLib.h"
#include "DS1307RTC.h"

#define SCREEN_SAVER_TIME 60
#define EXIT_RUNNING_TIME 6
#define EXIT_RUNNNING_BUTTONS(BUTTONS) IS_PRESSED(BUTTONS, BUTTON_LEFT) && IS_PRESSED(BUTTONS, BUTTON_RIGHT)
//Remember you first have to set RTC otherwise it will be stopped
int checkRTC(){

  tmElements_t tm;
  /*tm.Hour = 23;
  tm.Minute = 55;
  tm.Second = 00;
  tm.Day = 3;
  tm.Month = 11;
  tm.Year = 23;
  if (RTC.write(tm)) {
      //config = true;
  }*/

  int i = 0, sec1 = 0, sec2 = 0;
  while(i<3 && sec1 == 0){
    if (RTC.read(tm)) {
      sec1 = tm.Second;
    }
    i++;
    delay(1000);
  }
  i = 0;
  while(i<3 && sec2 == 0){
    delay(1500);
    if (RTC.read(tm)) {
      sec2 = tm.Second;
    }
    i++;
  }
  
  /*displayClear();
  displaySetCursor(0, 0);
  displayPrint(sec1);  displayPrint(" ");   displayPrint(sec2);
  displayDisplay();
  delay(3000);*/
  
  if(sec1 != 0 && sec2 != 0 && sec1 != sec2){
    return 1;
  }
  return 0;
}


int dumpFRAM(){

  FRAM fram(0b00);
    uint16_t i;
		//fram.WriteByte(0, 10, 0xAA);
    for(i=0;i<64;i++){
		//	fram.WriteByte(0,i,0);
      //Serial.print(fram.ReadByte(0,i));Serial.print(F(","));
    }
}

int checkFRAM(){
  FRAM fram(0b00);
  uint8_t existingVal = fram.ReadByte(0,0x0);
  fram.WriteByte(0, 0x0, 0xaa);
  uint8_t test = fram.ReadByte(0, 0x0);
  fram.WriteByte(0, 0x0, existingVal);
  if(test == 0xaa){
    return 1;
  }else{
    return 0;
  }
}

void testIO(){
  //DI0 - PB2 - D10
  //DI1 - PB3 - D11
  //DI2 - PB4 - D12
  //DI3 - PB5 - D13

  //DO0 - PB0 - D8   
  //DO1 - PD7 - D7
  //DO2 - PD6 - D6
  //DO3 - PD5 - D5
  displayClear();
  displaySetTextNormal();
  displaySetCursor(0, 0);
  long l = digitalRead(10);
  displayPrint(l);
  l = digitalRead(11);
  displayPrint(l);
  l = digitalRead(12);
  displayPrint(l);
  l = digitalRead(13);
  displayPrint(l);
  displayDisplay();

  delay(2000);
  digitalWrite(5, 1);
  delay(2000);
  digitalWrite(5, 0);
  delay(2000);
  digitalWrite(6, 1);
  delay(2000);
  digitalWrite(6, 0);
  delay(2000);
  digitalWrite(7, 1);
  delay(2000);
  digitalWrite(7, 0);
  delay(2000);
  digitalWrite(8, 1);
  delay(2000);
  digitalWrite(8, 0);
}

void setup() {
  //Serial.begin(9600);
  setupGUI();
  
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1=0;

  OCR1A=20000;
  
  TCCR1B|=(1<<CS11)|(1<<WGM12);
  TIMSK1|=(1<<OCIE1A);
  interrupts();

  //test connected devices
  displayClear();
  displaySetTextNormal();
  displaySetCursor(0, 0);
  printA(message, OLED_TEST_PASSED);
  displaySetCursor(0, 8);

  if(checkRTC()){
    printA(message, RTC_TEST_PASSED);
    displayDisplay();
  }else{
    printA(message, RTC_TEST_NOT_PASSED);
    displayDisplay();
  }

  displaySetCursor(0, 16);

  if(checkFRAM()){
    printA(message, FRAM_TEST_PASSED);
    displayDisplay();
  }else{
    printA(message, FRAM_TEST_NOT_PASSED);
    displayDisplay();
  }

  //dumpFRAM();

  delay(4000);
  displayClear();
  displaySetTextNormal();
  displaySetCursor(0, 0);
}

const char _0dot[] PROGMEM = {""};
const char _1dot[] PROGMEM = {"."};
const char _2dot[] PROGMEM = {".."};
const char _3dot[] PROGMEM = {"..."};
const char* const runningPromptArray[] PROGMEM = {_0dot, _1dot, _2dot, _3dot};

uint8_t exitRunningCounter;
uint8_t screenSaverCounter;
uint8_t runningIndCounter;
uint8_t runningIndCounterPrev;

void runEvery500ms(){
  runningIndCounter++;
  if(screenSaverCounter<SCREEN_SAVER_TIME)
    screenSaverCounter++;
  if(EXIT_RUNNNING_BUTTONS(buttons))
    exitRunningCounter--;
  else{
    exitRunningCounter = EXIT_RUNNING_TIME;
  }
}

int timerCounter = 0;
ISR(TIMER1_COMPA_vect){
  timersRoutine();
  timerCounter++;
  if(timerCounter>=50){
    runEvery500ms();
    timerCounter = 0;
  }
}

void runProgram(){
  //load
  setupMem();
  displayClear();
  displaySetTextNormal();
  
  runningIndCounter = 0;
  runningIndCounterPrev = 0;
  screenSaverCounter = 0;
  exitRunningCounter = EXIT_RUNNING_TIME;
  
  //run
  if(readProgramFromEeprom()){
    programChanged = 0;
    while(exitRunningCounter>0){

      readAnalog();
      writeAnalog();
      
      displayClear();
      if(screenSaverCounter < 60){

        //display running indicator
        if(runningIndCounterPrev != runningIndCounter){
          
          displaySetCursor(0, 0);
          printA(message, RUNNING_MSG);
          printA(runningPromptArray, runningIndCounter%4);
  
          //display values
          uint8_t lines = 0;
          int16_t value;
          if(m[1] & (1 << 4)){value = m[58] + (m[59] << 8); lines++;;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          if(m[1] & (1 << 5)){value = m[60] + (m[61] << 8); lines++;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          if(m[1] & (1 << 6)){value = m[62] + (m[63] << 8); lines++;displaySetCursor(0, lines*8);displayPrint(lines);printA(message, COLON); displayPrint(value);}
          
          displayDisplay();
          runningIndCounterPrev = runningIndCounter;
        }
      }
      else if (screenSaverCounter==60){
        displayDisplay();
        screenSaverCounter=61;
      }

      uint8_t newButtons = ~getButtonsNoneBlocking();

      if(screenSaverCounter<60)
        buttons = newButtons;
        
      if(buttons != newButtons)
        screenSaverCounter=0;
      
      executeCommandAt(PC);
      PC++;
      if(PC>=PS){
        onLoopEnd();
        PC=0;
      }
    }

    //readProgramFromEeprom();
    
  }else{
    displayClear();
    displaySetCursor(0, 0);
    printA(message, NOPROGRAM_MSG);
    displayDisplay();
    delay(3000);

    // display buttons byte
    /*while(true){
      displayClear();
      uint8_t newButtons = ~getButtonsNoneBlocking();
            
      displaySetCursor(0, 0);
      displayPrint(newButtons);
      displayDisplay();
    }*/
  }
}

int newMenuPosition = -2;

void setupDateTime(){
	tmElements_t tm;
	RTC.read(tm);
	uint8_t value = enterValue(ENTER_DAY_MSG, tm.Day, 0, 2, 9);
	if(value < 1 || value > 31){
		printMessageAndWaitForButton(MUST_BE_IN_RANGE, 1, 31);
		return;				
	} 
	tm.Day = value;
	value = enterValue(ENTER_MONTH_MSG, tm.Month, 0, 2, 9);
	if(value < 1 || value > 12){
		printMessageAndWaitForButton(MUST_BE_IN_RANGE, 1, 12);
		return;				
	} 
	tm.Month = value;
	value = enterValue(ENTER_YEAR_MSG, tmYearToY2k(tm.Year), 0, 2, 9);
	if(value < 23 || value > 99){
		printMessageAndWaitForButton(MUST_BE_IN_RANGE, 23, 99);
		return;				
	} 
	tm.Year = y2kYearToTm(value);	
	value = enterValue(ENTER_HOUR_MSG, tm.Hour, 0, 2, 9);
	if(value < 0 || value > 23){
  	printMessageAndWaitForButton(MUST_BE_IN_RANGE, 0, 23);
    return;
	}
	tm.Hour = value;
	value = enterValue(ENTER_MINUTE_MSG, tm.Minute, 0, 2, 9);
	if(value < 0 || value > 59){
		printMessageAndWaitForButton(MUST_BE_IN_RANGE, 0, 59);
		return;
	}
	tm.Minute = value;
	RTC.write(tm);
}

void loop() {

  switch(newMenuPosition){
    case -1: break;
    case 0:{ 
          int8_t res = showMenu(runMenu, NULL, 0, 3);
          switch(res){
            case 0:writeProgramToEeprom();runProgram();break;
            case 1:runProgram();break;
            case 2:break; 
            default:break;
          }
          break;
          } 
    case 1: editProgram(); break;
    case 2: if(programChanged)writeProgramToEeprom();else printMessageAndWaitForButton(NO_CHANGES);break;
    case 3: printA(message, CLEAR_LOCAL_PROGRAM_MSG); displayDisplay(); clearProgramLocal(); delay(1000); break;
    case 4: setupDateTime(); break;
    default: runProgram(); break;
  }
  
  newMenuPosition = showMenu(mainMenu, NULL, 0, MAIN_MENU_SIZE);
     
}
