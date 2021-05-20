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

bool incFlag = false;
bool decFlag = false;

bool btnFirstFlag = false;

LCDScreen lcdScreen(&phLast, &phSoll, &phSollThres);

PhSonde phSonde;

//ACHTUNG!STATMASCINE===============================================
state_t state = SYS_WAIT; //init State

unsigned char exitCheck(); // fragt die Induktionsschleife ab

unsigned char zaehler;

void stateMachine() //~~~♪callMe from main()
{
  switch (state)
  {

  case SYS_DUMMY: //void case, plotzholter!!
  {
    // // Ampel1(ROT);
    // // Ampel2(GRUEN);

    // lcdScreen.redraw(state);
    // phLast = phSonde.getPhIst();

    // if (exitCheck())
    //   state == SYS_RUN;

    break;
  }

  case SYS_RUN:
    digitalWrite(MOTORGATE, LOW);
    phLast = phSonde.getPhIst();
    checkForNewButtonPress();
    break;

  case SYS_WAIT:
    digitalWrite(MOTORGATE, LOW);
    phLast = phSonde.getPhIst();
    checkForNewButtonPress();

    zaehler = zaehler - 1;
    if (zaehler == 0)
      state = SYS_DUMMY;
    break;

  case SYS_SET_SOLL:
    if (incFlag == true)
      incSoll();
    if (decFlag == true)
      decSoll();

    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case SYS_SET_THRES:
    if (incFlag == true)
      incThres();
    if (decFlag == true)
      decThres();

    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case SYS_CAL:
    checkForNewButtonPress();
    zaehler = 4;
    state = SYS_DUMMY;
    break;

  case CAL_START:
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case CAL_PH4:
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case CAL_PH7:
    phSonde.setVolt4(volt);
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case CAL_CONF:
    phSonde.setVolt7(volt);
    phSonde.calcDelta();
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case CAL_OK:
    phSonde.applyCallibration();
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case RUN_RED:
    digitalWrite(MOTORGATE, HIGH);
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case RUN_YELLOW:
    digitalWrite(MOTORGATE, LOW);
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

  case RUN_GREEN:
    digitalWrite(MOTORGATE, LOW);
    checkForNewButtonPress();
    state = SYS_DUMMY;
    break;

    // if (phLast >= phSoll + phSollThres)
    // {
    //   RUNstate = RUN_RED;
    //   digitalWrite(MOTORGATE, HIGH);
    // }
    // else if (phLast >= phSoll)
    // {
    //   RUNstate = RUN_YELLOW;
    //   digitalWrite(MOTORGATE, LOW);
    // }
    // else if (phLast < phSoll)
    // {
    //   RUNstate = RUN_GREEN;
    //   digitalWrite(MOTORGATE, LOW);
    // }
  }

  lcdScreen.redraw(state);
}

//MyMethodes==========================================================
void checkForNewButtonPress()
{
  lcd_key = read_LCD_buttons();   //read BUTTON

  switch (lcd_key)                //check welcher BUTTON?
  {
  //wenn noBUTTON, [btnFirstFlagFALSE} u EXIT; wenn button PRESS [DO] u [btnFirstFlagFALSE]
  case btnNONE:    btnFirstFlag = false;    break;

  //wenn BUTTON, check ob schun aknowledged->donn break, SUSCHT resetFLAG u DOSHIT()
  case btnRIGHT:       if (btnFirstFlag == false)      break;
  case btnLEFT:    if (btnFirstFlag == false)      break;
  case btnSELECT:    if (btnFirstFlag == false)      break;
  case btnUP:    if (btnFirstFlag == false)      break;
  case btnDOWN:    if (btnFirstFlag == false)      break;
    btnFirstFlag = false; //resetFLAG
    setState(lcd_key);    //DOSHIT
    break;
  }

}

void setState(int keyPressed)
{
  switch (state)
  {
  case SYS_RUN:
    if (keyPressed == btnRIGHT)
      state = SYS_WAIT;
    break;

  case SYS_WAIT:
    if (keyPressed == btnRIGHT)
      state = SYS_SET_SOLL;
    if (keyPressed == btnLEFT)
      state = SYS_RUN;
    break;

  case SYS_SET_SOLL:
    if (keyPressed == btnRIGHT)
      state = SYS_SET_THRES;
    if (keyPressed == btnLEFT)
      state = SYS_WAIT;
    if (keyPressed == btnUP)
      incSoll();
    if (keyPressed == btnDOWN)
      decSoll();
    break;

  case SYS_SET_THRES:
    if (keyPressed == btnRIGHT)
      state = SYS_CAL;
    if (keyPressed == btnLEFT)
      state = SYS_SET_SOLL;
    if (keyPressed == btnUP)
      incThres();
    if (keyPressed == btnDOWN)
      decThres();
    break;

  case CAL_START:
    if (keyPressed == btnLEFT)
      state = SYS_SET_THRES;
    if (keyPressed == btnSELECT)
      state = CAL_PH4;
    break;

  case CAL_PH4:
    if (keyPressed == btnSELECT)
      state = CAL_PH7;
    break;

  case CAL_PH7:
    if (keyPressed == btnSELECT)
      state = CAL_CONF;
    break;

  case CAL_CONF:
    if (keyPressed == btnSELECT)
      state = CAL_OK;
    if (keyPressed == btnLEFT)
      state = CAL_START;
    break;

  case CAL_OK:
    state = SYS_WAIT;
    break;
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

  // checkForNewButtonPress();
}

void incSoll() { phSoll += 0.1; }
void decSoll() { phSoll -= 0.1; }
void incThres() { phSollThres += 0.1; }
void decThres() { phSollThres -= 0.1; }
