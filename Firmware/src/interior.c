#include "main.h"
#include "hardware.h"
#include "scheduler.h"
#include "interior.h"
#include "DS1307.h"
#include "tempr.h"
#include "DS1307.h"
void display(s_task_handle_t me, s_task_msg_t **msg, void* arg);
void display_floor(s_task_handle_t me, s_task_msg_t **msg, void* arg);
void set_segments(uint8_t base_pin, uint8_t segments);


bool init_interior(void)
{
    bool ret;
    ret &= s_task_create(1,S_TASK_NORMAL_PRIORITY,5000,display,NULL,NULL);
    ret &= s_task_create(1,S_TASK_NORMAL_PRIORITY,1000,display_floor,NULL,NULL);
    return ret;
}


void display_floor(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    set_segments(F_BCD_0,0x02);
}


void display(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    static uint8_t turn = 0;
    switch (turn)
    {
    case 0: // temp
        display_temp();
        break;

    case 1: // date
        display_time();
        break;
    
    case 2: // time
    display_date();
    break;
    }
    turn = (turn % 3) + 1 ;
    
    
}
