/*
  enums - Library for emus, defines and ol the bad mofos..
  Created by Mir. Dio. della cane, November 66, A. D. 2021 ACorona
  Released into the public domain.
*/
#ifndef enums_h
#define enums_h

////REALSHIZ
typedef enum
{
  SYS_RUN_INTERFACE,
  SYS_RUN_RED,
  SYS_RUN_YELLOW,
  SYS_RUN_GREEN, 
  SYS_WAIT,
  SYS_SET_SOLL,
  SYS_SET_THRES,
  SYS_CAL,
  CAL_PH4,
  CAL_PH7,
  CAL_CONF,
  CAL_OK,
  VOID_DUMMY
} state_t;


//btnINIT============================================

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
int lcd_key = 0;
int adc_key_in = 0;
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