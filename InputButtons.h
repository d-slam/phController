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
    InputButtons()
    {
        pBtnPressed = &bufferKeypad;
        // pCallback = &callback;
    }

    void checkForNewButtonPress()                 
    {
        switch (bufferKeypad = getBtnIndex())
        {
        case btnNONE:               btnAcptNewInput = true;                    break;

        case btnRIGHT:            if (btnAcptNewInput == false)                break;
            btnAcptNewInput = inputButtonsCallback(pBtnPressed);
            break;

        case btnLEFT:            if (btnAcptNewInput == false)                   break;
            btnAcptNewInput = inputButtonsCallback(pBtnPressed);
            break;

        case btnSELECT:            if (btnAcptNewInput == false)                 break;
            btnAcptNewInput = inputButtonsCallback(pBtnPressed);
            break;

        case btnUP:            if (btnAcptNewInput == false)                     break;
            btnAcptNewInput = inputButtonsCallback(pBtnPressed);
            break;

        case btnDOWN:            if (btnAcptNewInput == false)                    break;
            btnAcptNewInput = inputButtonsCallback(pBtnPressed);
            break;
        }
    }

private:
    int *pBtnPressed;            //Pointer auf keyValue, werd dor switchState geben fürn switchCheck

    // bool (*pCallback) (int* );

    int keyAnalog = 0;
    int bufferKeypad = 0;

    bool btnAcptNewInput = false;

    int getBtnIndex()
    {
        keyAnalog = analogRead(0); // read the value from the sensor
        if (keyAnalog > 1000)              return btnNONE;
        if (keyAnalog < 50)                return btnRIGHT;
        if (keyAnalog < 250)               return btnUP;
        if (keyAnalog < 450)               return btnDOWN;
        if (keyAnalog < 650)               return btnLEFT;
        if (keyAnalog < 850)               return btnSELECT;
                                            return btnNONE; // when all others fail, return this...
    }
};




#endif