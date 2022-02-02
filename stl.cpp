
#include <Arduino.h>
#include "stl.h"
#include "messages.h"
#include "gui.h"

uint32_t program[MAX_PROGRAM_SIZE];
int32_t accumulator[2];
uint8_t nullByte;
uint8_t programChanged = 1;
void _nop(uint32_t param);

void (*func_ptr[])(uint32_t) = {_nop, _and, _or, _nand, _nor, _assign, _s, _r, _fp, _fn, 
      _l, _t, 
      _sp, _se, _sd, _ss, _sf, _rt, _cu, _cd, _cs, _cr, _cl, 
       _addI, _subI, _mulI, _divI,
       _eqI, _diffI, _gtI, _ltI, _gteqI, _lteqI,
       _ju, _jc, _jcn};
 
uint8_t volatile buttons;
uint8_t volatile m[64];
uint8_t volatile t[8];
uint8_t volatile ai[8];
uint8_t volatile ao[12];
uint8_t volatile c;

uint8_t volatile * const memNull[] = {&nullByte};
uint8_t volatile * const memDI[] = {&nullByte};
uint8_t volatile * const memDO[] = {&nullByte};
uint8_t volatile * const memM[] = {&m[0], &m[1], &buttons, &m[3], &m[4], &m[5], &m[6], &m[7],
                                   &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15],
                                   &m[16], &m[17], &m[18], &m[19], &m[20], &m[21], &m[22], &m[23], 
                                   &m[24], &m[25], &m[26], &m[27], &m[28], &m[29], &m[30], &m[31], 
                                   &m[32], &m[33], &m[34], &m[35], &m[36], &m[37], &m[38], &m[39], 
                                   &m[40], &m[41], &m[42], &m[43], &m[44], &m[45], &m[46], &m[47], 
                                   &m[48], &m[49], &m[50], &m[51], &m[52], &m[53], &m[54], &m[55], 
                                   &m[56], &m[57], &m[58], &m[59], &m[60], &m[61], &m[62], &m[63]};
uint8_t volatile * const memC[] = {&c};
uint8_t volatile * const memT[] = {&t[0], &t[1], &t[2], &t[3], &t[4], &t[5], &t[6], &t[7]};
uint8_t volatile * const memAI[] = {&ai[0], &ai[1], &ai[2], &ai[3], &ai[4], &ai[5], &ai[6], &ai[7]};
uint8_t volatile * const memAO[] = {&ao[0], &ao[1], &ao[2], &ao[3], &ao[4], &ao[5], &ao[6], &ao[7], &ao[8], &ao[9], &ao[10], &ao[11]};

uint8_t volatile activeAImask;
uint8_t volatile fixedTimer[8];
uint32_t volatile timer[8];
int32_t volatile counter[8];

const PROGMEM uint8_t fixedTimerTime[]  = {10, 20, 40, 50, 80, 100, 160, 200};

uint8_t volatile *const *memMap[] = {
  memNull,
  memDI,
  memT,
  memM,
  memDO,
  memNull,
  memAI,
  memM,
  memM,
  memM,
  memAO,
  memC,
  memNull
};

uint8_t volatile RLO = 1;
uint8_t volatile cancel_RLO = true;
int8_t volatile PC = 0, PS = 0;

uint8_t mem_ptr, bit_pos, mask, mem_id;
int8_t addr;

void writeProgramToEeprom(){
  printA(message, PROGRAMMING_EEPROM);
  displayDisplay();
  int addr = 0;
  for(uint8_t i=0; i<PS; i++){
    EEPROM.write(addr, (program[i] >> 24) & 0xFF);
    EEPROM.write(addr+1, (program[i] >> 16) & 0xFF);
    EEPROM.write(addr+2, (program[i] >> 8) & 0xFF);
    EEPROM.write(addr+3, program[i] & 0xFF);
    addr+=4;
  }
  //Write PS and the end of eeprom
  EEPROM.write(0x3FF, PS);
  programChanged = 1;
  delay(1000);
}

void readProgramFromEeprom(){
  int addr = 0;
  PS = EEPROM.read(0x3FF);
  if(PS == 0xFF)
    PS = 0;
  //Serial.println(PS);
  for(uint8_t i=0; i<PS; i++){
    program[i] = ((uint32_t)EEPROM.read(addr) << 24UL) +
                  ((uint32_t)EEPROM.read(addr+1) << 16UL)+
                  ((uint32_t)EEPROM.read(addr+2) << 8UL) +
                  ((uint32_t)EEPROM.read(addr+3));
    addr+=4;
  }
  programChanged = 1;
}

