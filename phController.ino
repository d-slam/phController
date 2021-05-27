///phController////////><(((°>//////><(((°>////////><(((°>/////
/*
  phController - Main 
  Created by D. -Tomätebauer- Ami, 21 Mæi, A. D. 2021 ACorona
  Released into the Wild.
*/
//////////////////////////////////////////////////////////////
#include "enums.h"

//VERY!GLOBALS===============================================
#include "OutputLCDScreen.h"
#include "InputButtons.h"
#include "InputPhSonde.h"

//GLOBALS===============================================
float phIst = 0.0;
float phLast = 0.0;
float phSoll = 5.5;
float phSollThres = 0.5;

state_t state = SYS_WAIT;     //init State

int nRunState = 0;        //wieviel durchläufe bis phSoll<=>phIst check

//MODUES===============================================
OutputLCDScreen outputLCDScreen(&phLast, &phSoll, &phSollThres);

InputPhSonde inputPhSonde;
InputButtons inputButtons(&switchState);  

//SETUP===============================================
void setup()
{
  Serial.begin(9600);
  Serial.println("Serial hüü!");

  outputLCDScreen.drawStartScreen();

  pinMode(MOTORGATE, OUTPUT);

}

//LOOP==========================================================
void loop()
{
  executeState(state);

  delay(20);
}






//ACHTUNG!STATMASCINE===============================================
void executeState(state_t s)
{  
 switch (state)  {

                case SYS_INT_RUN:     doSYS_INT_RUN();     break;
                case RUN_RED:         doRUN_RED();         break;
                case RUN_YELLOW:      doRUN_YELLOW();      break;
                case RUN_GREEN:       doRUN_GREEN();       break;
                case RUN_ERROR:       doRUN_ERROR();       break;
                case SYS_WAIT:        doSYS_WAIT();        break;
                case SYS_SET_SOLL:    doSYS_SET_SOLL();    break;
                case INC_SET_SOLL:    doINC_SET_SOLL();    break;
                case DEC_SET_SOLL:    doDEC_SET_SOLL();    break;
                case SYS_SET_THRES:   doSYS_SET_THRES();   break;
                case INC_SET_THRES:   doINC_SET_THRES();   break;
                case DEC_SET_THRES:   doDEC_SET_THRES();   break;
                case SYS_CAL:         doSYS_CAL();         break;
                case CAL_PH4:         doCAL_PH4();         break;
                case CAL_PH7:         doCAL_PH7();         break;
                case CAL_CONF:        doCAL_CONF();        break;
                case CAL_OK:          doCAL_OK();          break; //eventuell delay(1500) donn state == SYS_WAIT 
  }
}

void doSYS_INT_RUN()          
{
  nRunState = 10;
  if      (phLast >= phSoll + phSollThres)    {    state = RUN_RED;     }
  else if (phLast >= phSoll)                  {    state = RUN_YELLOW;  }
  else if (phLast < phSoll)                   {    state = RUN_GREEN;   }
  else                                        {    state = RUN_ERROR;   } 

  reIO(RE_PH_LCD);    //erst check donn reIO, suscht sigsch nix  
}
void doRUN_RED()
{
  digitalWrite(MOTORGATE, HIGH);
  reIO(RE_BTN);
  decRUNState();
}
void doRUN_YELLOW()
{
  digitalWrite(MOTORGATE, LOW);
  reIO(RE_BTN);
  decRUNState();
}
void doRUN_GREEN()
{
  digitalWrite(MOTORGATE, LOW);
  reIO(RE_BTN);
  decRUNState();
}
void doRUN_ERROR()
{
  digitalWrite(MOTORGATE, LOW);
  reIO(RE_BTN);
}
void doSYS_WAIT()
{
  digitalWrite(MOTORGATE, LOW);
  reIO(RE_PH_LCD_BTN);
}
void doSYS_SET_SOLL() 
{
  reIO(RE_LCD_BTN);
}
void doINC_SET_SOLL()
{
  phSoll += 0.1;
  state = SYS_SET_SOLL;
}
void doDEC_SET_SOLL()
{
  phSoll -= 0.1;
  state = SYS_SET_SOLL;
}
void doSYS_SET_THRES()
{
  reIO(RE_LCD_BTN);
}
void doINC_SET_THRES()
{
  phSollThres += 0.1;
  state = SYS_SET_THRES;
}
void doDEC_SET_THRES()
{
  phSollThres -= 0.1;
  state = SYS_SET_THRES;
}
void doSYS_CAL()
{
  reIO(RE_LCD_BTN);   
}
void doCAL_PH4()
{
  reIO(RE_PH_LCD_BTN);  
}
void doCAL_PH7()
{
  inputPhSonde.setVolt4();
  reIO(RE_PH_LCD_BTN); 
}
void doCAL_CONF()
{
  inputPhSonde.setVolt7();
  inputPhSonde.calcDelta();
  reIO(RE_LCD_BTN); 
}
void doCAL_OK()
{
  reIO(RE_LCD);
  inputPhSonde.applyCallibration();
  delay(1500);
  state = SYS_WAIT;
}
//Helper///
void decRUNState()
{
  nRunState -= 1;
  if (nRunState == 0)
    state = SYS_INT_RUN;
}

