#include "main.h"
#include "hardware.h"
#include "scheduler.h"

/*local prototypes:*/
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/**
 * @brief Function to setup all IOs
 * 
 */
void init_io_ports(void) {
    // Configure port A as inputs
    set_tris_a(0xFF);
    output_a(0x00);

    // Configure port B as inputs for buttons
    set_tris_b(0xFF);
    output_b(0x00);

    // Configure port C as needed (modify based on unused pins)
    set_tris_c(0xFF);
    output_c(0x00);

    // Configure port D for motor control outputs
    set_tris_d(0x00);
    output_d(0x00);

    // Configure port E for additional control signals or displays
    set_tris_e(0x00);
    output_e(0x00);

    // Configure port F for LED outputs
    set_tris_f(0x00);
    output_f(0x00);

    // Configure port G for unused or future connections
    set_tris_g(0xFF);
    output_g(0x00);
}

bool init_hw(void) {
    init_io_ports();

    // Timer1: used for scheduler
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Configure for 1ms interrupt

    return true;
}

uint8_t floor_up_button_pin(uint8_t floor) {
    switch (floor) {
        case 0: return PIN_B0; // Up button for floor 0
        case 1: return PIN_B2; // Up button for floor 1
        case 2: return PIN_B4; // Up button for floor 2
        case 3: return PIN_B6; // Up button for floor 3
        default: return 0xFF;  // Invalid floor
    }
}

uint8_t floor_down_button_pin(uint8_t floor) {
    switch (floor) {
        case 0: return PIN_B1; // Down button for floor 0
        case 1: return PIN_B3; // Down button for floor 1
        case 2: return PIN_B5; // Down button for floor 2
        case 3: return PIN_B7; // Down button for floor 3
        default: return 0xFF;  // Invalid floor
    }
}

uint8_t floor_led_pin(uint8_t floor) {
    switch (floor) {
        case 0: return PIN_F4; // Ground Floor LED
        case 1: return PIN_F5; // Floor 1 LED
        case 2: return PIN_F6; // Floor 2 LED
        case 3: return PIN_F7; // Floor 3 LED
        default: return 0xFF;  // Invalid floor
    }
}


#if 0
/**
 * 
 * @param me
 * @param msg
 */
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   static uint8_t heart_beat_cnt = 0;

   //Heart Beat
    if (bit_test(heart_beat_cnt, 4))
    {
        heart_beat_cnt = 0;
        LED1OFF();
        restart_wdt();  //reset WDT
    } else if ((!bit_test(heart_beat_cnt, 2)) && (!bit_test(heart_beat_cnt, 3)))
    {
        LED1TOGGLE();
        heart_beat_cnt++;
    } else
    {
        heart_beat_cnt++;
    }
}
#endif
