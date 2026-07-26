#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t count_raw;
    int32_t delta;
    float speed_rps;
} Encoder_State_t;

void Encoder_Init(void);
void Encoder_PollGpio(void);
void Encoder_UpdateSpeed(uint32_t dt_ms);
int32_t Encoder_GetCount(uint8_t idx);
float Encoder_GetSpeedRps(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H */
