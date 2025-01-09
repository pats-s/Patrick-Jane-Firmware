/**
 * @file tempr.c
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "main.h"
#include "tempr.h"
#include "adc.h"
#include "scheduler.h"
#include "hardware.h"

const uint8_t segment_map[10] = {
    0x00, 
    0x01, 
    0x02, 
    0x03, 
    0x04, 
    0x05, 
    0x06, 
    0x07, 
    0x08, 
    0x09  
};


bool init_temp_sensor(void);
void set_segments(uint8_t base_pin, uint8_t segments);
void testing(s_task_handle_t me, s_task_msg_t **msg, void* arg);

bool init_temp_sensor(void)
{
    bool ret = true;
    //ret &= s_task_create(true,S_TASK_NORMAL_PRIORITY,1000,display_temp,NULL,NULL);
    return ret;
}



float read_temperature1(void)
{
    // Get the ADC value for the channel 
    uint16_t adc_value = get_adc(ADC_TEMPR); 
    
    // Calculate the temperature in Celsius (human readale not raw)
    float temperature = (float)adc_value * (MAX_TEMPERATURE / (ADC_MAX_LVL - 1));
    
    return temperature;
}


void display_temp() {
    // Read the temperature as a float and convert to an integer
    float temp = read_temperature1();
    if (temp > 99 ) temp=99;
    uint8_t temp_whole = (uint8_t)temp; // conv to int


    uint8_t digit1 = temp_whole / 10; 
    uint8_t digit2 = temp_whole % 10; 

 

    // Display the digits and letters
    set_segments(CT_D1_BCD0, segment_map[digit1]); 
    set_segments(CT_D2_BCD0, segment_map[digit2]);
    set_segments(CT_D3_BCD0, 0xD);           
    set_segments(CT_D4_BCD0, 0xC);           
}

void set_segments(uint8_t base_pin, uint8_t segments) {
    // Turn off all segments first
    output_low(base_pin);           // Segment A
    output_low(base_pin + 1);       // Segment B
    output_low(base_pin + 2);       // Segment C
    output_low(base_pin + 3);       // Segment D

    // Turn on the segments based on the bitmask
    if (segments & 0x01) output_high(base_pin);         
    if (segments & 0x02) output_high(base_pin + 1);     
    if (segments & 0x04) output_high(base_pin + 2);     
    if (segments & 0x08) output_high(base_pin + 3);     
}