void reIO(int r)
{
  switch(r)
  {
      case RE_BTN: reBtn();     break;
      case RE_LCD:  reLcd();    break;
      case RE_LCD_BTN:  reLcdBtn();    break;
      case RE_PH_LCD:  rePhLcd();    break;
      case RE_PH_LCD_BTN: rePhLcdBtn();     break;
  }
}

void reBtn()
{
  inputButtons.checkForNewButtonPress();
}

void reLcd()
{
  outputLCDScreen.redraw(state);
}

void reLcdBtn()
{
  inputButtons.checkForNewButtonPress();  
  outputLCDScreen.redraw(state);
}

void rePhLcd()
{
  phLast = inputPhSonde.getPhIstSmooth();
  outputLCDScreen.redraw(state);
}

void rePhLcdBtn()
{
  phLast = inputPhSonde.getPhIstSmooth();
  outputLCDScreen.redraw(state);
  inputButtons.checkForNewButtonPress(); 
}


//MENUMAP====================callback von dor inputButtons===========================
bool switchState(int* pButton)                
{
  switch (state)  {
  case SYS_INT_RUN:           if (*pButton == btnRIGHT)      state = SYS_WAIT;              break;
  case RUN_RED:               if (*pButton == btnRIGHT)      state = SYS_WAIT;              break;
  case RUN_YELLOW:            if (*pButton == btnRIGHT)      state = SYS_WAIT;              break;
  case RUN_GREEN:             if (*pButton == btnRIGHT)      state = SYS_WAIT;              break;
  case RUN_ERROR:             if (*pButton == btnRIGHT)      state = SYS_WAIT;              break;
  case SYS_WAIT:
                              if (*pButton == btnRIGHT)      state = SYS_SET_SOLL;
                              if (*pButton == btnLEFT)       state = SYS_INT_RUN;
                                                                                           break;
  case SYS_SET_SOLL:
                              if (*pButton == btnRIGHT)      state = SYS_SET_THRES;
                              if (*pButton == btnLEFT)       state = SYS_WAIT;
                              if (*pButton == btnUP)         state = INC_SET_SOLL;
                              if (*pButton == btnDOWN)       state = DEC_SET_SOLL;
                                                                                           break;
  case SYS_SET_THRES:
                              if (*pButton == btnRIGHT)      state = SYS_CAL;
                              if (*pButton == btnLEFT)       state = SYS_SET_SOLL;
                              if (*pButton == btnUP)         state = INC_SET_THRES;
                              if (*pButton == btnDOWN)       state = DEC_SET_THRES;
                                                                                           break;
  case SYS_CAL:
                              if (*pButton == btnLEFT)       state = SYS_SET_THRES;
                              if (*pButton == btnSELECT)     state = CAL_PH4;
                                                                                           break;
  case CAL_PH4:
                              if (*pButton == btnSELECT)     state = CAL_PH7;
                              if (*pButton == btnLEFT)       state = SYS_CAL;
                                                                                           break;
  case CAL_PH7:
                              if (*pButton == btnSELECT)     state = CAL_CONF;
                              if (*pButton == btnLEFT)       state = SYS_CAL;
                                                                                           break;
  case CAL_CONF:
                              if (*pButton == btnSELECT)     state = CAL_OK;
                              if (*pButton == btnLEFT)       state = SYS_CAL;
                                                                                            break;
  case CAL_OK:                                               state = SYS_WAIT;              break;
  }
  outputLCDScreen.redraw(state);
  return false;
}
