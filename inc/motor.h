#ifndef MOTOR_H
#define MOTOR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MOTOR_CH_A = 0,
    MOTOR_CH_B = 1
} Motor_Channel_t;

typedef struct {
    uint16_t pwm_max;
    uint16_t pwm_deadband;
    bool invert_dir;
} Motor_Config_t;

void Motor_Init(void);
void Motor_SetConfig(Motor_Channel_t ch, const Motor_Config_t *cfg);
void Motor_SetDuty(Motor_Channel_t ch, int16_t duty);
void Motor_StopAll(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
