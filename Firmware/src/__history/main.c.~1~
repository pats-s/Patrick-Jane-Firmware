#include "main.h"
#include "hardware.h"
#include "system.h"
#include "scheduler.h"
#include "timer.h"
#include "adc.h"
#include "tempr.h"

void main()
{
    bool ret;
    
    delay_ms(100);                  /*power up delay, wait a 100ms*/

    /*Initialize scheduler:*/
    ret = scheduler_init(get_ticks_counter);
    
    if (true == ret)
    {
        SEG1_A_OFF();
        SEG1_B_ON();
        SEG1_C_ON();
        SEG1_D_OFF();
        ledon();
        /*Initialize system:*/
        ret &= init_hw();           /*initialize HW and create LED heartbeat task*/
        ret &= init_system();       /*initialize system and enable interrupt(s) (events)*/

        ret &= init_adc();  
        ret &= init_temp_sensor();
        /*initialize ADC sampling*/
        /*. . . . . . .*/

        if (true == ret)            /*success?*/
        {
            while (true)
            {
                scheduler();        /*run scheduler forever*/
            }
        }
        else
        {
            while (true);           /*stall*/
        }
    }
    else
    {
        while (true);               /*stall*/
    }
}
