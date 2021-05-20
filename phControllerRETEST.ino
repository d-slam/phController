/*
  phController - Main 
  Created by D. -Haxormän- Ami, 21 Mai, A. D. 2021 ACorona
  Released into the Wild.
*/
#include "enums.h"

//VERY!GLOBALS===============================================
#define MOTORGATE 2

#include "LCDScreen.h"
#include "PhSonde.h"

//GLOBALS===============================================
float volt = 0.0;
float phIst = 0.0;
float phLast = 0.0;
float phSoll = 5.5;
float phSollThres = 0.5;

LCDScreen lcdScreen(&phLast, &phSoll, &phSollThres);

PhSonde phSonde;

bool incFlag = false;
bool decFlag = false;
bool btnFirstFlag = false;


//ACHTUNG!STATMASCINE===============================================
state_t state = SYS_WAIT; //init State

unsigned char incSYS_RUN;

void stateMachine() //~~~♪callMe from main()
{
  switch (state)  {

  case SYS_RUN_INTERFACE:
    incSYS_RUN = 4;                 //soviele durchläufe bis neuerCheck
    doSYS_RUN_INTERFACE();          //kurz check wia IST zu SOLL steat donn subito flag auf RED,YELLOW,GREEN
    break;

  case SYS_RUN_RED:
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

  case SYS_WAIT:
    doSYS_WAIT();
    checkForNewButtonPress();
    break;

  case SYS_SET_SOLL:
    doSYS_SET_SOLL();
    checkForNewButtonPress();
    break;

  case SYS_SET_THRES:
    doSYS_SET_THRES();
    checkForNewButtonPress();
    break;

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


  case VOID_DUMMY: //void case, plotzholter!!
  {
    doVOID_DUMMY();
    // // Ampel1(ROT);
    // // Ampel2(GRUEN);

    // lcdScreen.redraw(state);
    // phLast = phSonde.getPhIst();

    // if (exitCheck())
    //   state == SYS_RUN;
    break;
  }

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

void doRUN_RED()    {  digitalWrite(MOTORGATE, HIGH);   }
void doRUN_YELLOW() {  digitalWrite(MOTORGATE, LOW);    }
void doRUN_GREEN()  {  digitalWrite(MOTORGATE, LOW);    }

void doSYS_WAIT()
{
  digitalWrite(MOTORGATE, LOW);
  phLast = phSonde.getPhIst();
}

void doSYS_SET_SOLL()
{
  if (incFlag == true)    incSoll();
  if (decFlag == true)    decSoll();
}
void doSYS_SET_THRES()
{
  if (incFlag == true)    incThres();
  if (decFlag == true)    decThres();
}

void doSYS_CAL()
{

}

void doCAL_PH4()
{
  phLast = phSonde.getPhIst();

}
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
void doCAL_OK()
{
  phSonde.applyCallibration();
}

void doVOID_DUMMY()
{
}

//MyMethodes==========================================================

//BtnInterface===============================================
void checkForNewButtonPress()
{
  lcd_key = read_LCD_buttons();   //read BUTTON

  switch (lcd_key)                //check welcher BUTTON?
  {
  //wenn noBUTTON, [btnFirstFlagFALSE} u EXIT; wenn button PRESS [DO] u [btnFirstFlagFALSE]
  case btnNONE:    btnFirstFlag = false;    break;

  //wenn BUTTON, check ob schun aknowledged->donn break, SUSCHT resetFLAG u DOSHIT()
  case btnRIGHT:        if (btnFirstFlag == false)      break;
  case btnLEFT:         if (btnFirstFlag == false)      break;
  case btnSELECT:       if (btnFirstFlag == false)      break;
  case btnUP:           if (btnFirstFlag == false)      break;
  case btnDOWN:         if (btnFirstFlag == false)      break;
    btnFirstFlag = false; //resetFLAG
    doNewButton(lcd_key);    //DOSHIT
    break;
  }

}
//MENUMAP===============================================
void doNewButton(int keyPressed)
{
  switch (state)
  {
  case SYS_RUN_INTERFACE:
    if (keyPressed == btnRIGHT)      state = SYS_WAIT;
    break;

  case SYS_WAIT:
    if (keyPressed == btnRIGHT)      state = SYS_SET_SOLL;
    if (keyPressed == btnLEFT)      state = SYS_RUN_INTERFACE;
    break;

  case SYS_SET_SOLL:
    if (keyPressed == btnRIGHT)      state = SYS_SET_THRES;
    if (keyPressed == btnLEFT)      state = SYS_WAIT;
    if (keyPressed == btnUP)      incSoll();
    if (keyPressed == btnDOWN)      decSoll();
    break;

  case SYS_SET_THRES:
    if (keyPressed == btnRIGHT)      state = SYS_CAL;
    if (keyPressed == btnLEFT)      state = SYS_SET_SOLL;
    if (keyPressed == btnUP)      incThres();
    if (keyPressed == btnDOWN)      decThres();
    break;

  case SYS_CAL:
    if (keyPressed == btnLEFT)      state = SYS_SET_THRES;
    if (keyPressed == btnSELECT)      state = CAL_PH4;
    break;

  case CAL_PH4:
    if (keyPressed == btnSELECT)      state = CAL_PH7;
    if (keyPressed == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_PH7:
    if (keyPressed == btnSELECT)      state = CAL_CONF;
    if (keyPressed == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_CONF:
    if (keyPressed == btnSELECT)      state = CAL_OK;
    if (keyPressed == btnLEFT)      state = SYS_CAL;
    break;

  case CAL_OK:    state = SYS_WAIT;    break;
  }
}

void incSoll() { phSoll += 0.1; }
void decSoll() { phSoll -= 0.1; }
void incThres() { phSollThres += 0.1; }
void decThres() { phSollThres -= 0.1; }


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

}

