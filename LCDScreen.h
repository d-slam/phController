/*
  LCDScreen - Library for dassi eppes sig do
  Created by Mir diocane, November 66, 2021.
  Released into the public domain.
*/
#ifndef LCDScreen_h
#define LCDScreen_h

#include "Arduino.h"

#include <LiquidCrystal.h>

//LCD_INIT============================================
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

class LCDScreen
{
public:
  LCDScreen() { lcd.begin(16, 2); }

  void redrawLCD()
  {
    lcd.clear();
    drawLeftScreen();

    switch (SYSstate)
    {
    case SYS_RUN:

      writeFloatAtXY(phSoll, 8, 1);
      writeAtXY("+", 12, 1);
      writeFloatAtXY(phSollThres, 13, 1);

      switch (RUNstate)
      {
      case RUN_RED:
        writeAtXY("Pumpt", 8, 0);
        break;

      case RUN_YELLOW:
        writeAtXY("In Range", 8, 0);
        break;

      case RUN_GREEN:
        writeAtXY("Halte", 8, 0);
        break;
      }
      break;

    case SYS_WAIT:
      writeAtXY("Wartet...", 8, 0);
      writeAtXY("  <-Go<<", 8, 1);
      break;

    case SYS_SET_SOLL:
      writeAtXY("set-> Soll", 6, 0);
      writeFloatAtXY(phSoll, 8, 1);
      break;

    case SYS_SET_THRES:
      writeAtXY("set-> Thres", 6, 0);
      writeFloatAtXY(phSollThres, 8, 1);
      break;

    case SYS_CAL:
      switch (CALstate)
      {
      case CAL_START:
        lcd.clear();
        writeAtXY("Start Cal?", 6, 0);
        writeAtXY("[SELECT]", 8, 1);
        break;

      case CAL_PH4:
        drawLeftScreen();
        writeAtXY("Cal", 8, 0);
        writeAtXY("-> Ph4", 8, 1);
        break;

      case CAL_PH7:
        drawLeftScreen();
        writeAtXY("Cal", 8, 0);
        writeAtXY("-> Ph7", 8, 1);
        break;

      case CAL_CONF:
        lcd.clear();
        writeAtXY("CANCEL?   apply?", 0, 0);
        writeAtXY("[LEFT]  [SELECT]", 0, 1);
        break;

      case CAL_OK:
        lcd.clear();
        writeAtXY("Callibration", 0, 0);
        writeAtXY("Complette!", 6, 1);

        //delay(1500);
        break;
      }
      break;
    }
  }

  void writeAtXY(const char *string, uint8_t x, uint8_t y)
  {
    lcd.setCursor(x, y);
    lcd.print(string);
  }

  void writeFloatAtXY(float value, uint8_t x, uint8_t y)
  {
    lcd.setCursor(x, y);
    lcd.print(value);
  }

  void drawSplashscreen(const char *stringA, uint8_t xA, uint8_t yA, const char *stringB, uint8_t xB, uint8_t yB, int tDelay)
  {
    lcd.clear();
    lcd.setCursor(xA, yA);
    lcd.print(stringA);
    lcd.setCursor(xB, yB);
    lcd.print(stringB);
    delay(tDelay);
  }

  void drawLeftScreen()
  {
    writeAtXY("Ph Ist", 0, 0);
    writeFloatAtXY(phLast, 0, 1);
  }

private:
};

//btnINIT============================================
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
int read_LCD_buttons()
{
  adc_key_in = analogRead(0); // read the value from the sensor
  if (adc_key_in > 1000)
    return btnNONE;
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 250)
    return btnUP;
  if (adc_key_in < 450)
    return btnDOWN;
  if (adc_key_in < 650)
    return btnLEFT;
  if (adc_key_in < 850)
    return btnSELECT;
  return btnNONE; // when all others fail, return this...
}

#endif