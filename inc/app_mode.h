#ifndef APP_MODE_H
#define APP_MODE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void AppMode_Init(void);
void AppMode_Task(uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif /* APP_MODE_H */
