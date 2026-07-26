#ifndef VISION_H
#define VISION_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x;
    float y;
    uint32_t ts_ms;
    bool valid;
} Vision_Point_t;

void Vision_Init(void);
void Vision_SetNowMs(uint32_t now_ms);
void Vision_PollUartRx(void);
void Vision_RxFeed(uint8_t byte_in);
bool Vision_GetPoint(Vision_Point_t *pt_out);
bool Vision_IsTimeout(uint32_t now_ms, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* VISION_H */
