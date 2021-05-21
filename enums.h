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

//btnINIT====================Müssen do bleiben suscht siegi sie net in dor menumap
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5


#endif