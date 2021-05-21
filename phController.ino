/*
  phController - Main 
  Created by D. -Haxormän- Ami, 21 Mai, A. D. 2021 ACorona
  Released into the Wild.
*/
#include "enums.h"

//VERY!GLOBALS===============================================

#include "LCDScreen.h"

#include "InputButtons.h"
#include "PhSonde.h"

#define MOTORGATE 2

//GLOBALS===============================================
float volt = 0.0;
float phIst = 0.0;
float phLast = 0.0;
float phSoll = 5.5;
float phSollThres = 0.5;

LCDScreen lcdScreen(&phLast, &phSoll, &phSollThres);

PhSonde phSonde;

InputButtons inputButtons;

state_t state = SYS_WAIT;     //init State

bool btnNewIncAllowedFlag = false;
bool btnNewDecAllowedFlag = false;
bool btnNewInputAllowedFlag = false;

// unsigned char incSYS_RUN;
int incSYS_RUN = 0;

int* pBtnPressed;

void checkForNewButtonPress()
{
  int bufferKeypad = inputButtons.read_LCD_buttons(); //read BUTTON
  pBtnPressed = &bufferKeypad;

  switch (bufferKeypad)                     //WOOOS BUFFERN!!! OB MIR WOS BUFFERN??? ÜBERHAUPT
  {
  case btnNONE:
    btnNewInputAllowedFlag = true;
    break;

  case btnRIGHT:
    if (btnNewInputAllowedFlag == false)      break;
    switchState(pBtnPressed); 
    btnNewInputAllowedFlag = false;
    break;

  case btnLEFT:
    if (btnNewInputAllowedFlag == false)      break;
    switchState(pBtnPressed); 
    btnNewInputAllowedFlag = false;
    break;

  case btnSELECT:
    if (btnNewInputAllowedFlag == false)      break;
    switchState(pBtnPressed); 
    btnNewInputAllowedFlag = false;
    break;

  case btnUP:
    if (btnNewInputAllowedFlag == false)      break;

    btnNewIncAllowedFlag = true;
    switchState(pBtnPressed); 
    btnNewIncAllowedFlag = false;
    btnNewInputAllowedFlag = false;
    break;

  case btnDOWN:
    if (btnNewInputAllowedFlag == false)      break;

    btnNewDecAllowedFlag = true;
    switchState(pBtnPressed);
    btnNewDecAllowedFlag = false;
    btnNewInputAllowedFlag = false;
    break;
  }

}

