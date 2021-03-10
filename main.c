#pragma once
/*********************************
 * 
 * Pin Configuration & Direction
 * 
 * MOSI = PA.3, Output
 * MISO = PA.4, Input
 * SCK = PA.1, Output
 * CS = PB.6, Output
 * 
 *********************************/
#include "SPI.h"

#define REG_NUM_7 7

unsigned char PORT_A;
unsigned char PORT_B;


#define MOSI_pin (1<<3)
#define MISO_pin (1<<4)
#define SCK_pin (1<<1)
#define CS_pin (1<<6)

/************************************************
* target device communicates when CS pin is low
*************************************************/
void target_cs(void){
    PORT_B &= ~CS_pin;
}

/*************************************************************
* target device communication deactivate when  CS pin is high
**************************************************************/
void target_ds(void){
    SetGPIOportValues(PORT_B, CS_pin);
}

/***************************************************************************************************
* Set all pins of PORT_A and PORT_B as input and output with mask and SetGPIOdirection() function
***************************************************************************************************/
void set_pin_direction(){
    const unsigned char mask_port_A = 0x8F;
    const unsigned char mask_port_B = 0xE4;
    SetGPIOdirection(PORT_A, mask_port_A);
    SetGPIOdirection(PORT_B, mask_port_B);
}

void clock_rising_edge(void){
    if(!(GetGPIOportValues(PORT_A) & SCK_pin))
        SetGPIOportValues(PORT_A, SCK_pin);       
}

void clock_falling_edge(void){
    if(GetGPIOportValues(PORT_A) & SCK_pin)
        PORT_A &= ~SCK_pin;
}

void master_send_slave_recieve(uint8_t send_data){
    for(int i = 7 ; i >= 0; i--){
        clock_rising_edge();
        if(send_data & 0x80){
            delay__ms(0.0025);                   //Maintaining 400 kHZ of slave and master clock frequency for sampling the data
            SetGPIOportValues(PORT_A, MOSI_pin);
        }
        else{
            delay__ms(0.0025);
            SetGPIOportValues(PORT_A, MOSI_pin);
        }
        send_data <<= 1;  
        clock_falling_edge();
    }
}

uint16_t slave_send_master_recieve(void){
    uint16_t get_values = 0; 
    for(int i = 15; i >=0; i--){    
        delay__ms(0.0025);
        clock_rising_edge();
        if(GetGPIOportValues(PORT_A) & MISO_pin){
            get_values |= 1<<i;
        }       
        clock_falling_edge();
    }
    return get_values;
}

uint16_t update_PORT_A_and_PORT_B(uint16_t get_values) {
    uint8_t received_data_LSB;
    uint8_t received_data_MSB;
    
    uint8_t button_data = (get_values & 0x3F);

    for(int i = 0; i < 6; i++){
        uint8_t button__status = button_data & 0x01;

        switch(i){
            case '0': if(button__status)
                        SetGPIOportValues(PORT_A, 0x01);
                      else
                        PORT_A &= ~ 0x01;
            break;
            case '1': if(button__status)
                        SetGPIOportValues(PORT_A, 0x04);
                      else
                        PORT_A &= ~ 0x04;
            break;                        
            case '2': if(button__status)
                        SetGPIOportValues(PORT_A, 0x80);
                      else
                        PORT_A &= ~ 0x80;
            break;
            case '3': if(button__status)
                        SetGPIOportValues(PORT_B, 0x04);
                      else
                        PORT_B &= ~ 0x04;
            break;           
            case '4': if(button__status)
                        SetGPIOportValues(PORT_B, 0x20);
                      else
                        PORT_B &= ~ 0x20;
            break;
            case '5': if(button__status)
                        SetGPIOportValues(PORT_B, 0x80);
                      else
                        PORT_B &= ~ 0x80;
            break;
            default: break;
        }

        button_data >>= 1;
    }

    received_data_LSB = GetGPIOportValues(PORT_A);
    received_data_MSB = GetGPIOportValues(PORT_B);
        
    return ((received_data_MSB<<8 ) |  received_data_LSB);
}

uint16_t spi_exchange(uint8_t send_data){

    master_send_slave_recieve(send_data);
    
    delay__ms(1);

    uint16_t get_values = slave_send_master_recieve();

    return update_PORT_A_and_PORT_B(get_values);
}


uint8_t get_button_status(uint16_t slave_data, uint16_t mask, uint16_t pos){
    
    slave_data &= mask;
    uint16_t x = (1 << pos);

    if((slave_data & x) != 0)
        return 1;
    else
        return 0;
}

void update_digital_output(void){

    uint8_t send_data = REG_NUM_7;
    uint16_t slave_data;
    
    while(1){
        target_cs();
        slave_data = spi_exchange(send_data);
        target_ds();

        printf("Button 1 status:%d\n", get_button_status(slave_data, 0x0001, 0));
        printf("Button 2 status:%d\n", get_button_status(slave_data, 0x0004, 2));
        printf("Button 3 status:%d\n", get_button_status(slave_data, 0x0080, 7));
        printf("Button 4 status:%d\n", get_button_status(slave_data, 0x0400, 10));
        printf("Button 5 status:%d\n", get_button_status(slave_data, 0x0200, 13));
        printf("Button 6 status:%d\n", get_button_status(slave_data, 0x8000, 15));
    }
}

void clear_port_values(void){    
    unsigned char clear_bit = 0xFF;
    PORT_A &= ~clear_bit;
    PORT_B &= ~clear_bit;
}

int main(void)
{
    clear_port_values();

    set_pin_direction();

    update_digital_output();

    return 0;

}