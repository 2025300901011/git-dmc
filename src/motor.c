#include "motor.h"

#include "app_config.h"
#include "hw_map.h"

static Motor_Config_t g_motor_cfg[2];

static int16_t clamp_duty(int16_t duty, uint16_t lim)
{
    if (duty > (int16_t) lim) {
        return (int16_t) lim;
    }
    if (duty < -(int16_t) lim) {
        return -(int16_t) lim;
    }
    return duty;
}

static uint32_t to_cc(uint16_t duty_abs)
{
    return ((uint32_t) duty_abs * (uint32_t) MOTOR_PWM_PERIOD_COUNTS) / (uint32_t) MOTOR_PWM_MAX;
}

static void set_m1_dir(bool forward)
{
    if (forward) {
        DL_GPIO_setPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M1_AIN1_PIN);
        DL_GPIO_clearPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M1_AIN2_PIN);
    } else {
        DL_GPIO_clearPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M1_AIN1_PIN);
        DL_GPIO_setPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M1_AIN2_PIN);
    }
}

static void set_m2_dir(bool forward)
{
    if (forward) {
        DL_GPIO_setPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M2_BIN1_PIN);
        DL_GPIO_clearPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M2_BIN2_PIN);
    } else {
        DL_GPIO_clearPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M2_BIN1_PIN);
        DL_GPIO_setPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M2_BIN2_PIN);
    }
}

void Motor_Init(void)
{
    Motor_Config_t cfg_default;

    cfg_default.pwm_max      = MOTOR_PWM_MAX;
    cfg_default.pwm_deadband = MOTOR_PWM_DEADBAND;
    cfg_default.invert_dir   = false;

    g_motor_cfg[0] = cfg_default;
    g_motor_cfg[1] = cfg_default;

    DL_GPIO_setPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_DRV_STBY_PIN);
    DL_Timer_startCounter(PWM_TB6612_INST);
    Motor_StopAll();
}

void Motor_SetConfig(Motor_Channel_t ch, const Motor_Config_t *cfg)
{
    if ((cfg == 0) || (ch > MOTOR_CH_B)) {
        return;
    }
    g_motor_cfg[ch] = *cfg;
}

void Motor_SetDuty(Motor_Channel_t ch, int16_t duty)
{
    Motor_Config_t *cfg;
    bool forward;
    uint16_t duty_abs;
    uint32_t cc;

    if (ch > MOTOR_CH_B) {
        return;
    }

    cfg      = &g_motor_cfg[ch];
    duty     = clamp_duty(duty, cfg->pwm_max);
    forward  = (duty >= 0);
    duty_abs = (uint16_t) ((duty >= 0) ? duty : -duty);

    if (duty_abs < cfg->pwm_deadband) {
        duty_abs = 0;
    }
    if (cfg->invert_dir) {
        forward = !forward;
    }

    if (ch == MOTOR_CH_A) {
        set_m1_dir(forward);
        cc = to_cc(duty_abs);
        DL_Timer_setCaptureCompareValue(PWM_TB6612_INST, cc, DL_TIMER_CC_0_INDEX);
    } else {
        set_m2_dir(forward);
        cc = to_cc(duty_abs);
        DL_Timer_setCaptureCompareValue(PWM_TB6612_INST, cc, DL_TIMER_CC_1_INDEX);
    }
}

void Motor_StopAll(void)
{
    DL_Timer_setCaptureCompareValue(PWM_TB6612_INST, 0U, DL_TIMER_CC_0_INDEX);
    DL_Timer_setCaptureCompareValue(PWM_TB6612_INST, 0U, DL_TIMER_CC_1_INDEX);

    DL_GPIO_clearPins(GPIO_TB6612_CTRL_PORT, GPIO_TB6612_CTRL_M1_AIN1_PIN |
                                                 GPIO_TB6612_CTRL_M1_AIN2_PIN |
                                                 GPIO_TB6612_CTRL_M2_BIN1_PIN |
                                                 GPIO_TB6612_CTRL_M2_BIN2_PIN);
}
