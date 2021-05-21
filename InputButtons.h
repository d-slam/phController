/*
  InputButtons - Library for Button Inputs from the lcd
  Created by Mir. Dio. della cane, November 1212, A. D. 2021 ACorona
  Released into the public domain.
*/
#ifndef InputButtons_h
#define InputButtons_h

class InputButtons
{
public:
    InputButtons(void (*_pSwitchState)(int *)) : pSwitchState(_pSwitchState) 
    {
        
    }

    bool btnNewInputAllowedFlag = false;

    bool aknIncFlag()
    {
        btnNewIncAllowedFlag = false;
        return true;
    }
    bool aknDecFlag()
    {
        btnNewDecAllowedFlag = false;
        return true;
    }

//START von dor mascihine....wenn net geprellt, gebmor in keypointer weiter an SWITCHSTATE
//seint so zu lafn...
    void checkForNewButtonPress()           
    {
        int bufferKeypad = read_LCD_buttons();
        pBtnPressed = &bufferKeypad;
        switch (bufferKeypad)                  
        {
        case btnNONE:                   btnNewInputAllowedFlag = true;                  break;

        case btnRIGHT:              if (btnNewInputAllowedFlag == false)                break;
            switchState(pBtnPressed);
            // pSwitchState(pBtnPressed);          // kp wieso i gmoant hon i brauch des, bleib erst mol do
            btnNewInputAllowedFlag = false;
            break;

        case btnLEFT:                if (btnNewInputAllowedFlag == false)               break;
            switchState(pBtnPressed);
            btnNewInputAllowedFlag = false;
            break;

        case btnSELECT:             if (btnNewInputAllowedFlag == false)                break;
            switchState(pBtnPressed);
            btnNewInputAllowedFlag = false;
            break;

        case btnUP:                 if (btnNewInputAllowedFlag == false)                break;
            btnNewIncAllowedFlag = true;
            switchState(pBtnPressed);
            btnNewInputAllowedFlag = false;
            break;

        case btnDOWN:               if (btnNewInputAllowedFlag == false)                break;
            btnNewDecAllowedFlag = true;
            switchState(pBtnPressed);
            btnNewInputAllowedFlag = false;
            break;
        }
    }

private:
    int lcd_key = 0;
    int adc_key_in = 0;

    void (*pSwitchState)(int *); //Pointer auf die SwitchState


    int *pBtnPressed;            //Pointer auf keyValue

    bool btnNewIncAllowedFlag = false;
    bool btnNewDecAllowedFlag = false;

    int read_LCD_buttons()
    {
        adc_key_in = analogRead(0); // read the value from the sensor
        if (adc_key_in > 1000)              return btnNONE;
        if (adc_key_in < 50)                return btnRIGHT;
        if (adc_key_in < 250)               return btnUP;
        if (adc_key_in < 450)               return btnDOWN;
        if (adc_key_in < 650)               return btnLEFT;
        if (adc_key_in < 850)               return btnSELECT;
                                            return btnNONE; // when all others fail, return this...
    }
};

#endif