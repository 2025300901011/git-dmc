#include "control.h"

#include <math.h>

#include "app_config.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "safty.h"
#include "vision.h"

static Control_Target_t g_target;
static PID_Handle_t g_pid_pos_x;
static PID_Handle_t g_pid_pos_y;
static PID_Handle_t g_pid_vel_m1;
static PID_Handle_t g_pid_vel_m2;
static float g_m1_ref;
static float g_m2_ref;

static float clampf(float v, float lo, float hi)
{
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

void Control_Init(void)
{
    PID_Config_t cpos = {
        PID_POS_KP_DEFAULT,
        PID_POS_KI_DEFAULT,
        PID_POS_KD_DEFAULT,
        PID_POS_OUT_MAX_DEFAULT,
        PID_POS_I_MAX_DEFAULT
    };
    PID_Config_t cvel = {
        PID_VEL_KP_DEFAULT,
        PID_VEL_KI_DEFAULT,
        PID_VEL_KD_DEFAULT,
        PID_VEL_OUT_MAX_DEFAULT,
        PID_VEL_I_MAX_DEFAULT
    };

    g_target.target_x = TARGET_X_DEFAULT;
    g_target.target_y = TARGET_Y_DEFAULT;
    g_target.enabled  = false;
    g_m1_ref          = 0.0f;
    g_m2_ref          = 0.0f;

    PID_LoadConfig(&g_pid_pos_x, &cpos);
    PID_LoadConfig(&g_pid_pos_y, &cpos);
    PID_LoadConfig(&g_pid_vel_m1, &cvel);
    PID_LoadConfig(&g_pid_vel_m2, &cvel);
}

void Control_SetTarget(float x, float y)
{
    g_target.target_x = x;
    g_target.target_y = y;
}

void Control_Enable(bool en)
{
    g_target.enabled = en;
    if (!en) {
        g_m1_ref = 0.0f;
        g_m2_ref = 0.0f;
        PID_Reset(&g_pid_pos_x);
        PID_Reset(&g_pid_pos_y);
        PID_Reset(&g_pid_vel_m1);
        PID_Reset(&g_pid_vel_m2);
    }
}

void Control_TaskPosLoop(uint32_t now_ms)
{
    Vision_Point_t pt;
    float vx_ref;
    float vy_ref;
    const float dt = (float) CTRL_POS_LOOP_MS * 0.001f;

    (void) now_ms;

    if (!g_target.enabled || !Safty_IsMotorAllowed()) {
        g_m1_ref = 0.0f;
        g_m2_ref = 0.0f;
        return;
    }

    if (!Vision_GetPoint(&pt)) {
        g_m1_ref = 0.0f;
        g_m2_ref = 0.0f;
        return;
    }

    vx_ref = PID_Update(&g_pid_pos_x, g_target.target_x, pt.x, dt);
    vy_ref = PID_Update(&g_pid_pos_y, g_target.target_y, pt.y, dt);

    g_m1_ref = clampf(vx_ref + vy_ref, -PID_POS_OUT_MAX_DEFAULT, PID_POS_OUT_MAX_DEFAULT);
    g_m2_ref = clampf(vx_ref - vy_ref, -PID_POS_OUT_MAX_DEFAULT, PID_POS_OUT_MAX_DEFAULT);
}

void Control_TaskVelLoop(uint32_t now_ms)
{
    float fdb1;
    float fdb2;
    float out1;
    float out2;
    const float dt = (float) CTRL_VEL_LOOP_MS * 0.001f;

    (void) now_ms;

    if (!g_target.enabled || !Safty_IsMotorAllowed()) {
        Motor_StopAll();
        PID_Reset(&g_pid_vel_m1);
        PID_Reset(&g_pid_vel_m2);
        return;
    }

    fdb1 = Encoder_GetSpeedRps(0U);
    fdb2 = Encoder_GetSpeedRps(1U);

    out1 = PID_Update(&g_pid_vel_m1, g_m1_ref, fdb1, dt);
    out2 = PID_Update(&g_pid_vel_m2, g_m2_ref, fdb2, dt);

    if (fabsf(out1) < 1.0f) {
        out1 = 0.0f;
    }
    if (fabsf(out2) < 1.0f) {
        out2 = 0.0f;
    }

    Motor_SetDuty(MOTOR_CH_A, (int16_t) out1);
    Motor_SetDuty(MOTOR_CH_B, (int16_t) out2);
}
