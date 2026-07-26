#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float target_x;
    float target_y;
    bool enabled;
} Control_Target_t;

void Control_Init(void);
void Control_SetTarget(float x, float y);
void Control_Enable(bool en);
void Control_TaskPosLoop(uint32_t now_ms);
void Control_TaskVelLoop(uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif /* CONTROL_H */
