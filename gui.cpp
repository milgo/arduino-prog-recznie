#include "gui.h"

#include "PCF8574.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <avr/pgmspace.h>
#include "messages.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
PCF8574 pcf20(0x20);

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

void displayPrintln(const char* msg){
  display.println(msg);
}

void displayPrintln(){
  display.println();
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
