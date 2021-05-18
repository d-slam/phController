/*
  enums - Library for boooa
  Created by Mir diocane, November 66, 2021.
  Released into the public domain.
*/
#ifndef enums_h
#define enums_h


typedef enum 
{
  SYS_RUN,
  SYS_WAIT,
  SYS_SET_SOLL,
  SYS_SET_THRES,
  SYS_CAL
} statesSystem;


typedef enum
{
  CAL_START,
  CAL_PH4,
  CAL_PH7,
  CAL_CONF,
  CAL_OK
} statesCal;


typedef enum
{
  RUN_RED,
  RUN_YELLOW,
  RUN_GREEN
} statesRun;


statesSystem SYSstate = SYS_WAIT;
statesCal CALstate = CAL_START;
statesRun RUNstate = RUN_GREEN;


#endif