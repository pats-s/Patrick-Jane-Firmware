#ifndef FLOOR_REQUEST_H
#define FLOOR_REQUEST_H

#include "stdbool.h"
#include "stdint.h"

// Max floors supported
#define MAX_FLOORS 4

// Floor Directions
typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_NONE
} direction_t;

// Floor Request Structure
typedef struct {
    uint8_t floor;       // Requested floor
    direction_t dir;     // Requested direction
} floor_request_t;

// Floor Request Queue
typedef struct {
    floor_request_t queue[MAX_FLOORS]; // Stores floor requests
    uint8_t head;                      // Points to the next request to process
    uint8_t tail;                      // Points to the next empty space in the queue
    uint8_t count;                     // Number of active requests
} floor_request_queue_t;

// Function prototypes
bool init_floor_request(void);
bool add_floor_request(uint8_t floor, direction_t dir);
bool get_next_floor(floor_request_t *request);


#endif
