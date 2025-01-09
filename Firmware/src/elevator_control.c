#include "main.h"
#include "hardware.h"
#include "scheduler.h"
#include "elevator_control.h"

// Globals
static uint8_t current_floor = 0; // Start at Ground Floor

// Task Prototypes
void task_move_elevator(s_task_handle_t me, s_task_msg_t **msg, void *arg);

// Initialize the elevator control system
bool init_elevator_control(void) {
    // Create the elevator movement task
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_move_elevator, NULL, NULL);
}

// Task to move the elevator
void task_move_elevator(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    static floor_request_t active_request;
    static bool processing_request = false;

    if (!processing_request) {
        // Get the next request from the queue
        if (get_next_floor(&active_request)) {
            processing_request = true;
        } else {
            return; // No requests, nothing to do
        }
    }

    // Handle elevator movement
    if (active_request.floor > current_floor) {
        output_high(MOTOR_UP_PIN);
        output_low(MOTOR_DOWN_PIN);
        current_floor++;
    } else if (active_request.floor < current_floor) {
        output_high(MOTOR_DOWN_PIN);
        output_low(MOTOR_UP_PIN);
        current_floor--;
    } else {
        output_low(MOTOR_UP_PIN);
        output_low(MOTOR_DOWN_PIN);
        processing_request = false;
        simulate_door_opening();
    }


#ifdef USE_MESSAGING
    s_task_flush_msgs(msg);
#endif
}

// Simulate door opening
void simulate_door_opening(void) {
    output_high(DOOR_OPEN_PIN);  // Simulate door opening
    s_task_b_delay(3000);       // Simulate door open delay (replace with non-blocking task if required)
    output_low(DOOR_OPEN_PIN);  // Close door
}


void task_update_leds(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    update_floor_leds();

    #ifdef USE_MESSAGING
        s_task_flush_msgs(msg);
    #endif
}

