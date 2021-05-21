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
    InputButtons(void (*_pSwitchState)(int *)) : pSwitchState(_pSwitchState) {    }

private:
    void (*pSwitchState)(int *);
};



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