void clearProgramLocal(){
  for(uint8_t i=0; i<PS; i++){
      program[i] = 0;
  }
  programChanged = 0;
  PS = 0;
}

void extractParams(uint32_t param){
  mem_ptr = (param >> MEM_BIT_POS) & 0xFF;
  mem_id = param >> 4 & 0xFF;
  bit_pos = param & 0x7;
}

void setupMem(){

  int i;
  for(i=0; i<64; i++)m[i] = 0;
  for(i=0; i<8; i++)t[i] = 0;
  for(i=0; i<8; i++)ai[i] = 0;
  for(i=0; i<12; i++)ao[i] = 0;
  for(i=0; i<8; i++)fixedTimer[i] = 0;
  for(i=0; i<8; i++)timer[i] = 0;
  for(i=0; i<8; i++)counter[i] = 0;
  c = 0;

  m[1] |=  1 << 0; //FirstScan
  m[1] |=  0 << 1; //DiagStatusUpdate
  m[1] |=  1 << 2; //AlwaysTrue
  m[1] |=  0 << 3; //AlwaysFalse

  m[1] |=  0 << 4; //Display 1 value dw29
  m[1] |=  0 << 5; //Display 2 value dw30
  m[1] |=  0 << 6; //Display 3 value dw31

  for(int i=0; i<12; i++){
    if(i==9 || i==10)continue;//problematic outputs
    analogWrite(i, 0);
  }

  activeAImask = 0x00;
}

void onLoopEnd(){
  m[1] &= ~(1<<0);
  accumulator[1] = 0;
  accumulator[0] = 0;
  RLO=0;
  cancel_RLO=true;
}

void timersRoutine(){//10ms
  for(uint8_t i=0; i<8; i++){
    timer[i]+=10;
    
    fixedTimer[i]+=1;
    if(fixedTimer[i] >= pgm_read_byte_near(fixedTimerTime + i)){
      //Serial.println(fixedTimer[i]);
      fixedTimer[i] = 0;
      m[0] ^= 1<<i;
      
    }
  }
}

void executeCommand(uint32_t instr){
  uint8_t func_id = instr >> FUNC_BIT_POS;
  (*func_ptr[func_id])(instr);
}

void executeCommandAt(int pl){
  executeCommand(program[pl]);
}

int32_t popFromACC(){
  int32_t res = accumulator[0];
  accumulator[0]=accumulator[1];
  accumulator[1]=0;
  //Serial.print("after pop: ");Serial.print(accumulator[0]);Serial.print(",");Serial.println(accumulator[1]);
  return res;
}

void pushToAcc(uint32_t param){
  accumulator[1] = accumulator[0];
  accumulator[0] = (int32_t)param;
  //Serial.print("after push: ");Serial.print(accumulator[0]);Serial.print(",");Serial.println(accumulator[1]);
}

void readAnalog(){
  for(int i=0; i<8; i++){
    //Serial.print("activeAImask: ");Serial.print(activeAImask);Serial.print(", ");Serial.print(" 1<<i: ");Serial.println( 1<<i);
    if((activeAImask & 1<<i) == 1<<i){
      ai[i] = analogRead(i) >> 2; //10-bit to 8-bit
      //Serial.print("reading ai: ");Serial.print(i);
    }
  }
}

void writeAnalog(){
  for(int i=0; i<12; i++){
     //Serial.print("analogWrite");Serial.println(i);
     if(ao[i]>0)analogWrite(i, ao[i]);
  }
}

volatile uint8_t *getMemPtr(uint8_t ptr, uint8_t id){
  return memMap[ptr][id];
}

void setMem(uint8_t ptr, uint8_t id, uint8_t val){
  *memMap[ptr][id] = val;
}

uint8_t getMemBit(uint8_t ptr, uint8_t id, uint8_t b){
  if(ptr == 1){
    return digitalRead(id);
  }
  return (*getMemPtr(ptr, id)>>b) & 0x1;
}

