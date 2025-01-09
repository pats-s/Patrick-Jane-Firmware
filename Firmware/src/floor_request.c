#include "main.h"
#include "floor_request.h"
#include "hardware.h"
#include "scheduler.h"

// Floor Request Queue
static floor_request_queue_t request_queue;

// Task Prototypes
void task_read_buttons(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void task_handle_floor_requests(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void task_update_leds(s_task_handle_t me, s_task_msg_t **msg, void *arg);

// Initialize the floor request system and create tasks
bool init_floor_request(void) {
    // Initialize the request queue
    request_queue.head = 0;
    request_queue.tail = 0;
    request_queue.count = 0;

    // Create the task for reading button inputs
    if (!s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_read_buttons, NULL, NULL)) {
        return false; // Task creation failed
    }

    // Create the task for handling floor requests
    if (!s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_handle_floor_requests, NULL, NULL)) {
        return false; // Task creation failed
    }

    if(!s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_update_leds, NULL, NULL)){
        return false;
    }
    return true; // Initialization successful
}

// Add a floor request to the queue
bool add_floor_request(uint8_t floor, direction_t dir) {
    if (request_queue.count >= MAX_FLOORS) {
        return false; // Queue is full
    }

    request_queue.queue[request_queue.tail].floor = floor;
    request_queue.queue[request_queue.tail].dir = dir;
    request_queue.tail = (request_queue.tail + 1) % MAX_FLOORS;
    request_queue.count++;
    return true;
}

// Get the next floor from the queue
bool get_next_floor(floor_request_t *request) {
    if (request_queue.count == 0) {
        return false; // No requests
    }

    *request = request_queue.queue[request_queue.head];
    request_queue.head = (request_queue.head + 1) % MAX_FLOORS;
    request_queue.count--;
    return true;
}

// Task to read button inputs
void task_read_buttons(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    static bool button_state_up[MAX_FLOORS] = {false};   // State of Up buttons
    static bool button_state_down[MAX_FLOORS] = {false}; // State of Down buttons

    for (uint8_t floor = 0; floor < MAX_FLOORS; floor++) {
        bool current_up = input(floor_up_button_pin(floor));
        bool current_down = input(floor_down_button_pin(floor));

        // Up button pressed
        if (current_up && !button_state_up[floor]) {
            add_floor_request(floor, DIRECTION_UP);
        }
        // Down button pressed
        if (current_down && !button_state_down[floor]) {
            add_floor_request(floor, DIRECTION_DOWN);
        }

        button_state_up[floor] = current_up;   // Update state
        button_state_down[floor] = current_down; // Update state
    }

#ifdef USE_MESSAGING
    s_task_flush_msgs(msg);
#endif
}

// Task to handle floor requests
void task_handle_floor_requests(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    floor_request_t request;

    if (get_next_floor(&request)) {
        // Process the request
        if (request.dir == DIRECTION_UP) {
            // Notify motor task to move up
        } else if (request.dir == DIRECTION_DOWN) {
            // Notify motor task to move down
        }
    }

#ifdef USE_MESSAGING
    s_task_flush_msgs(msg);
#endif
}

void update_floor_leds(void) {
    for (uint8_t floor = 0; floor < MAX_FLOORS; floor++) {
        if (floor == current_floor) {
            output_high(floor_led_pin(floor)); // Turn on LED for current floor
        } else {
            output_low(floor_led_pin(floor));  // Turn off LEDs for other floors
        }
    }
}

void task_update_leds(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    update_floor_leds();

    #ifdef USE_MESSAGING
        s_task_flush_msgs(msg);
    #endif
}

void task_update_elevator_position(s_task_handle_t me, s_task_msg_t **msg, void *arg) {
    static uint8_t next_floor = 0; // Example: set this value dynamically based on requests

    // Simulate checking if the motor has arrived at the floor
    if (motor_arrives_at_floor(next_floor)) { 
        current_floor = next_floor;  // Update the current floor
        update_floor_leds();         // Update the LEDs
    }

#ifdef USE_MESSAGING
    s_task_flush_msgs(msg);
#endif
}

