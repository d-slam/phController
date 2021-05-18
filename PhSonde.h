/*
  enums - Library for boooa
  Created by Mir diocane, November 66, 2021.
  Released into the public domain.
*/
#ifndef PhSonde_h
#define PhSonde_h

#define RX_PH A1

class PhSonde
{
public:
    float getPhIst()
    {
        int sampleBuffer[10];
        int temp = 0;
        unsigned long int avgVal = 0;

        for (int i = 0; i < 10; i++)
        {
            sampleBuffer[i] = analogRead(RX_PH);
            delay(10);
        }

        for (int i = 0; i < 9; i++)
        {
            for (int j = i + 1; j < 10; j++)
            {
                if (sampleBuffer[i] > sampleBuffer[j])
                {
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
        return phIst;
        
    }



private:

float volt = 0.0;

float phIst, phLast = 0.0;

float calDelta = 2.19;
float calOffset = 2.85;

int nSmooth = 20;
int incBuffer = 0;
float vecBuffer = 0;

};

#endif