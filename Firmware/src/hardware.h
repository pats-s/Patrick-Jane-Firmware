#ifndef  HARDWARE_H
#define  HARDWARE_H

#include "stdbool.h"    /*in order to use "bool" type*/

//List all IOs here:

uint8_t floor_up_button_pin(uint8_t floor);
uint8_t floor_down_button_pin(uint8_t floor);

///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif
