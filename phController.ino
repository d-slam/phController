///phController////////><(((°>//////><(((°>////////><(((°>/////
/*
  phController - Main 
  Created by D. -Tomätebauer- Ami, 21 Mæi, A. D. 2021 ACorona
  Released into the Wild.
*/
//////////////////////////////////////////////////////////////
#include "enums.h"

//MENUMAP===============================================

typedef void (*pAknNewInput)();                             //ptr fürn fknPointer deklarieren
void regCallback(pAknNewInput _ptrFkn) { (*_ptrFkn)(); }    // do londmor wenn mor in dor main in callbackCallen!

state_t state = SYS_WAIT;     //init State
void switchState(int* pButton)                
{
  switch (state)  {

  case SYS_RUN_INTERFACE:     if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_RED:           if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_YELLOW:        if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;
  case SYS_RUN_GREEN:         if (*pButton == btnRIGHT)      state = SYS_WAIT;    break;

  case SYS_WAIT:
    if (*pButton == btnRIGHT)                               state = SYS_SET_SOLL;
    if (*pButton == btnLEFT)                                state = SYS_RUN_INTERFACE;
    break;

  case SYS_SET_SOLL:
    if (*pButton == btnRIGHT)                               state = SYS_SET_THRES;
    if (*pButton == btnLEFT)                                state = SYS_WAIT;
    if (*pButton == btnUP)                                          incSoll();
    if (*pButton == btnDOWN)                                        decSoll();
    break;

  case SYS_SET_THRES:
    if (*pButton == btnRIGHT)                               state = SYS_CAL;
    if (*pButton == btnLEFT)                                state = SYS_SET_SOLL;
    if (*pButton == btnUP)                                          incThres();
    if (*pButton == btnDOWN)                                        decThres();
    break;

  case SYS_CAL:
    if (*pButton == btnLEFT)                              state = SYS_SET_THRES;
    if (*pButton == btnSELECT)                            state = CAL_PH4;
    break;

  case CAL_PH4:
    if (*pButton == btnSELECT)                            state = CAL_PH7;
    if (*pButton == btnLEFT)                              state = SYS_CAL;
    break;

  case CAL_PH7:
    if (*pButton == btnSELECT)                            state = CAL_CONF;
    if (*pButton == btnLEFT)                              state = SYS_CAL;
    break;

  case CAL_CONF:
    if (*pButton == btnSELECT)                            state = CAL_OK;
    if (*pButton == btnLEFT)                              state = SYS_CAL;
    break;

  case CAL_OK:                                            state = SYS_WAIT;    break;
  }

}

//VERY!GLOBALS===============================================
#include "LCDScreen.h"
#include "InputButtons.h"
#include "PhSonde.h"

//GLOBALS===============================================
float volt = 0.0;
float phIst = 0.0;
float phLast = 0.0;
float phSoll = 5.5;
float phSollThres = 0.5;

//MODUES===============================================
LCDScreen lcdScreen(&phLast, &phSoll, &phSollThres);
PhSonde phSonde;

InputButtons inputButtons;                    

void my_callback()                      //DE fknt werd gecallt, und über der konni auf meine buttons zuagreifen
{
  Serial.println("seas von callback...alias aknNewINp");
  inputButtons.aknNewInput();
}
pAknNewInput ptrCallback = my_callback; //fknPTR lädt die fkn myCallback

//SETUP===============================================
void setup()
{
  Serial.begin(9600);
  Serial.println("Serial hüü!");  

  lcdScreen.drawStartScreen();
  pinMode(MOTORGATE, OUTPUT);
}

//LOOP==========================================================
void loop()
{  

  regCallback(ptrCallback);       //leit un den callback!!!!

  stateMachine();
  delay(20);
}

//ACHTUNG!STATMASCINE===============================================
int decSYS_RUN = 0;
void stateMachine() //~~~♪callMe from main()
{
    inputButtons.checkForNewButtonPress();

  lcdScreen.redraw(state);
  switch (state)  {

  case SYS_RUN_INTERFACE:
    decSYS_RUN = 5;                 //soviele durchläufe bis neuerCheck
    Serial.println("grüzzi v run interface");
    doSYS_RUN_INTERFACE();          //kurz check wia IST zu SOLL steat donn subito flag auf RED,YELLOW,GREEN
    break;

  case SYS_RUN_RED:
    doRUN_RED();
    decSYS_RUN -= 1;
    if (decSYS_RUN == 0)      state = SYS_RUN_INTERFACE;
    break;

  case SYS_RUN_YELLOW:
    doRUN_YELLOW();
    decSYS_RUN -= 1;
    if (decSYS_RUN == 0)      state = SYS_RUN_INTERFACE;
    break;

  case SYS_RUN_GREEN:
    doRUN_GREEN();
    decSYS_RUN -= 1;
    if (decSYS_RUN == 0)      state = SYS_RUN_INTERFACE;
    break;

  case SYS_WAIT:          doSYS_WAIT();         break;
  case SYS_SET_SOLL:      doSYS_SET_SOLL();     break;
  case SYS_SET_THRES:     doSYS_SET_THRES();    break;
  case SYS_CAL:           doSYS_CAL();          break;
  case CAL_PH4:           doCAL_PH4();          break;
  case CAL_PH7:           doCAL_PH7();          break;
  case CAL_CONF:          doCAL_CONF();         break;   
  case CAL_OK:            doCAL_OK();           break;   //eventuell delay(1500) donn state == SYS_WAIT
  }

}

//stateMaschine-doFnkBlock//////////////////////////////////
//wenn ph nan, bleiber im interface hängen...momentan hupfter auf GREEN, ober magari a eigerner SYS_RUN_ERROR war net schlecht
////////OOOOODER cal isch kaputt, schmeis olm NA--->erster sell checkn
void doSYS_RUN_INTERFACE()          
{
  phLast = phSonde.getPhIst();

  if      (phLast >= phSoll + phSollThres)    {    state = SYS_RUN_RED;     }
  else if (phLast >= phSoll)                  {    state = SYS_RUN_YELLOW;  }
  else if (phLast < phSoll)                   {    state = SYS_RUN_GREEN;   }
  //else                                        {    state = SYS_RUN_GREEN;   }         //error case
}

void doRUN_RED()                              {    digitalWrite(MOTORGATE, HIGH);   }
void doRUN_YELLOW()                           {    digitalWrite(MOTORGATE, LOW);    }
void doRUN_GREEN()                            {    digitalWrite(MOTORGATE, LOW);    }

void doSYS_WAIT()
{
  digitalWrite(MOTORGATE, LOW);
  phLast = phSonde.getPhIst();
}

void doSYS_SET_SOLL()   {}
void doSYS_SET_THRES()  {}

void doSYS_CAL(){}
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

void incSoll() { phSoll += 0.1; }
void decSoll() { phSoll -= 0.1; }
void incThres() { phSollThres += 0.1; }
void decThres() { phSollThres -= 0.1; }
