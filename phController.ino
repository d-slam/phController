#include <LiquidCrystal.h>

//LCD_INIT============================================
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int read_LCD_buttons() {
  adc_key_in = analogRead(0);      // read the value from the sensor
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;  // when all others fail, return this... 
}

//GLOBALS===============================================
#define RX_PH     A1
#define MOTORGATE 2

typedef enum {SYS_RUN,  SYS_WAIT,  SYS_SET_SOLL, SYS_SET_THRES,  SYS_CAL} statesSystem;
statesSystem SYSstate = SYS_WAIT;

typedef enum {CAL_START, CAL_PH4,  CAL_PH7,  CAL_CONF, CAL_OK} statesCal;
statesCal CALstate = CAL_START;

typedef enum {RUN_RED, RUN_YELLOW, RUN_GREEN} statesRun;
statesRun RUNstate = RUN_GREEN;

bool incFlag = false;
bool decFlag = false;

bool btnPrellFlag = false;

float calDelta = 2.19;
float calOffset = 2.85;

float volt = 0.0;

float phSoll = 5.5;
float phSollThres = 0.5;
float phIst, phLast = 0.0;

int nSmooth = 20;
int incBuffer = 0;
float vecBuffer = 0;

int incStateCheck = 0;

//bool noPrell = true;

//SETUP===============================================
void setup()
{
  pinMode(MOTORGATE, OUTPUT);

  Serial.begin(9600);
  Serial.println("Hüü");

  lcd.begin(16, 2);              // <== WICHTIG!!
  lcd.setCursor(0, 0);
  lcd.print("ph-Meter");
  lcd.setCursor(0, 1);
  lcd.print("><(((°>");
  delay(1500);
  lcd.clear();

  redrawLCD();
}

//LOOP==========================================================
void loop()
{
  bufferPh();

  //if (incBuffer >= nSmooth - 1 || (noPrell == false && adc_key_in != 1023) )
  redrawLCD();

  incStateCheck++;
  if (SYSstate == SYS_RUN && incStateCheck >= 50)
    checkState();

  checkButtons();
}
//MyMethodes==========================================================
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
          case RUN_RED: {
              lcd.setCursor(8, 0);
              lcd.print("Pumpt   ");
              break;
            }
          case RUN_YELLOW: {
              lcd.setCursor(8, 0);
              lcd.print("In Range");
              break;
            }
          case RUN_GREEN: {
              lcd.setCursor(8, 0);
              lcd.print("Halte");
              break;
            }
        }
        break;
      }
    case SYS_WAIT:
      {
        lcd.setCursor(8, 1);
        lcd.print("<-Lauf<<");
        lcd.setCursor(8, 0);
        lcd.print("Warten..");
        break;
      }
    case SYS_SET_SOLL:
      {
        lcd.setCursor(8, 1);
        lcd.print(phSoll);
        lcd.setCursor(8, 0);
        lcd.print("Set Soll");
        break;
      }
    case SYS_CAL:
      {
        switch (CALstate)
        {
          case CAL_START:
            {
              lcd.setCursor(8, 1);
              lcd.print("Start?");
              lcd.setCursor(8, 0);
              lcd.print("pH Cal");
              break;
            }

          case CAL_PH4:
            {
              lcd.setCursor(8, 1);
              lcd.print("Set?");
              lcd.setCursor(8, 0);
              lcd.print("ph 4");
              break;
            }

          case CAL_PH7:
            {
              lcd.setCursor(8, 1);
              lcd.print("Set?");
              lcd.setCursor(8, 0);
              lcd.print("ph 7");
              break;
            }

          case CAL_CONF:
            {
              lcd.setCursor(8, 1);
              lcd.print("Save?");
              lcd.setCursor(8, 0);
              lcd.print("Enter New Cal");
              break;
            }

          case CAL_OK:
            {
              lcd.setCursor(8, 1);
              lcd.print("Save?");
              lcd.setCursor(8, 0);
              lcd.print("okokl");
              break;
            }
        }
        break;
      }
  }
}

