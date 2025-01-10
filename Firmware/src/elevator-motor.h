#ifndef ELEVATOR_MOTOR_H
#define ELEVATOR_MOTOR_H

#include <stdint.h>
#include <stdbool.h>


// Elevator States
typedef enum {
    FLOOR_GF,
    FLOOR_F1,
    FLOOR_F2,
    FLOOR_F3
} floor_t;

// Functions
bool init_elevator_tasks(void);


#endif
