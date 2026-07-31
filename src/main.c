#include "ti_msp_dl_config.h"

#include "app_mode.h"
#include "app_config.h"
#include "control.h"
#include "encoder.h"
#include "key.h"
#include "motor.h"
#include "safty.h"
#include "UART_Hand_PID.h"
#include "vision.h"

/* Keep the generated Debug manifest untouched; its legacy source list omits new C files. */
#define UART_HAND_PID_EMBEDDED
#include "UART_Hand_PID.c"
#define KEY_EMBEDDED
#include "key.c"
#define APP_MODE_EMBEDDED
#include "app_mode.c"

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
    Key_Init();
    AppMode_Init();
    UART_Hand_PID_Init();
    Control_Enable(false);

    while (1) {
        Vision_SetNowMs(now_ms);
        Vision_PollUartRx();
        Encoder_PollGpio();
        Safty_Update(now_ms);
        Key_Task(now_ms);
        AppMode_Task(now_ms);
        UART_Hand_PID_Service(now_ms);

        if (t_vel == 0U) {
            Encoder_UpdateSpeed(CTRL_VEL_LOOP_MS);
            Control_TaskVelLoop(now_ms);
            t_vel = CTRL_VEL_LOOP_MS;
        }
        if (t_pos == 0U) {
            Control_TaskPosLoop(now_ms);
            t_pos = CTRL_POS_LOOP_MS;
        }

        /*
         * Keep the control cadence at approximately 1 ms, but do not leave
         * UART2 unattended for a full millisecond.  At 115200 baud one byte
         * arrives about every 86.8 us; servicing only once per millisecond
         * can overflow the RX FIFO while a 30-byte SETALL frame is arriving.
         */
        for (uint32_t uart_slice = 0U; uart_slice < 10U; uart_slice++) {
            delay_cycles(CPUCLK_FREQ / 10000U);
            UART_Hand_PID_Service(now_ms);
        }
        now_ms++;
        if (t_vel > 0U) {
            t_vel--;
        }
        if (t_pos > 0U) {
            t_pos--;
        }
    }
}
