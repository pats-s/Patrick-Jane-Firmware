#ifndef ELEVATOR_CONTROL_H
#define ELEVATOR_CONTROL_H

#include "stdint.h"
#include "stdbool.h"

// Function Prototypes
bool init_elevator_control(void);
void task_move_elevator(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void simulate_door_opening(void);

#endif
