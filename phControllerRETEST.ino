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

//ACHTUNGSTATMASCINE===============================================

state_t state = ALLE_ROT_1;
unsigned char zaehler;

void stateMachine() //~~~callMe from main()
{
  switch (state)
  {
  case OSTWEST_GRUEN:
    Ampel1(ROT);
    Ampel2(GRUEN);
    if (abfragee())
    {
      state = OSTWEST_GELB;
    }
    break;

  case OSTWEST_GELB:
    Ampel1(ROT);
    Ampel2(GELB);
    state = ALLES_ROT_1;
    break;

  case ALLES_ROT_1:
    Ampel1(ROT);
    Ampel2(ROT);
    state = NORDSUED_ROTGELB;
    break;

  case NORDSUED_ROTGELB:
    Ampel1(ROT_GELB);
    Ampel2(ROT);
    state = NORDSUED_GRUEN;
    break;

  case NORDSUED_GRUEN:
    Ampel1(GRUEN);
    Ampel2(ROT);
    zaehler = 4;
    state = WARTE_NORDSUED;
    break;

  case NORDSUED_GELB:
    Ampel1(GELB);
    Ampel2(ROT);
    state = ALLES_ROT_2;
    break;

  case ALLES_ROT_2:
    Ampel1(ROT);
    Ampel2(ROT);
    state = OSTWEST_ROTGELB;
    break;

  case OSTWEST_ROTGELB:
    Ampel1(ROT);
    Ampel2(ROT_GELB);
    state = OSTWEST_GRUEN;
    break;

  case WARTE_NORDSUED:
    zaehler = zaehler - 1;
    if (zaehler == 0)
      state = NORDSUED_GELB;
    break;
  }
}




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

  lcdScreen.redraw(SYSstate, CALstate, RUNstate);

  checkState();

  checkButtons();
}
//MyMethodes==========================================================


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
