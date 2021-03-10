#include <stdio.h>
#include <stdint.h>

/***************************************************
*mask represent Input = 0, Output = 1 configuration
* of all pins of the given port
****************************************************/
void SetGPIOdirection(unsigned char port, unsigned char mask);

/****************************************************
*returns the current state of all pinss of a given port 
*****************************************************/
unsigned char GetGPIOportValues(unsigned char port);

/******************************************************************
*Set the state of all pins of the given port that are configured
*as GPIO output and the bits for pins not configured as GPIO output
*must be written as zero  
*******************************************************************/

void SetGPIOportValues(unsigned char port, unsigned char mask);

/*****************************************************************
 * returns a running counter of microseconds since system start up
 * ***************************************************************/
unsigned long GetPeripheralTime(void);

void delay__ms(int number_of_millisec);