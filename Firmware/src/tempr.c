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
// Segment map for digits 0-9
// const uint8_t segment_map[10] = {
//     0000, // 0
//     0001, // 1
//     0010, // 2
//     0011, // 3
//     0100, // 4
//     0101, // 5
//     0110, // 6
//     0111, // 7
//     1000, // 8
//     1001  // 9
// };
const uint8_t segment_map[10] = {
    0x00, // 0
    0x01, // 1
    0x02, // 2
    0x03, // 3
    0x04, // 4
    0x05, // 5
    0x06, // 6
    0x07, // 7
    0x08, // 8
    0x09  // 9
};

// Segment map for letters "D" and "C"
#define SEGMENT_D 0b01011110
#define SEGMENT_C 0b00111001


bool init_temp_sensor(void);
void display_temp(s_task_handle_t me, s_task_msg_t **msg, void* arg);
void set_segments(uint8_t base_pin, uint8_t segments);
void testing(s_task_handle_t me, s_task_msg_t **msg, void* arg);

bool init_temp_sensor(void)
{
    bool ret = true;
    ret &= s_task_create(true,S_TASK_NORMAL_PRIORITY,1000,display_temp,NULL,NULL);
    return ret;
}


void testing(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   output_high(CT_D1_BCD0);
   COL_ON();

}

// float read_temperature(void)
// {
//     uint16_t adc_value = get_adc_live(0); 
//     float voltage = (adc_value * ADC_VREF) / ADC_MAX_LVL; 
//     float temperature = voltage * 100.0; 
//     return temperature;
// }
float read_temperature1(void)
{
    // Get the ADC value for the channel (e.g., ADC_TEMPR)
    uint16_t adc_value = get_adc(ADC_TEMPR); 
    
    // Calculate the temperature in Celsius
    float temperature = (float)adc_value * (MAX_TEMPERATURE / (ADC_MAX_LVL - 1));
    
    return temperature;
}


void display_temp(s_task_handle_t me, s_task_msg_t **msg, void* arg) {
    // Read the temperature as a float and convert to an integer
    float temp = read_temperature1();
    if (temp > 99 ) temp=99;
    uint8_t temp_whole = (uint8_t)temp; // Extract integer part 


    uint8_t digit1 = temp_whole / 10; 
    uint8_t digit2 = temp_whole % 10; // to extract the first and second number

 

    // Display the digits and letters
    set_segments(CT_D1_BCD0, segment_map[digit1]); // Display the tens digit on the first display
    set_segments(CT_D2_BCD0, segment_map[digit2]); // Display the ones digit on the second display
    set_segments(CT_D3_BCD0, 0xD);           // Display "D" on the third display
    set_segments(CT_D4_BCD0, 0xC);           // Display "C" on the fourth display
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
