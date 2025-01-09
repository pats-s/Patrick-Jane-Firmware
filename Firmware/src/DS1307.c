#include "DS1307.h"
#include "hardware.h"
#include "scheduler.h"
#include "main.h"

#define _AND_ACK    (1)
#define _AND_NACK   (0)

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
    uint8_t buffer[4];
    ds1307_time_t time;

    //read from the registers
    if (ds1307_read_registers(DS1307_REG_MINUTES, buffer, 4)) {
        time.minutes = bcd_to_bin(buffer[0]);      
        time.hours = bcd_to_bin(buffer[1] & 0x3F); 
        time.date = bcd_to_bin(buffer[2]);         
        time.month = bcd_to_bin(buffer[3]);        
    } else {
        //in case of any error
        time.minutes = 0xFF;
        time.hours = 0xFF;
        time.date = 0xFF;
        time.month = 0xFF;
    }

    return time;
}
