#ifndef DS1307_H
#define DS1307_H

typedef struct {
    uint8_t minutes; // Minutes value (0–59)
    uint8_t hours;   // Hours value (0–23 or 0–12 depending on the format)
    uint8_t dof;
    uint8_t day;
    uint8_t month;
} ds1307_time_t;


#define DS1307_SLAVE_ADRS_W   (0x68 << 1)    // DS1307 I2C Write Address
#define DS1307_SLAVE_ADRS_R   (DS1307_SLAVE_ADRS_W | 0x01) // DS1307 I2C Read Address


#define DS1307_REG_SECONDS  (0x00) // Seconds Register
#define DS1307_REG_MINUTES  (0x01) // Minutes Register
#define DS1307_REG_HOURS    (0x02) // Hours Register
#define DS1307_REG_DAY      (0x03) // Day Register
#define DS1307_REG_DATE     (0x04)// Date Register
#define DS1307_REG_MONTH    (0x05)// Month Register
#define DS1307_REG_YEAR     (0x06)// Year Register


bool ds1307_read_register(uint8_t reg_address, uint8_t *data);
bool ds1307_read_buffer(uint8_t start_address, uint8_t *buffer, uint8_t size);
void display_time();
void display_date();


#endif
