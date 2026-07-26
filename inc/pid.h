#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float kp;
    float ki;
    float kd;
    float out_max;
    float i_max;
} PID_Config_t;

typedef struct {
    PID_Config_t cfg;
    float integral;
    float last_err;
    float last_fdb;
    float out;
} PID_Handle_t;

void PID_Reset(PID_Handle_t *pid);
void PID_LoadConfig(PID_Handle_t *pid, const PID_Config_t *cfg);
float PID_Update(PID_Handle_t *pid, float ref, float fdb, float dt_s);

#ifdef __cplusplus
}
#endif

#endif /* PID_H */
