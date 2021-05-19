/*
  LCDScreen - Library for dassi eppes sig do
  Created by Mir diocane, November 66, 2021.
  Released into the public domain.
*/
#ifndef LCDScreen_h
#define LCDScreen_h

// #include "Arduino.h" //lei für strings, honni ober im mom koane

#include <LiquidCrystal.h>

//LCD_INIT============================================
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

class LCDScreen
{
public:
  LCDScreen(float *_phLast, float *_phSoll, float *_phSollThres)
      : pPhLast(_phLast), pPhSoll(_phSoll), pPhSollThres(_phSollThres)
  {
    lcd.begin(16, 2);
  }

  void redraw(const statesSys_t SYSstate, const statesCal_t CALstate, const statesRun_t RUNstate)
  {
    lcd.clear();
    drawLeftScreen();
    switch (SYSstate)
    {
    case SYS_RUN:
      switch (RUNstate)
      {
      case RUN_RED:
        drawRUN_RED();
        break;
      case RUN_YELLOW:
        drawRUN_YELLOW();
        break;
      case RUN_GREEN:
        drawRUN_GREEN();
        break;
      }
      break;
    case SYS_WAIT:
      drawSYS_WAIT();
      break;
    case SYS_SET_SOLL:
      drawSYS_SET_SOLL();
      break;
    case SYS_SET_THRES:
      drawSYS_SET_THRES();
      break;
    case SYS_CAL:
      switch (CALstate)
      {
      case CAL_START:
        drawCAL_START();
        break;
      case CAL_PH4:
        drawCAL_PH4();
        break;
      case CAL_PH7:
        drawCAL_PH7();
        break;
      case CAL_CONF:
        drawCAL_CONF();
        break;
      case CAL_OK:
        drawCAL_OK();
        break;
      }
      break;
    }
  }

  void drawStartScreen() { drawSplashscreen("ph-Meter", 0, 0, "><(((°>", 0, 1, 1500); }

private:
  void drawRUN_RED()
  {
    writeAtXY("Pumpt", 8, 0);
    drawSollAndThres();
  }
  void drawRUN_YELLOW()
  {
    writeAtXY("In Range", 8, 0);
    drawSollAndThres();
  }
  void drawRUN_GREEN()
  {
    writeAtXY(
        "Halte", 8, 0);
    drawSollAndThres();
  }
  void drawSollAndThres()
  {
    writeFloatAtXY(*pPhSoll, 8, 1);
    writeAtXY("+", 12, 1);
    writeFloatAtXY(*pPhSollThres, 13, 1);
  }
  void drawSYS_WAIT()
  {
    drawLeftScreen();
    writeAtXY("Wartet...", 8, 0);
    writeAtXY("  <-Go<<", 8, 1);
  }
  void drawSYS_SET_SOLL()
  {
    lcd.clear();
    writeAtXY("->Ph Soll-Wert", 2, 0);
    writeFloatAtXY(*pPhSoll, 8, 1);
  }
  void drawSYS_SET_THRES()
  {
    lcd.clear();
    writeAtXY("->Ph Threshold", 2, 0);
    writeAtXY("+", 12, 1);
    writeFloatAtXY(*pPhSollThres, 13, 1);
  }
  void drawCAL_START()
  {
    lcd.clear();
    writeAtXY("Start Cal?", 6, 0);
    writeAtXY("[SELECT]", 8, 1);
  }
  void drawCAL_PH4()
  {
    drawLeftScreen();
    writeAtXY("Cal", 8, 0);
    writeAtXY("-> Ph4", 8, 1);
  }
  void drawCAL_PH7()
  {
    drawLeftScreen();
    writeAtXY("Cal", 8, 0);
    writeAtXY("-> Ph7", 8, 1);
  }
  void drawCAL_CONF()
  {
    lcd.clear();
    writeAtXY("cancel?   apply?", 0, 0);
    writeAtXY("[LEFT]  [SELECT]", 0, 1);
  }
  void drawCAL_OK()
  {
    lcd.clear();
    writeAtXY("Callibration", 0, 0);
    writeAtXY("Complette!", 6, 1);
    //delay(1500);
  }
  void drawSplashscreen(const char *cA, uint8_t xA, uint8_t yA, const char *cB, uint8_t xB, uint8_t yB, int tDelay)
  {
    lcd.clear();
    lcd.setCursor(xA, yA);
    lcd.print(cA);
    lcd.setCursor(xB, yB);
    lcd.print(cB);
    delay(tDelay);
  }
  void drawLeftScreen()
  {
    writeAtXY("Ph Ist", 0, 0);
    writeFloatAtXY(*pPhLast, 0, 1);
  }
  void writeAtXY(const char *c, uint8_t x, uint8_t y)
  {
    lcd.setCursor(x, y);
    lcd.print(c);
  }
  void writeFloatAtXY(float value, uint8_t x, uint8_t y)
  {
    lcd.setCursor(x, y);
    lcd.print(value);
  }
  float *pPhLast, *pPhSoll, *pPhSollThres;
};
#endif