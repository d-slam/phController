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

int decRUNState = 0;        //wieviel durchläufe bis phSoll<=>phIst check
int decRefreshLCD = 0;      //wieviel cycles worten bis lcdRefresh

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
  decRUNState = 10;     
  phLast = inputPhSonde.getPhIstSmooth();  

  if      (phLast >= phSoll + phSollThres)    {    state = RUN_RED;     }
  else if (phLast >= phSoll)                  {    state = RUN_YELLOW;  }
  else if (phLast < phSoll)                   {    state = RUN_GREEN;   }
  else                                        {    state = RUN_ERROR;   }         //error case  
  
  outputLCDScreen.redraw(state);

}
void doRUN_RED()
{
  digitalWrite(MOTORGATE, HIGH);
  inputButtons.checkForNewButtonPress();  
  decRUNState -= 1;
  if (decRUNState == 0)
    state = SYS_INT_RUN;
}
void doRUN_YELLOW()
{
  digitalWrite(MOTORGATE, LOW);
  inputButtons.checkForNewButtonPress(); 
  decRUNState -= 1;
  if (decRUNState == 0)
    state = SYS_INT_RUN;
}
void doRUN_GREEN()
{
  digitalWrite(MOTORGATE, LOW);
  inputButtons.checkForNewButtonPress(); 
  decRUNState -= 1;
  if (decRUNState == 0)
    state = SYS_INT_RUN;
}
void doRUN_ERROR()
{
  digitalWrite(MOTORGATE, LOW);
  inputButtons.checkForNewButtonPress();      

}
void doSYS_WAIT()
{
  digitalWrite(MOTORGATE, LOW);
  phLast = inputPhSonde.getPhIstSmooth();
  outputLCDScreen.redraw(state);

  inputButtons.checkForNewButtonPress();      

}
void doSYS_SET_SOLL() 
{
  inputButtons.checkForNewButtonPress();  
  outputLCDScreen.redraw(state);

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
  inputButtons.checkForNewButtonPress();  
  outputLCDScreen.redraw(state);

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
  inputButtons.checkForNewButtonPress();    
  outputLCDScreen.redraw(state);
}
void doCAL_PH4()
{
  phLast = inputPhSonde.getPhIstSmooth();
  outputLCDScreen.redraw(state);
  inputButtons.checkForNewButtonPress();   
}
void doCAL_PH7()
{
  phLast = inputPhSonde.getPhIstSmooth();
  inputPhSonde.setVolt4();
  outputLCDScreen.redraw(state);
  inputButtons.checkForNewButtonPress();   
}
void doCAL_CONF()
{
  outputLCDScreen.redraw(state);
  inputPhSonde.setVolt7();
  inputPhSonde.calcDelta();
  inputButtons.checkForNewButtonPress();      

}
void doCAL_OK()
{
  outputLCDScreen.redraw(state);
  inputPhSonde.applyCallibration();
  inputButtons.checkForNewButtonPress();      

      //wait, donn back...
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
