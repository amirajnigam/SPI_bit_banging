#include "SPI.h"

void SetGPIOdirection(unsigned char port, unsigned char mask)
{
//Black box API
}

unsigned char GetGPIOportValues(unsigned char port)
{
//Black box API
}

void SetGPIOportValues(unsigned char port, unsigned char mask)
{
//Black box API
}

unsigned long GetPeripheralTime(void)
{
//Black box API
}


void delay__ms(float number_of_millisec)
{
    unsigned long micro_sec = 1000 * number_of_millisec;

    unsigned long start_time = GetPeripheralTime();

    while(GetPeripheralTime() < start_time + micro_sec){
        ;
    }
}