void setMemBit(uint8_t ptr, uint8_t id, uint8_t b, uint8_t v){
  if(ptr == 4){
    digitalWrite(id,v);
  }else{
    mask = 1 << b;
    *memMap[ptr][id] = ((*memMap[ptr][id] & ~mask) | v << b);
  }
}

void resetMemBit(uint8_t ptr, uint8_t id, uint8_t b){
  mask = 1 << b;
  if(ptr == 4){
    digitalWrite(id,0);
  }else{
    *memMap[ptr][id] = (*memMap[ptr][id] & ~mask);
  }
}

void _nop(uint32_t param){}

void _and(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = getMemBit(mem_ptr,mem_id,bit_pos);
  else RLO &= getMemBit(mem_ptr,mem_id,bit_pos);
  cancel_RLO = false;
}

void _nand(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = ~getMemBit(mem_ptr,mem_id,bit_pos)&0x1;
  else RLO &= (~getMemBit(mem_ptr,mem_id,bit_pos)&0x1);
  cancel_RLO = false;
}

void _or(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = getMemBit(mem_ptr,mem_id,bit_pos);
  else RLO |= getMemBit(mem_ptr,mem_id,bit_pos); 
  cancel_RLO = false;
}

void _nor(uint32_t param){
  extractParams(param);
  if(cancel_RLO) RLO = ~getMemBit(mem_ptr,mem_id,bit_pos)&0x1;
  else RLO |= (~getMemBit(mem_ptr,mem_id,bit_pos)&0x1);
  cancel_RLO = false;
}

void _assign(uint32_t param){
  extractParams(param);
  setMemBit(mem_ptr,mem_id,bit_pos,RLO);
  cancel_RLO = true;
}

void _s(uint32_t param){
  extractParams(param);
  if(RLO==0x1)
    setMemBit(mem_ptr,mem_id,bit_pos,0x1);
  cancel_RLO = true;
}

void _r(uint32_t param){
  extractParams(param);
  if(RLO==0x1)
    resetMemBit(mem_ptr,mem_id,bit_pos);
  cancel_RLO = true;
}

void _fp(uint32_t param){
  extractParams(param);
  //mask = 1 << bit_pos;
  uint8_t m = getMemBit(mem_ptr,mem_id,bit_pos);
  if(RLO == 0)resetMemBit(mem_ptr,mem_id,bit_pos);//*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask));
  if(RLO == 0x1 && m == 0x0){
    RLO = 0x1;
    //*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | 1 << bit_pos);
    setMemBit(mem_ptr,mem_id,bit_pos,0x1);
  }else{RLO = 0x0;}
  cancel_RLO = false;
}

void _fn(uint32_t param){
  extractParams(param);
  mask = 1 << bit_pos;
  uint8_t m = getMemBit(mem_ptr,mem_id,bit_pos);
  if(RLO == 1)setMemBit(mem_ptr,mem_id,bit_pos,0x1);//*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask) | 1 << bit_pos);
  if(RLO == 0x0 && m == 0x1){
    RLO = 0x1;
    //*memMap[mem_ptr][mem_id] = ((*memMap[mem_ptr][mem_id] & ~mask));
    resetMemBit(mem_ptr,mem_id,bit_pos);
  }else{RLO = 0x0;}  
  cancel_RLO = false;
}

void _l(uint32_t param){
  extractParams(param);
  
  uint32_t temp = 0;
  uint8_t type = mem_ptr-7;//0,1,2,3,4
  uint8_t bytes = 1 << type; //byte, word, dword

  if(mem_ptr == CS){ //const //dodaj AI
    temp = param & 0xFFFF;
  }
  else if(mem_ptr == AI){ //const //dodaj AI
    activeAImask = 1 << mem_id;
    temp = *getMemPtr(mem_ptr,mem_id);
    //Serial.print("activeAImask: ");Serial.println(activeAImask);
  }
  else{
    for(uint8_t i=0; i<bytes; i++){
      uint32_t t = *getMemPtr(mem_ptr,mem_id*bytes+i);
      temp += t<<(i*8); 
   }
  }
  pushToAcc(temp);
  
}

