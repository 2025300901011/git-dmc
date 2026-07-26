#include "ti_msp_dl_config.h"

#include "app_config.h"
#include "control.h"
#include "encoder.h"
#include "motor.h"
#include "safty.h"
#include "vision.h"

int main(void)
{
    uint32_t now_ms = 0U;
    uint32_t t_pos  = 0U;
    uint32_t t_vel  = 0U;

    SYSCFG_DL_init();

    Motor_Init();
    Encoder_Init();
    Vision_Init();
    Safty_Init();
    Control_Init();
    Control_Enable(true);

    while (1) {
        Vision_SetNowMs(now_ms);
        Vision_PollUartRx();
        Encoder_PollGpio();
        Safty_Update(now_ms);

        if (t_vel == 0U) {
            Encoder_UpdateSpeed(CTRL_VEL_LOOP_MS);
            Control_TaskVelLoop(now_ms);
            t_vel = CTRL_VEL_LOOP_MS;
        }
        if (t_pos == 0U) {
            Control_TaskPosLoop(now_ms);
            t_pos = CTRL_POS_LOOP_MS;
        }

        delay_cycles(CPUCLK_FREQ / 1000U);
        now_ms++;
        if (t_vel > 0U) {
            t_vel--;
        }
        if (t_pos > 0U) {
            t_pos--;
        }
    }
}
