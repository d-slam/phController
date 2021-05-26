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

  SYS_INT_RUN,            //run interface
  RUN_RED,
  RUN_YELLOW,
  RUN_GREEN,
  RUN_ERROR,

  SYS_WAIT,

  SYS_SET_SOLL,
  INC_SET_SOLL,
  DEC_SET_SOLL,
  
  SYS_SET_THRES,
  INC_SET_THRES,
  DEC_SET_THRES,

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

//sensorDEFS====================
#define RX_PH A1

//aktorDEFS====================
#define MOTORGATE 2


#endif