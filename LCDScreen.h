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
  LCDScreen()  {    lcd.begin(16, 2);  }

  void drawSplashscreen()
  {
    lcd.setCursor(0, 0);
    lcd.print("ph-Meter");
    lcd.setCursor(0, 1);
    lcd.print("><(((°>");
    delay(1500);
    lcd.clear();
  }

  void redrawLCD()
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ph Ist");
    lcd.setCursor(0, 1);
    lcd.print(phLast);

    switch (SYSstate)
    {
    case SYS_RUN:
    {
      lcd.setCursor(8, 1);
      lcd.print(phSoll);
      lcd.setCursor(12, 1);
      lcd.print("+0.5");

      switch (RUNstate)
      {
      case RUN_RED:
        lcd.setCursor(8, 0);
        lcd.print("Pumpt   ");
        break;

      case RUN_YELLOW:
        lcd.setCursor(8, 0);
        lcd.print("In Range");
        break;

      case RUN_GREEN:
        lcd.setCursor(8, 0);
        lcd.print("Halte");
        break;
      }
      break;
    }
    case SYS_WAIT:
      lcd.setCursor(8, 1);
      lcd.print("<-Lauf<<");
      lcd.setCursor(8, 0);
      lcd.print("Warten..");
      break;

    case SYS_SET_SOLL:
      lcd.setCursor(8, 1);
      lcd.print(phSoll);
      lcd.setCursor(8, 0);
      lcd.print("Set Soll");
      break;

    case SYS_CAL:
    {
      switch (CALstate)
      {
      case CAL_START:
        lcd.setCursor(8, 1);
        lcd.print("Start?");
        lcd.setCursor(8, 0);
        lcd.print("pH Cal");
        break;

      case CAL_PH4:
        lcd.setCursor(8, 1);
        lcd.print("Set?");
        lcd.setCursor(8, 0);
        lcd.print("ph 4");
        break;

      case CAL_PH7:
        lcd.setCursor(8, 1);
        lcd.print("Set?");
        lcd.setCursor(8, 0);
        lcd.print("ph 7");
        break;

      case CAL_CONF:
        lcd.setCursor(8, 1);
        lcd.print("Save?");
        lcd.setCursor(8, 0);
        lcd.print("Enter New Cal");
        break;

      case CAL_OK:
        lcd.setCursor(8, 1);
        lcd.print("Save?");
        lcd.setCursor(8, 0);
        lcd.print("okokl");
        break;
      }
      break;
    }
    }
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