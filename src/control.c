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
static PID_Handle_t g_pid_pipe;
static float g_m1_ref;
static float g_m2_ref;
static float g_pipe_target_count;

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
    PID_Config_t cpipe = {
        PIPE_MOTOR_POS_KP_DEFAULT,
        PIPE_MOTOR_POS_KI_DEFAULT,
        PIPE_MOTOR_POS_KD_DEFAULT,
        PIPE_PWM_MAX_DEFAULT,
        PIPE_MOTOR_POS_I_MAX_DEFAULT
    };

    g_target.target_x = TARGET_X_DEFAULT;
    g_target.target_y = TARGET_Y_DEFAULT;
    g_target.enabled  = false;
    g_m1_ref          = 0.0f;
    g_m2_ref          = 0.0f;
    g_pipe_target_count = 0.0f;

    PID_LoadConfig(&g_pid_pos_x, &cpos);
    PID_LoadConfig(&g_pid_pos_y, &cpos);
    PID_LoadConfig(&g_pid_vel_m1, &cvel);
    PID_LoadConfig(&g_pid_vel_m2, &cvel);
    PID_LoadConfig(&g_pid_pipe, &cpipe);
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
        PID_Reset(&g_pid_pipe);
        g_pipe_target_count = 0.0f;
    }
}

void Control_TaskPosLoop(uint32_t now_ms)
{
    Vision_Point_t pt;
#if CONTROL_MODE_PIPE_BALANCE
    float target;
    float fdb;
    float raw_target_count;
    float delta;
#else
    float vx_ref;
    float vy_ref;
    const float dt = (float) CTRL_POS_LOOP_MS * 0.001f;
#endif

    (void) now_ms;

    if (!g_target.enabled || !Safty_IsMotorAllowed()) {
        g_m1_ref = 0.0f;
        g_m2_ref = 0.0f;
        g_pipe_target_count = 0.0f;
        return;
    }

    if (!Vision_GetPoint(&pt)) {
        g_m1_ref = 0.0f;
        g_m2_ref = 0.0f;
        g_pipe_target_count = 0.0f;
        return;
    }

#if CONTROL_MODE_PIPE_BALANCE
#if PIPE_USE_X_AXIS
    target = g_target.target_x;
    fdb    = pt.x;
#else
    target = g_target.target_y;
    fdb    = pt.y;
#endif

    raw_target_count = (target - fdb) * PIPE_ENCODER_COUNTS_PER_PIXEL;
#if PIPE_CONTROL_INVERT
    raw_target_count = -raw_target_count;
#endif
    raw_target_count = clampf(raw_target_count, -PIPE_TARGET_COUNT_MAX, PIPE_TARGET_COUNT_MAX);

    delta = raw_target_count - g_pipe_target_count;
    delta = clampf(delta, -PIPE_TARGET_COUNT_SLEW_STEP, PIPE_TARGET_COUNT_SLEW_STEP);
    g_pipe_target_count += delta;
#else
    vx_ref = PID_Update(&g_pid_pos_x, g_target.target_x, pt.x, dt);
    vy_ref = PID_Update(&g_pid_pos_y, g_target.target_y, pt.y, dt);

    g_m1_ref = clampf(vx_ref + vy_ref, -PID_POS_OUT_MAX_DEFAULT, PID_POS_OUT_MAX_DEFAULT);
    g_m2_ref = clampf(vx_ref - vy_ref, -PID_POS_OUT_MAX_DEFAULT, PID_POS_OUT_MAX_DEFAULT);
#endif
}

void Control_TaskVelLoop(uint32_t now_ms)
{
    const float dt = (float) CTRL_VEL_LOOP_MS * 0.001f;
#if !CONTROL_MODE_PIPE_BALANCE
    float fdb1;
    float fdb2;
    float out1;
    float out2;
#endif

    (void) now_ms;

    if (!g_target.enabled || !Safty_IsMotorAllowed()) {
        Motor_StopAll();
        PID_Reset(&g_pid_vel_m1);
        PID_Reset(&g_pid_vel_m2);
        PID_Reset(&g_pid_pipe);
        g_pipe_target_count = 0.0f;
        return;
    }

#if CONTROL_MODE_PIPE_BALANCE
    {
        float count_now = (float) Encoder_GetCount(PIPE_ENCODER_IDX);
        float err_count = g_pipe_target_count - count_now;
        float out = PID_Update(&g_pid_pipe, g_pipe_target_count, count_now, dt);

        if (fabsf(err_count) < PIPE_TARGET_COUNT_TOLERANCE) {
            out = 0.0f;
            PID_Reset(&g_pid_pipe);
        } else if (fabsf(out) < PIPE_PWM_MIN_EFFECTIVE) {
            out = (out >= 0.0f) ? PIPE_PWM_MIN_EFFECTIVE : -PIPE_PWM_MIN_EFFECTIVE;
        }

        Motor_SetDuty(PIPE_MOTOR_CH, (int16_t) out);
    }
#else
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
#endif
}
