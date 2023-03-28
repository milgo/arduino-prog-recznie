#include "gui.h"

#include "PCF8574.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <avr/pgmspace.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
PCF8574 pcf20(0x20);

int menuPosition = 0;
int selectedPosition = 0;
char bufferStr[16];

void setupGUI(){

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.setTextWrap(false);

  displayDisplay();
  delay(2000);

  displayClear();
  displayDisplay();
}

uint8_t getButtonsBlocking(){
  while(true){
    for(int i=0; i<8; i++){
      int buttonStatus = pcf20.readButton(i);
      if(!buttonStatus){
        while(true){
          buttonStatus = pcf20.readButton(i);
          if(buttonStatus)
            return (1<<i);
        }
      }
    }
  }
  return 0x0;
}

uint8_t getButtonsNoneBlocking(){
  return pcf20.read8();
}


void enterCurrentOption(int newMenuPosition){
  menuPosition = (newMenuPosition + 1) * 4;
  selectedPosition = menuPosition;
  Serial.println(menuPosition);
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

      strcpy_P(bufferStr, (char*)pgm_read_word(&(menu[from+i])));
      display.print(bufferStr);
      

      if(descMenu){
        display.print(F(":"));
        strcpy_P(bufferStr, (char*)pgm_read_word(&(descMenu[from+i])));
        display.println(bufferStr);
      }else{
        display.println();
      }
        
      if(pos == i){
        displaySetTextNormal();
      }
    }

    displayDisplay();
    newButtons = getButtonsBlocking();

    Serial.println(newButtons, BIN);
    
    delay(100);
  }
}

int32_t enterValue(int msg, long int curVal, bool isSigned, int len, int maxDigit){
  unsigned char newButtons = 0;
  int32_t retVal = 0;
  byte v[10];

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

    strcpy_P(bufferStr, (char*)pgm_read_word(&(message[msg])));
    display.print(bufferStr);
    
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
        display.print("^ ");
        //display.print("]");
      }

      displaySetCursor(i*6, 8);
      if(i==0){
        if(v[0]==0)display.print("+");
        else display.print("-");
      }else{
        strcpy_P(bufferStr, (char*)pgm_read_word(&(numbers[v[i]])));
        display.print(bufferStr);
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

void displaySetTextNormal(){
  display.setTextColor(SSD1306_WHITE); 
}

void displaySetTextInvert(){
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  
}

void displayClear(){
  display.clearDisplay();
}

void displayDisplay(){
  display.display();
}

void displaySetCursor(uint8_t x, uint8_t y){
  display.setCursor(x, y);
}

void displayPrint(const char* msg){
  display.print(msg);
}

void displayPrint(long i){
  display.print(i);
}

void printA(const char *const* arr, int id){
  strcpy_P(bufferStr, (char*)pgm_read_word(&(arr[id])));
  display.print(bufferStr);//display.print(" ");
}

void printAtoBuf(const char *const* arr, int id, char* buf){
  strcpy_P(buf, (char*)pgm_read_word(&(arr[id])));
}

uint8_t printMessageAndWaitForButton(int msg, uint8_t showVal, int v1, int v2){
  display.setTextWrap(true);
  uint8_t newButtons = 0;
  while(true){
    displayClear();
    displaySetCursor(0, 0);
    strcpy_P(bufferStr, (char*)pgm_read_word(&(message[msg])));
    display.print(bufferStr);
    if(showVal == 0){
      display.print(v1);display.print(", ");
      display.print(v2);
    }
    displayDisplay();
    newButtons = getButtonsBlocking();
    if(newButtons>0)
      break;
  }
  displayClear();
  displaySetCursor(0, 0);
  return newButtons;
  display.setTextWrap(false);
}

uint8_t printMessageAndWaitForButton(int msg, int v1, int v2){
  return printMessageAndWaitForButton(msg, 0, v1, v2);
}

uint8_t printMessageAndWaitForButton(int msg){
  return printMessageAndWaitForButton(msg, 1, 0, 0);
}
