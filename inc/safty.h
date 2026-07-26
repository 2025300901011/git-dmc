#ifndef SAFTY_H
#define SAFTY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool vision_timeout;
    bool driver_fault;
    bool emergency_stop;
} Safty_Flags_t;

void Safty_Init(void);
void Safty_Update(uint32_t now_ms);
bool Safty_IsMotorAllowed(void);
const Safty_Flags_t *Safty_GetFlags(void);
void Safty_TriggerEStop(void);
void Safty_ClearEStop(void);

#ifdef __cplusplus
}
#endif

#endif /* SAFTY_H */
