#ifndef ELEVATOR_MOTOR_H
#define ELEVATOR_MOTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    FLOOR_GF,
    FLOOR_F1,
    FLOOR_F2,
    FLOOR_F3
} floor_t;

// prototypes
bool init_elevator_tasks(void);


#endif
