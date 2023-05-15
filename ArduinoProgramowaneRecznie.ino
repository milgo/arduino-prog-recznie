#include "messages.h"
#include "stl.h"
#include "gui.h"
#include "editor.h"

#include "Adafruit_FRAM_I2C.h"

Adafruit_EEPROM_I2C i2ceeprom;

#define SCREEN_SAVER_TIME 60
#define EXIT_RUNNING_TIME 6
#define EXIT_RUNNNING_BUTTONS(BUTTONS) IS_PRESSED(BUTTONS, BUTTON_LEFT) && IS_PRESSED(BUTTONS, BUTTON_RIGHT)

void setup() {
  Serial.begin(9600);
  setupGUI();
  
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1=0;

  OCR1A=20000;
  
  TCCR1B|=(1<<CS11)|(1<<WGM12);
  TIMSK1|=(1<<OCIE1A);
  interrupts();
  
  readProgramFromEeprom();
  programChanged = 1;


  displayClear();
  displaySetTextNormal();
  displaySetCursor(0, 0);
  if (i2ceeprom.begin(0x50)) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    displayPrint("Found I2C FRAM");
    displayDisplay();
    i2ceeprom.write(0x0, 0xaa);
  } else {
    displayPrint("FRAM not identified");
    displayDisplay();
    //while (1);
  }
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
  if(program[0]!=0x0){
    
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

    readProgramFromEeprom();
    
  }else{
    displayClear();
    displaySetCursor(0, 0);
    printA(message, NOPROGRAM_MSG);
    displayDisplay();
    delay(3000);
  }
}

int newMenuPosition = -2;

void loop() {
  
  displayClear();
  displaySetTextNormal();
  displaySetCursor(0, 0);
  //printA(message, NOPROGRAM_MSG);
  //displayPrint("Hello");
  uint8_t test = i2ceeprom.read(0x0);
  displayPrint(test);
  displayDisplay();
  /*switch(newMenuPosition){
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
    case 2: if(programChanged==0)writeProgramToEeprom();else printMessageAndWaitForButton(NO_CHANGES);break;
    case 3: printA(message, CLEAR_LOCAL_PROGRAM_MSG); displayDisplay(); clearProgramLocal(); delay(1000); break;
    default: runProgram(); break;
  }
  
  newMenuPosition = showMenu(mainMenu, NULL, 0, MAIN_MENU_SIZE);*/
     
}
