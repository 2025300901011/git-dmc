#include "app_mode.h"

#ifdef APP_MODE_EMBEDDED

#include <stdbool.h>

#include "app_config.h"
#include "control.h"
#include "key.h"
#include "vision.h"

typedef enum {
    APP_MODE_OFF = 0,
    APP_MODE_READY,
    APP_MODE_CENTER_BALANCE,
    APP_MODE_MOVE_POS_5CM,
    APP_MODE_MOVE_NEG_5CM
} App_Mode_t;

static App_Mode_t g_mode;
static bool g_started;
static uint32_t g_stable_since_ms;

static float app_absf(float v)
{
    return (v < 0.0f) ? -v : v;
}

static void app_set_target(float x, float y)
{
    Control_SetTarget(x, y);
    Control_Enable(true);
    g_stable_since_ms = 0U;
}

static bool app_target_is_stable(float target_x, float target_y, uint32_t now_ms)
{
    Vision_Point_t pt;
    float err;

    if (!Vision_GetPoint(&pt)) {
        g_stable_since_ms = 0U;
        return false;
    }

#if PIPE_USE_X_AXIS
    (void) target_y;
    err = app_absf(pt.x - target_x);
#else
    (void) target_x;
    err = app_absf(pt.y - target_y);
#endif

    if (err > APP_TARGET_STABLE_TOLERANCE) {
        g_stable_since_ms = 0U;
        return false;
    }

    if (g_stable_since_ms == 0U) {
        g_stable_since_ms = now_ms;
        return false;
    }

    return ((uint32_t) (now_ms - g_stable_since_ms) >= APP_TARGET_STABLE_TIME_MS);
}

void AppMode_Init(void)
{
    g_mode = APP_MODE_OFF;
    g_started = false;
    g_stable_since_ms = 0U;
}

void AppMode_Task(uint32_t now_ms)
{
    if (Key_WasPressed(KEY_ID_1)) {
        g_started = true;
        g_mode = APP_MODE_READY;
        Control_Enable(false);
    }

    if (!g_started) {
        return;
    }

    if (Key_WasPressed(KEY_ID_2)) {
        app_set_target(APP_TARGET_POS_5CM_X, APP_TARGET_POS_5CM_Y);
        g_mode = APP_MODE_MOVE_POS_5CM;
    }

    if (Key_WasPressed(KEY_ID_3)) {
        app_set_target(APP_TARGET_CENTER_X, APP_TARGET_CENTER_Y);
        g_mode = APP_MODE_CENTER_BALANCE;
    }

    switch (g_mode) {
        case APP_MODE_MOVE_POS_5CM:
            if (app_target_is_stable(APP_TARGET_POS_5CM_X, APP_TARGET_POS_5CM_Y, now_ms)) {
                app_set_target(APP_TARGET_NEG_5CM_X, APP_TARGET_NEG_5CM_Y);
                g_mode = APP_MODE_MOVE_NEG_5CM;
            }
            break;

        case APP_MODE_MOVE_NEG_5CM:
            (void) app_target_is_stable(APP_TARGET_NEG_5CM_X, APP_TARGET_NEG_5CM_Y, now_ms);
            break;

        case APP_MODE_CENTER_BALANCE:
        case APP_MODE_READY:
        case APP_MODE_OFF:
        default:
            break;
    }
}

#endif /* APP_MODE_EMBEDDED */
