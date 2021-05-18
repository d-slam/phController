/*
  enums - Library for boooa
  Created by Mir diocane, November 66, 2021.
  Released into the public domain.
*/
#ifndef enums_h
#define enums_h


typedef enum stateSys_e
{
  SYS_RUN,
  SYS_WAIT,
  SYS_SET_SOLL,
  SYS_SET_THRES,
  SYS_CAL
} statesSys_t;


typedef enum statesCal_e
{
  CAL_START,
  CAL_PH4,
  CAL_PH7,
  CAL_CONF,
  CAL_OK
} statesCal_t;


typedef enum statesRun_e
{
  RUN_RED,
  RUN_YELLOW,
  RUN_GREEN
} statesRun_t;


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