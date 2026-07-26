#include "safty.h"

#include "app_config.h"
#include "vision.h"

static Safty_Flags_t g_flags;

void Safty_Init(void)
{
    g_flags.vision_timeout = true;
    g_flags.driver_fault   = false;
    g_flags.emergency_stop = false;
}

void Safty_Update(uint32_t now_ms)
{
    g_flags.vision_timeout = Vision_IsTimeout(now_ms, SAFE_LOST_FRAME_STOP_MS);
}

bool Safty_IsMotorAllowed(void)
{
    if (g_flags.emergency_stop) {
        return false;
    }
    if (g_flags.driver_fault) {
        return false;
    }
    if (g_flags.vision_timeout) {
        return false;
    }
    return true;
}

const Safty_Flags_t *Safty_GetFlags(void)
{
    return &g_flags;
}

void Safty_TriggerEStop(void)
{
    g_flags.emergency_stop = true;
}

void Safty_ClearEStop(void)
{
    g_flags.emergency_stop = false;
}