//MENUMAP===============================================
void switchState(int* pButton)
{
  switch (state)
  {

  case SYS_RUN_INTERFACE:     if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_RED:           if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_YELLOW:        if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_GREEN:         if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;

  case SYS_WAIT:
    if (*pButton == btnRIGHT)      state = SYS_SET_SOLL;
    if (*pButton == btnLEFT)      state = SYS_RUN_INTERFACE;
    break;

  case SYS_SET_SOLL:
    if (*pButton == btnRIGHT)      state = SYS_SET_THRES;
    if (*pButton == btnLEFT)      state = SYS_WAIT;
    if (*pButton == btnUP)      incSoll();
    if (*pButton == btnDOWN)      decSoll();
    break;

  case SYS_SET_THRES:
    if (*pButton == btnRIGHT)      state = SYS_CAL;
    if (*pButton == btnLEFT)      state = SYS_SET_SOLL;
    if (*pButton == btnUP)      incThres();
    if (*pButton == btnDOWN)      decThres();
    break;

  case SYS_CAL:
    if (*pButton == btnLEFT)      state = SYS_SET_THRES;
    if (*pButton == btnSELECT)      state = CAL_PH4;
    break;

  case CAL_PH4:
    if (*pButton == btnSELECT)      state = CAL_PH7;
    if (*pButton == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_PH7:
    if (*pButton == btnSELECT)      state = CAL_CONF;
    if (*pButton == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_CONF:
    if (*pButton == btnSELECT)      state = CAL_OK;
    if (*pButton == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_OK:    state = SYS_WAIT;    break;
  }
}


//SETUP===============================================
void setup()
{
  pinMode(MOTORGATE, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial hüü!");
  lcdScreen.drawStartScreen();
}

//LOOP==========================================================
void loop()
{  
  stateMachine();
  delay(20);
}

//MyMethodes==========================================================

//ACHTUNG!STATMASCINE===============================================
void stateMachine() //~~~♪callMe from main()
{
  switch (state)  {
//RUN================================
  case SYS_RUN_INTERFACE:
    incSYS_RUN = 4;                 //soviele durchläufe bis neuerCheck
    doSYS_RUN_INTERFACE();          //kurz check wia IST zu SOLL steat donn subito flag auf RED,YELLOW,GREEN
    break;

  case SYS_RUN_RED:
  Serial.println("hey run red");
    doRUN_RED();
    checkForNewButtonPress();
    incSYS_RUN = incSYS_RUN - 1;
    if (incSYS_RUN == 0)
      state = SYS_RUN_INTERFACE;
    break;

  case SYS_RUN_YELLOW:
    doRUN_YELLOW();
    checkForNewButtonPress();
    incSYS_RUN = incSYS_RUN - 1;
    if (incSYS_RUN == 0)
      state = SYS_RUN_INTERFACE;
    break;

  case SYS_RUN_GREEN:
    doRUN_GREEN();
    checkForNewButtonPress();
    incSYS_RUN = incSYS_RUN - 1;
    if (incSYS_RUN == 0)
      state = SYS_RUN_INTERFACE;
    break;

//WAIT================================
  case SYS_WAIT:
    doSYS_WAIT();
    checkForNewButtonPress();
    break;
//SET================================
  case SYS_SET_SOLL:
    doSYS_SET_SOLL();
    checkForNewButtonPress();
    break;

  case SYS_SET_THRES:
    doSYS_SET_THRES();
    checkForNewButtonPress();
    break;

//CAL================================
  case SYS_CAL:
    doSYS_CAL();
    checkForNewButtonPress();
    break;

  case CAL_PH4:
    doCAL_PH4();
    checkForNewButtonPress();
    break;

  case CAL_PH7:
    doCAL_PH7();
    checkForNewButtonPress();
    break;

  case CAL_CONF:
    doCAL_CONF();
    checkForNewButtonPress();
    break;

  case CAL_OK:
    doCAL_OK();
    checkForNewButtonPress();         //eventuell delay(1500) donn state == SYS_WAIT
    break;

  }

  lcdScreen.redraw(state);

}

//stateMaschine-doFnkBlock//////////////////////////////////
void doSYS_RUN_INTERFACE()
{
  phLast = phSonde.getPhIst();
  if      (phLast >= phSoll + phSollThres)    {    state = SYS_RUN_RED;     }
  else if (phLast >= phSoll)                  {    state = SYS_RUN_YELLOW;  }
  else if (phLast < phSoll)                   {    state = SYS_RUN_GREEN;   }
}

void doRUN_RED()                              {    digitalWrite(MOTORGATE, HIGH);   }
void doRUN_YELLOW()                           {    digitalWrite(MOTORGATE, LOW);    }
void doRUN_GREEN()                            {    digitalWrite(MOTORGATE, LOW);    }

void doSYS_WAIT()
{
  digitalWrite(MOTORGATE, LOW);
  phLast = phSonde.getPhIst();
}

void doSYS_SET_SOLL()
{
  if (btnNewIncAllowedFlag == true)    incSoll();
  if (btnNewDecAllowedFlag == true)    decSoll();
}
void doSYS_SET_THRES()
{
  if (btnNewIncAllowedFlag == true)    incThres();
  if (btnNewDecAllowedFlag == true)    decThres();
}

void doSYS_CAL(){}
void doCAL_PH4()                    {  phLast = phSonde.getPhIst();}
void doCAL_PH7()
{
  phLast = phSonde.getPhIst();
  phSonde.setVolt4(volt);
}
void doCAL_CONF()
{
  phSonde.setVolt7(volt);
  phSonde.calcDelta();
}
void doCAL_OK()                    { phSonde.applyCallibration(); }


void incSoll() { phSoll += 0.1; }
void decSoll() { phSoll -= 0.1; }
void incThres() { phSollThres += 0.1; }
void decThres() { phSollThres -= 0.1; }

