#include "pid.h"

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

void PID_Reset(PID_Handle_t *pid)
{
    if (pid == 0) {
        return;
    }
    pid->integral = 0.0f;
    pid->last_err = 0.0f;
    pid->last_fdb = 0.0f;
    pid->out      = 0.0f;
}

void PID_LoadConfig(PID_Handle_t *pid, const PID_Config_t *cfg)
{
    if ((pid == 0) || (cfg == 0)) {
        return;
    }
    pid->cfg = *cfg;
    PID_Reset(pid);
}

float PID_Update(PID_Handle_t *pid, float ref, float fdb, float dt_s)
{
    float err;
    float der;
    float out;

    if ((pid == 0) || (dt_s <= 0.0f)) {
        return 0.0f;
    }

    err = ref - fdb;
    der = (err - pid->last_err) / dt_s;

    pid->integral += err * dt_s;
    pid->integral = clampf(pid->integral, -pid->cfg.i_max, pid->cfg.i_max);

    out = pid->cfg.kp * err + pid->cfg.ki * pid->integral + pid->cfg.kd * der;
    out = clampf(out, -pid->cfg.out_max, pid->cfg.out_max);

    pid->last_err = err;
    pid->last_fdb = fdb;
    pid->out      = out;

    return out;
}
