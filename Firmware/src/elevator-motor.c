#include "main.h"
#include "elevator-motor.h"
#include "hardware.h"
#include "scheduler.h"


/*locla variables*/
static floor_t current_floor = FLOOR_GF;
static bool elevator_status = false;
static bool init_done = false;
static bool in_Use = false;
static floor_t target_floor = FLOOR_GF;

/*local prototypes*/
void elevator_init_task(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void check_for_inside(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void check_for_outside(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void update_bcd(void);
void check_for_req(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void set_segments(uint8_t base_pin, uint8_t segments);


bool init_elevator_tasks(void) {
    bool ret = true;
    ret &=  s_task_create(true, S_TASK_NORMAL_PRIORITY,1000,elevator_init_task,NULL,NULL);
    ret &=  s_task_create(true, S_TASK_NORMAL_PRIORITY,200,check_for_req,NULL,NULL);
    ret &=  s_task_create(true, S_TASK_NORMAL_PRIORITY,100,check_for_inside,NULL,NULL);
    ret &=  s_task_create(true, S_TASK_NORMAL_PRIORITY,100,check_for_outside,NULL,NULL);
    //ret &=  s_task_create(true, S_TASK_NORMAL_PRIORITY,150,update_bcd,NULL,NULL);

    return ret;
}


void elevator_init_task(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    if (init_done) return; // to execute it just once

    output_low(M_Speed_High);
    output_low(M_Speed_Low);
    output_low(M_Dir_1);
    output_low(M_Dir_2);

    set_segments(F_BCD_0, 0x00);
    output_low(Flr_LED_Up);
    output_high(Flr_LED_Md);
    output_low(Flr_LED_Dn);

    current_floor = FLOOR_GF;

    init_done = true;
}


void check_for_req(s_task_handle_t me, s_task_msg_t **msg, void *arg){
    elevator_status = (current_floor != target_floor);
    if(current_floor == target_floor){ //when we arrive to the target floor we stop the motor
        output_low(M_Dir_1);
        output_low(M_Dir_2);
        output_low(M_Speed_Low);
        output_low(Flr_LED_Dn);
        output_low(Flr_LED_Up);

        in_Use=false;
        return;
    }
    in_Use = true;
    if(current_floor < target_floor) // we need to go up
    {
        output_high(M_Dir_1);
        output_high(M_Speed_Low);
        output_high(Flr_LED_Up);
        current_floor++;

    }
    else{ // we need to go down
        output_high(Flr_LED_Dn);
        output_high(M_Dir_2);
        output_high(M_Speed_Low);
        current_floor--;
    }
    update_bcd(); //update the floor number
}

void check_for_outside(s_task_handle_t me, s_task_msg_t **msg, void *arg){
    if(in_Use == true) return;
    if(!input(Flr0_Btn_Up)){
       target_floor = FLOOR_GF;
    }
    if((!input(Flr1_Btn_Up)) || (!input(Flr1_Btn_Dn))){
        target_floor = FLOOR_F1;
    }
    if((!input(Flr2_Btn_Up)) || (!input(Flr2_Btn_Dn))){
        target_floor = FLOOR_F2;;
    }
    if(!input(Flr3_Btn_Dn)){
        target_floor = FLOOR_F3;
    }
}


void check_for_inside(s_task_handle_t me, s_task_msg_t **msg, void *arg){
     if(in_Use == true) return;
      if(!input(Cab_GF)){
       target_floor = FLOOR_GF;
    }
    if(!input(Cab_F1)){
        target_floor = FLOOR_F1;
    }
    if(!input(Cab_F2)){
        target_floor = FLOOR_F2;;
    }
    if(!input(Cab_F3)){
        target_floor = FLOOR_F3;
    }
}

void update_bcd(void){
    switch (current_floor)
    {
    case FLOOR_GF:
        set_segments(F_BCD_0,0x00);
        break;
    
    case FLOOR_F1:
        set_segments(F_BCD_0,0x01);
        break;
        case FLOOR_F2:
        set_segments(F_BCD_0,0x02);
        break;
        case FLOOR_F3:
        set_segments(F_BCD_0,0x03);
        break;
    }
}

void set_segments(uint8_t base_pin, uint8_t segments) {
    output_low(base_pin);           // Segment A
    output_low(base_pin + 1);       // Segment B
    output_low(base_pin + 2);       // Segment C
    output_low(base_pin + 3);       // Segment D

    if (segments & 0x01) output_high(base_pin);         
    if (segments & 0x02) output_high(base_pin + 1);     
    if (segments & 0x04) output_high(base_pin + 2);     
    if (segments & 0x08) output_high(base_pin + 3);     
}
