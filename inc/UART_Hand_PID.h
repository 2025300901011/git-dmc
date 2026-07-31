#ifndef UART_HAND_PID_H
#define UART_HAND_PID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void UART_Hand_PID_Init(void);
void UART_Hand_PID_Service(uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif /* UART_HAND_PID_H */