void _t(uint32_t param){
  extractParams(param);
  
  int32_t temp = popFromACC();

  //dodaj AO
  if(mem_ptr == AO){ //const //dodaj AI
    setMem(mem_ptr, mem_id, temp&0xFF);
  }
  else
  {
    uint8_t type = mem_ptr-7;//0,1,2,3,4
    uint8_t bytes = 1 << type; //byte, word, dword
    for(uint8_t i=0; i<bytes; i++){
      setMem(mem_ptr, mem_id*bytes+i, temp>>(i*8)&0xFF); 
    }
  }
}

//change getting mem_ptr to using function

void _sp(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x3;
      timer[mem_id]=0;
    }

    //stop
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] &= ~(0x1);
    }
    
  }else{
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }
  cancel_RLO = true;
}

void _se(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x3;
      timer[mem_id]=0;
    }
  }

  //stop
  if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  cancel_RLO = true;
}

void _sd(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x2;
      timer[mem_id]=0;
    }

    //stop
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] |= 0x1;
    }
    
  }else{
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  cancel_RLO = true;
}

void _ss(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] = 0x2;
      timer[mem_id]=0;
    }
    
  }else{
    //*memMap[mem_ptr][mem_id] &= ~(0x3);
  }

  //stop
  if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
    *memMap[mem_ptr][mem_id] |= 0x1;
  }

  cancel_RLO = true;
}

void _sf(uint32_t param){
  extractParams(param);

  if(RLO == 1){

    //start
    if((*memMap[mem_ptr][mem_id] & 0x2) == 0){
      *memMap[mem_ptr][mem_id] |= 0x1;
    }
    
  }else{
    if((*memMap[mem_ptr][mem_id] & 0x3) == 0x1){
      *memMap[mem_ptr][mem_id] |= 0x3;
      timer[mem_id]=0;
    }
    
    if(timer[mem_id]>accumulator[0] && (*memMap[mem_ptr][mem_id] & 0x2)){
      *memMap[mem_ptr][mem_id] &= ~(0x3);
    }
  }

  cancel_RLO = true;  
}

void _rt(uint32_t param){
  extractParams(param);

  if(RLO == 1){
    *memMap[mem_ptr][mem_id] &= ~(0x3);
  }
  cancel_RLO = true;
}

void _cu(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]++;
  }
  cancel_RLO = true;
}

void _cd(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]--;
  }
  cancel_RLO = true;
}

void _cs(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]=accumulator[0];
  }
  cancel_RLO = true;
}

void _cr(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  if(RLO == 1){
    counter[mem_id]=0;
  }
  cancel_RLO = true;
}

void _cl(uint32_t param){
  mem_id = (param >> 4) & 0xFF;
  pushToAcc(counter[mem_id]);
  //cancel_RLO = true;
}

int32_t accI0 = 0, accI1 = 0;
void _addI(uint32_t param){accI0 = (int32_t)(accumulator[1])+(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _subI(uint32_t param){accI0 = (int32_t)(accumulator[1])-(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _mulI(uint32_t param){accI0 = (int32_t)(accumulator[1])*(int32_t)(accumulator[0]); accumulator[0] = accI0;}
void _divI(uint32_t param){accI0 = (int32_t)(accumulator[1])/(int32_t)(accumulator[0]); accumulator[0] = accI0;}

void loadIFromAcc(){
  accI0 = (int32_t)accumulator[0]; 
  accI1 = (int32_t)accumulator[1];
}
 
void _eqI(uint32_t param){loadIFromAcc(); RLO=(accI1==accI0); cancel_RLO=false;}
void _diffI(uint32_t param){loadIFromAcc(); RLO=(accI1!=accI0); cancel_RLO=false;}
void _gtI(uint32_t param){loadIFromAcc(); RLO=(accI1>accI0); cancel_RLO=false;}
void _ltI(uint32_t param){loadIFromAcc(); RLO=(accI1<accI0); cancel_RLO=false;}
void _gteqI(uint32_t param){loadIFromAcc(); RLO=(accI1>=accI0); cancel_RLO=false;}
void _lteqI(uint32_t param){loadIFromAcc(); RLO=(accI1<=accI0); cancel_RLO=false;}

void _ju(uint32_t param){
  addr = param & 0xFF;
  PC = addr - 2;
}

void _jc(uint32_t param){
  addr = param & 0xFF;
  if(RLO == 1)PC = addr - 2;
  cancel_RLO = true;
}

void _jcn(uint32_t param){
  addr = param & 0xFF;
  if(RLO == 0)PC = addr - 2;
  cancel_RLO = true;
}
