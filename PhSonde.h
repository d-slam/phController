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
        sampleVoltage();
        phIst = calDelta * volt + calOffset;
        return phIst;
    }

    void smoothPh()
    {
        int nSmooth = 20;
        vecBuffer += phIst;
        incBuffer++;
        if (incBuffer >= nSmooth)
        {
            phLast = vecBuffer / nSmooth;
            newValFlag = true;
            incBuffer = 0;
            vecBuffer = 0;
        }
    }

    void setVolt4(float v)
    {
        volt4 = v;
    }
    void setVolt7(float v)
    {
        volt7 = v;
    }
    void calcDelta()
    {
        tempCalDelta = (7 - 4) / (volt7 - volt4);
        tempCalOffset = 4 - (tempCalDelta * volt4);
    }
    void applyCallibration()
    {
        calDelta = tempCalDelta;
        calOffset = tempCalOffset;
    }

private:
    float tempCalDelta = 2.19;
    float tempCalOffset = 2.85;

    float volt7 = 0.0;
    float volt4 = 0.0;

    bool newValFlag = false;

    float volt = 0.0;

    float phIst, phLast = 0.0;

    float calDelta = 2.19;
    float calOffset = 2.85;

    int incBuffer = 0;
    float vecBuffer = 0;

    void sampleVoltage()
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
    }
};

#endif