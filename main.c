#include "project.h"
#include <stdint.h>

#define TABLE_SIZE 100

uint8 sineTable[TABLE_SIZE];
uint8 triangleTable[TABLE_SIZE];

volatile uint8 waveIndex = 0;
uint8 waveType = 0;

uint16 adcFreq = 0;
uint16 adcAmp = 0;

uint8 ampValue = 128;

// ISR
CY_ISR(Wave_ISR)
{
    uint16 value;

    if(waveType == 0)
        value = sineTable[waveIndex];
    else if(waveType == 1)
        value = (waveIndex < TABLE_SIZE/2) ? 255 : 0;
    else
        value = triangleTable[waveIndex];

    value = (value * ampValue) / 255;

    VDAC8_1_SetValue((uint8)value);

    waveIndex++;
    if(waveIndex >= TABLE_SIZE)
        waveIndex = 0;
}

// LUT
void generateTables()
{
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        float x = (float)i / TABLE_SIZE;
        float s = 4 * x * (1 - x);

        sineTable[i] = (uint8)(127 + 127 * s);

        if(i < TABLE_SIZE/2)
            triangleTable[i] = (uint8)((255 * i) / (TABLE_SIZE/2));
        else
            triangleTable[i] = (uint8)(255 - ((255 * (i - TABLE_SIZE/2)) / (TABLE_SIZE/2)));
    }
}

int main(void)
{
    CyGlobalIntEnable;

    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();

    AMux_1_Start();

    VDAC8_1_Start();

    Timer_1_Start();
    isr_1_StartEx(Wave_ISR);

    generateTables();

    uint8 ch = 0;

    while(1)
    {
       
        uint16 val = ADC_SAR_1_GetResult16();

        if(ch == 0)
        {
            adcFreq = val;
            AMux_1_Select(1);
            ch = 1;
        }
        else
        {
            adcAmp = val;
            AMux_1_Select(0);
            ch = 0;
        }

        // Frequency control (smooth)
        uint16 period = 250 + (3000 - adcFreq)/25;
        Timer_1_WritePeriod(period);

        // Amplitude
        ampValue = adcAmp >> 4;

        // Button
        if(button_Read() == 0)
        {
            CyDelay(200);

            waveType++;
            if(waveType > 2)
                waveType = 0;

            while(button_Read() == 0);
        }

        CyDelay(1);
    }
}
