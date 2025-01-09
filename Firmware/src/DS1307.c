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
void display_time() {
    // Read the time from the DS1307
    ds1307_time_t time = ds1307_read_time();

    // Check for invalid time values (0xFF indicates an error)
    if (time.hours == 0xFF || time.minutes == 0xFF) {
        // Display error on the 7-segment (e.g., show "EE" for error)
        set_segments(CT_D1_BCD0, 0xE); // Display 'E' on the first display
        set_segments(CT_D2_BCD0, 0xE); // Display 'E' on the second display
        return;
    }

    // Extract digits for hours and minutes
    uint8_t hour_tens = time.hours / 10;  // Tens digit of hours
    uint8_t hour_units = time.hours % 10; // Units digit of hours
    uint8_t min_tens = time.minutes / 10; // Tens digit of minutes
    uint8_t min_units = time.minutes % 10; // Units digit of minutes

    // Display the time on the 7-segment displays
    set_segments(CT_D1_BCD0, segment_map[hour_tens]);  // Display hour tens on the first display
    set_segments(CT_D2_BCD0, segment_map[hour_units]); // Display hour units on the second display
    COL_ON();
    set_segments(CT_D3_BCD0, segment_map[min_tens]);   // Display minute tens on the third display
    set_segments(CT_D4_BCD0, segment_map[min_units]);  // Display minute units on the fourth display
}


void display_date() {
    COL_OFF();
    // Read the date and month from the DS1307
    ds1307_time_t time = ds1307_read_time();

    // Check for invalid date values (0xFF indicates an error)
    if (time.day == 0xFF || time.month == 0xFF) {
        // Display error on the 7-segment (e.g., show "EE" for error)
        set_segments(CT_D1_BCD0, 0xE); // Display 'E' on the first display
        set_segments(CT_D2_BCD0, 0xE); // Display 'E' on the second display
        return;
    }

    // Extract digits for date and month
    uint8_t day_tens = time.day / 10;  // Tens digit of day
    uint8_t day_units = time.day % 10; // Units digit of day
    uint8_t month_tens = time.month / 10; // Tens digit of month
    uint8_t month_units = time.month % 10; // Units digit of month

    // Display the date on the 7-segment displays
    set_segments(CT_D1_BCD0, segment_map[day_tens]);   // Display day tens on the first display
    set_segments(CT_D2_BCD0, segment_map[day_units]);  // Display day units on the second display

    set_segments(CT_D3_BCD0, segment_map[month_tens]); // Display month tens on the third display
    set_segments(CT_D4_BCD0, segment_map[month_units]); // Display month units on the fourth display
}
