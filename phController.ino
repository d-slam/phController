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

bool btnPrellFlag = false;

int incStateCheck = 0;

statesSys_t SYSstate = SYS_WAIT;
statesCal_t CALstate = CAL_START;
statesRun_t RUNstate = RUN_GREEN;

LCDScreen lcdScreen(&phLast, &phSoll, &phSollThres);

PhSonde phSonde;

//SETUP===============================================
void setup()
{
  pinMode(MOTORGATE, OUTPUT);
  Serial.begin(9600);
  lcdScreen.drawStartScreen();
}

//LOOP==========================================================
void loop()
{

  phLast = phSonde.getPhIst();

  //if (incBuffer >= nSmooth - 1 || (btnPrellFlag == false && adc_key_in != 1023))
  lcdScreen.redraw(SYSstate, CALstate, RUNstate);

  incStateCheck++;
  //if ((SYSstate == SYS_RUN || SYSstate == SYS_SET_SOLL || SYSstate == SYS_SET_THRES) && incStateCheck >= 50)
  checkState();

  checkButtons();
}
//MyMethodes==========================================================

void setMenu(int keyPressed)
{
  switch (SYSstate)
  {
  case SYS_RUN:
    if (keyPressed == btnRIGHT)
      SYSstate = SYS_WAIT;
    break;

  case SYS_WAIT:
    if (keyPressed == btnRIGHT)
      SYSstate = SYS_SET_SOLL;
    if (keyPressed == btnLEFT)
      SYSstate = SYS_RUN;
    break;

  case SYS_SET_SOLL:
    if (keyPressed == btnRIGHT)
      SYSstate = SYS_SET_THRES;
    if (keyPressed == btnLEFT)
      SYSstate = SYS_WAIT;
    if (keyPressed == btnUP)
      incFlag = true;
    if (keyPressed == btnDOWN)
      decFlag = true;
    break;

  case SYS_SET_THRES:
    if (keyPressed == btnRIGHT)
      SYSstate = SYS_CAL;
    if (keyPressed == btnLEFT)
      SYSstate = SYS_SET_SOLL;
    if (keyPressed == btnUP)
      incFlag = true;
    if (keyPressed == btnDOWN)
      decFlag = true;
    break;

  case SYS_CAL:
    switch (CALstate)
    {
    case CAL_START:
      if (keyPressed == btnLEFT)
        SYSstate = SYS_SET_THRES;
      if (keyPressed == btnSELECT)
        CALstate = CAL_PH4;
      break;

    case CAL_PH4:
      if (keyPressed == btnSELECT)
        CALstate = CAL_PH7;
      break;

    case CAL_PH7:
      if (keyPressed == btnSELECT)
        CALstate = CAL_CONF;
      break;

    case CAL_CONF:
      if (keyPressed == btnSELECT)
        CALstate = CAL_OK;
      if (keyPressed == btnLEFT)
        CALstate = CAL_START;
      break;

    case CAL_OK:
      CALstate = CAL_START;
      SYSstate = SYS_WAIT;
      break;
    }
    break;
  }
}
void checkButtons()
{
  lcd_key = read_LCD_buttons();
  switch (lcd_key)
  {
  case btnRIGHT:
    if (btnPrellFlag == false)
      break;
    btnPrellFlag = false;
    setMenu(lcd_key);
    break;
  case btnLEFT:
    if (btnPrellFlag == false)
      break;
    btnPrellFlag = false;
    setMenu(lcd_key);
    break;
  case btnSELECT:
    if (btnPrellFlag == false)
      break;
    btnPrellFlag = false;
    setMenu(lcd_key);
    break;
  case btnUP:
    if (btnPrellFlag == false)
      break;
    btnPrellFlag = false;
    setMenu(lcd_key);
    break;
  case btnDOWN:
    if (btnPrellFlag == false)
      break;
    btnPrellFlag = false;
    setMenu(lcd_key);
    break;
  case btnNONE:
    btnPrellFlag = true;
    break;
  }
}

void checkState()
{
  incStateCheck = 0;
  switch (SYSstate)
  {
  case SYS_RUN:
    if (phLast >= phSoll + phSollThres)
    {
      RUNstate = RUN_RED;
      digitalWrite(MOTORGATE, HIGH);
    }
    else if (phLast >= phSoll)
    {
      RUNstate = RUN_YELLOW;
      digitalWrite(MOTORGATE, LOW);
    }
    else if (phLast < phSoll)
    {
      RUNstate = RUN_GREEN;
      digitalWrite(MOTORGATE, LOW);
    }
    break;

  case SYS_WAIT:
    digitalWrite(MOTORGATE, LOW);
    break;

  case SYS_SET_SOLL:
    if (incFlag == true)
      incSoll();
    if (decFlag == true)
      decSoll();
    break;

  case SYS_SET_THRES:
    if (incFlag == true)
      incThres();
    if (decFlag == true)
      decThres();
    break;

  case SYS_CAL:

    switch (CALstate)
    {
    case CAL_START:
      break;

    case CAL_PH4:
      break;

    case CAL_PH7:
      phSonde.setVolt4(volt);
      break;

    case CAL_CONF:
      phSonde.setVolt7(volt);
      phSonde.calcDelta();
      break;

    case CAL_OK:
      phSonde.applyCallibration();
      break;
    }
    break;
  }
}
void incSoll()
{
  phSoll += 0.1;
  incFlag = false;
}
void decSoll()
{
  phSoll -= 0.1;
  decFlag = false;
}
void incThres()
{
  phSollThres += 0.1;
  incFlag = false;
}
void decThres()
{
  phSollThres -= 0.1;
  decFlag = false;
}
