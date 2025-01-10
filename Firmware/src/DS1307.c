#include "main.h"
#include "hardware.h"
#include "scheduler.h"
#include "DS1307.h"

#define _AND_ACK    (1)
#define _AND_NACK   (0)
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



uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

//this function to read from the registers of the RTC nad put them in the buffer
bool ds1307_read_registers(uint8_t internal_address, uint8_t* buffer, uint8_t size) {
    uint8_t ack_state = 0;

    if ((size != 0) && (buffer != NULL)) {
        i2c_start(); // Send START condition
        ack_state |= i2c_write((DS1307_SLAVE_ADRS_W));
        ack_state |= i2c_write(internal_address);     
        i2c_start();                                  
        ack_state |= i2c_write(DS1307_SLAVE_ADRS_R);  

        for (uint8_t i = 0; i < size - 1; i++) {
            buffer[i] = i2c_read(_AND_ACK); 
        }
        buffer[size - 1] = i2c_read(_AND_NACK);
        i2c_stop(); 
        return (ack_state == 0) ? true : false;
    }
    return false;
}

//this function to get the values from the buffer and put thme in a struct
ds1307_time_t ds1307_read_time(void) {
    uint8_t buffer[5];
    ds1307_time_t time;

    //read from the registers
    if (ds1307_read_registers(DS1307_REG_MINUTES, buffer, 5)) {
        time.minutes = bcd_to_bin(buffer[0]);      
        time.hours = bcd_to_bin(buffer[1] & 0x3F); 
        time.dof = bcd_to_bin(buffer[2]);
        time.day = bcd_to_bin(buffer[3]);         
        time.month = bcd_to_bin(buffer[4]);    
            
    } else {
        //in case of any error
        time.minutes = 0xFF;
        time.hours = 0xFF;
         time.day = 0xFF;
        time.month = 0xFF;
       
    }

    return time;
}
void set_segments(uint8_t base_pin, uint8_t segments) {
    // Turn off all segments first
    output_low(base_pin);           
    output_low(base_pin + 1);       
    output_low(base_pin + 2);       
    output_low(base_pin + 3);       

    //bitmask op to turn on the desired segment
    if (segments & 0x01) output_high(base_pin);         
    if (segments & 0x02) output_high(base_pin + 1);     
    if (segments & 0x04) output_high(base_pin + 2);     
    if (segments & 0x08) output_high(base_pin + 3);     
}
void display_time() {
    ds1307_time_t time = ds1307_read_time();

    if (time.hours == 0xFF || time.minutes == 0xFF) {
        set_segments(CT_D1_BCD0, 0xE); //in case of error display the letter e
        set_segments(CT_D2_BCD0, 0xE); //in case of error display the letter e
        return;
    }

    uint8_t hour_tens = time.hours / 10;  
    uint8_t hour_units = time.hours % 10; 
    uint8_t min_tens = time.minutes / 10; 
    uint8_t min_units = time.minutes % 10;

    set_segments(CT_D1_BCD0, segment_map[hour_tens]);  
    set_segments(CT_D2_BCD0, segment_map[hour_units]); 
    COL_ON();
    set_segments(CT_D3_BCD0, segment_map[min_tens]);   
    set_segments(CT_D4_BCD0, segment_map[min_units]);  
}


void display_date() {
    COL_OFF();
    ds1307_time_t time = ds1307_read_time();

    if (time.day == 0xFF || time.month == 0xFF) {
        set_segments(CT_D1_BCD0, 0xE); //in case of error display the letter e
        set_segments(CT_D2_BCD0, 0xE); //in case of error display the letter e
        return;
    }

    uint8_t day_tens = time.day / 10;  
    uint8_t day_units = time.day % 10; 
    uint8_t month_tens = time.month / 10; 
    uint8_t month_units = time.month % 10;

    set_segments(CT_D1_BCD0, segment_map[day_tens]);   
    set_segments(CT_D2_BCD0, segment_map[day_units]);  

    set_segments(CT_D3_BCD0, segment_map[month_tens]); 
    set_segments(CT_D4_BCD0, segment_map[month_units]);
}
