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
const uint8_t segment_map[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
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
    bool ret = false;
    ret &= s_task_create(true,S_TASK_NORMAL_PRIORITY,1000,display_temp,NULL,NULL);
    return ret;
}


void testing(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    // SEG1_A_OFF();
    // output_high(CT_D2_BCD0);
    // SEG1_B_ON();
    // output_high(CT_D2_BCD1);
    // SEG1_C_OFF();
    // output_high(CT_D2_BCD2);
    // SEG1_D_ON();
    // output_high(CT_D2_BCD3);
}

float read_temperature(void)
{
    uint16_t adc_value = get_adc_live(0); 
    float voltage = (adc_value * ADC_VREF) / ADC_MAX_LVL; 
    float temperature = voltage * 100.0; 
    return temperature;
}


void display_temp(s_task_handle_t me, s_task_msg_t **msg, void* arg) {
    // Read the temperature as a float and convert to an integer
    float temp = read_temperature();
    uint8_t temp_whole = (uint8_t)temp; // Extract integer part 


    uint8_t digit1 = temp_whole / 10; 
    uint8_t digit2 = temp_whole % 10; // to extract the first and second number

    // Display the digits and letters
    set_segments(CT_D1_BCD0, segment_map[digit1]); // Display the tens digit on the first display
    set_segments(CT_D2_BCD0, segment_map[digit2]); // Display the ones digit on the second display
    set_segments(CT_D3_BCD0, SEGMENT_D);           // Display "D" on the third display
    set_segments(CT_D4_BCD0, SEGMENT_C);           // Display "C" on the fourth display
}

void set_segments(uint8_t base_pin, uint8_t segments) {
    // Turn off all segments first
    output_low(base_pin);           // Segment A
    output_low(base_pin + 1);       // Segment B
    output_low(base_pin + 2);       // Segment C
    output_low(base_pin + 3);       // Segment D

    // Turn on the segments based on the bitmask
    if (segments & 0x01) output_high(base_pin);         // Segment A
    if (segments & 0x02) output_high(base_pin + 1);     // Segment B
    if (segments & 0x04) output_high(base_pin + 2);     // Segment C
    if (segments & 0x08) output_high(base_pin + 3);     // Segment D
    // Add similar logic for other segments (E, F, G, DP)
}