void checkButtons()
{
  lcd_key = read_LCD_buttons();
  switch (lcd_key)
  {
    case btnRIGHT:
      if (btnPrellFlag == false) break;
      btnPrellFlag = false;
      setMenu(lcd_key);
      break;
    case btnLEFT:
      if (btnPrellFlag == false) break;
      btnPrellFlag = false;
      setMenu(lcd_key);
      break;
    case btnSELECT:
      if (btnPrellFlag == false) break;
      btnPrellFlag = false;
      setMenu(lcd_key);
      break;
    case btnUP:
      if (btnPrellFlag == false) break;
      btnPrellFlag = false;
      setMenu(lcd_key);
      break;
    case btnDOWN:
      if (btnPrellFlag == false) break;
      btnPrellFlag = false;
      setMenu(lcd_key);
      break;
    case btnNONE:      btnPrellFlag = true;      break;
  }
}
void setMenu(int keyPressed)
{
  switch (SYSstate)
  {
    case SYS_RUN :     
     if (keyPressed == btnRIGHT)      
       SYSstate = SYS_WAIT;    
         break;

    case SYS_WAIT :
      if (keyPressed == btnRIGHT)        SYSstate = SYS_SET_SOLL;
      if (keyPressed == btnLEFT)        SYSstate = SYS_RUN;
      break;

    case SYS_SET_SOLL:
      if (keyPressed == btnRIGHT)        SYSstate = SYS_SET_THRES;
      if (keyPressed == btnLEFT)        SYSstate = SYS_WAIT;
      if (keyPressed == btnUP)        incFlag = true;
      if (keyPressed == btnDOWN)        decFlag = true;
      break;

    case SYS_SET_THRES:
      if (keyPressed == btnRIGHT)        SYSstate = SYS_CAL;
      if (keyPressed == btnLEFT)        SYSstate = SYS_SET_SOLL;
      if (keyPressed == btnUP)        incFlag = true;
      if (keyPressed == btnDOWN)        decFlag = true;
      break;

    case SYS_CAL:
      switch (CALstate)
      {
        case CAL_START:
          if (keyPressed == btnLEFT)        SYSstate = SYS_SET_THRES;
          if (keyPressed == btnSELECT)        CALstate = CAL_PH4;
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
          if (keyPressed == btnSELECT)        CALstate = CAL_OK;
          if (keyPressed == btnLEFT)          CALstate = CAL_START;
          break;
          
        case CAL_OK:
          CALstate = CAL_PH4;
          SYSstate = SYS_WAIT;
          break;
      }
      break;
  }
}

void checkState()
{
  incStateCheck = 0;

  switch (SYSstate)
  {
    case SYS_RUN :
      break;

    case SYS_WAIT :
      break;

    case SYS_SET_SOLL:
      if (incFlag == true)        incSoll();
      if (decFlag == true)        decSoll();
      break;

    case SYS_SET_THRES:
      if (incFlag == true)        incThres();
      if (decFlag == true)        decThres();
      break;

    case SYS_CAL:
      switch (CALstate)
      {
        case CAL_START:         break;
        case CAL_PH4:          break;
        case CAL_PH7:          break;
        case CAL_CONF:          break;
        case CAL_OK:          break;
      }
      break;
  }

  //  switch (systemState)  {
  //    case sRun:        {
  //        if (phLast >= phSoll + phSollOffset) {
  //          runState = sRed;
  //          digitalWrite(MOTORGATE, HIGH);
  //        }
  //        else if (phLast >= phSoll) {
  //          runState = sYellow;
  //          digitalWrite(MOTORGATE, LOW);
  //        }
  //        else if (phLast < phSoll) {
  //          runState = sGreen;
  //          digitalWrite(MOTORGATE, LOW);
  //        } break;
  //      }
  //    case sWait:      {
  //        digitalWrite(MOTORGATE, LOW);
  //        break;
  //      }
  //    case sSet:      {
  //        digitalWrite(MOTORGATE, LOW);
  //        break;
  //      }
  //    case sCal:      {
  //        digitalWrite(MOTORGATE, LOW);
  //
  //        float tempCalDelta = 0.0;
  //        float tempCalOffset = 0.0;
  //        float volt7 = 0.0;
  //        float volt4 = 0.0;
  //
  //        switch (calState) {
  //
  //          case cal7: {
  //              volt7 = volt;
  //              break;
  //            }
  //
  //          case cal4: {
  //              volt4 = volt;
  //              tempCalDelta = (7 - 4) / (volt7 - volt4);
  //              tempCalOffset = 4 - ( tempCalDelta * volt4);
  //
  //              break;
  //            }
  //
  //          case calSet: {
  //
  //
  //              break;
  //            }
  //
  //
  //            break;
  //        }
  //
  //        break;
  //      }
  //    default: digitalWrite(MOTORGATE, LOW);
  //  }
}


void incSoll()
{
  phSoll += 0.1;
  incFlag = false;
}
void decSoll()
{
  phSoll += 0.1;
  decFlag = false;
}
void incThres()
{
  phSollThres += 0.1;
  incFlag = false;
}
void decThres()
{
  phSollThres += 0.1;
  decFlag = false;
}


void bufferPh()
{
  int sampleBuffer[10];
  int temp = 0;
  unsigned long int avgVal = 0;

  for (int i = 0; i < 10; i++)  {
    sampleBuffer[i] = analogRead(RX_PH);
    delay(10);
  }

  for (int i = 0; i < 9; i++)                 {
    for (int j = i + 1; j < 10; j++)          {
      if (sampleBuffer[i] > sampleBuffer[j])  {
        temp = sampleBuffer[i];
        sampleBuffer[i] = sampleBuffer[j];
        sampleBuffer[j] = temp;
      }
    }
  }
  for (int i = 2; i < 8; i++)
    avgVal += sampleBuffer[i];

  volt = ((float)avgVal * 5.0 / 1024 / 6);

  phIst = calDelta * volt + calOffset;

  vecBuffer += phIst;
  incBuffer++;
  if (incBuffer >= nSmooth)
  {
    phLast = vecBuffer / nSmooth;
    incBuffer = 0;
    vecBuffer = 0;
  }
}

//to clear the LCD display, use the comment below
//lcd.clear();
