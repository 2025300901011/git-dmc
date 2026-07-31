#include "encoder.h"

#include <stdbool.h>

#include "app_config.h"
#include "hw_map.h"

#define ENCODER_LEFT_REVERSE  (0)
#define ENCODER_RIGHT_REVERSE (1)

static Encoder_State_t g_enc[2];
static volatile int32_t g_count[2];
static int32_t g_last_count[2];

static void Encoder_HandleEdge(uint8_t idx)
{
    uint32_t phase_a_pin = (idx == 0U) ? GPIO_ENCODER_ENCODER_LEFT_A_PIN :
                                         GPIO_ENCODER_ENCODER_RIGHT_A_PIN;
    uint32_t phase_b_pin = (idx == 0U) ? GPIO_ENCODER_ENCODER_LEFT_B_PIN :
                                         GPIO_ENCODER_ENCODER_RIGHT_B_PIN;
    bool phase_a_high = DL_GPIO_readPins(GPIO_ENCODER_PORT, phase_a_pin) != 0U;
    bool phase_b_high = DL_GPIO_readPins(GPIO_ENCODER_PORT, phase_b_pin) != 0U;
    int32_t step = (phase_a_high == phase_b_high) ? 1 : -1;

    if ((idx == 0U && ENCODER_LEFT_REVERSE) ||
        (idx == 1U && ENCODER_RIGHT_REVERSE)) {
        step = -step;
    }
    g_count[idx] += step;
}

void Encoder_Init(void)
{
    g_enc[0].count_raw = 0;
    g_enc[1].count_raw = 0;
    g_enc[0].delta = 0;
    g_enc[1].delta = 0;
    g_enc[0].speed_rps = 0.0f;
    g_enc[1].speed_rps = 0.0f;
    g_count[0] = 0;
    g_count[1] = 0;
    g_last_count[0] = 0;
    g_last_count[1] = 0;

    DL_GPIO_clearInterruptStatus(
        GPIO_ENCODER_PORT,
        GPIO_ENCODER_ENCODER_LEFT_A_PIN | GPIO_ENCODER_ENCODER_RIGHT_A_PIN);
    NVIC_EnableIRQ(GPIO_ENCODER_INT_IRQN);
}

void Encoder_PollGpio(void)
{
    /* Encoder edges are captured by GROUP1_IRQHandler on the normal PCB. */
}

void Encoder_UpdateSpeed(uint32_t dt_ms)
{
    int32_t count0;
    int32_t count1;
    float dt_s;
    float alpha;

    if (dt_ms == 0U) {
        return;
    }

    NVIC_DisableIRQ(GPIO_ENCODER_INT_IRQN);
    count0 = g_count[0];
    count1 = g_count[1];
    NVIC_EnableIRQ(GPIO_ENCODER_INT_IRQN);

    g_enc[0].delta = count0 - g_last_count[0];
    g_enc[1].delta = count1 - g_last_count[1];
    g_last_count[0] = count0;
    g_last_count[1] = count1;
    g_enc[0].count_raw += g_enc[0].delta;
    g_enc[1].count_raw += g_enc[1].delta;

    dt_s = (float) dt_ms * 0.001f;
    alpha = ENCODER_SPEED_FILTER_ALPHA;
    g_enc[0].speed_rps = (1.0f - alpha) * g_enc[0].speed_rps +
                         alpha * ((float) g_enc[0].delta / dt_s);
    g_enc[1].speed_rps = (1.0f - alpha) * g_enc[1].speed_rps +
                         alpha * ((float) g_enc[1].delta / dt_s);
}

int32_t Encoder_GetCount(uint8_t idx)
{
    if (idx > 1U) {
        return 0;
    }
    return g_enc[idx].count_raw;
}

float Encoder_GetSpeedRps(uint8_t idx)
{
    if (idx > 1U) {
        return 0.0f;
    }
    return g_enc[idx].speed_rps;
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_ENCODER_INT_IIDX:
            if (DL_GPIO_getEnabledInterruptStatus(
                    GPIO_ENCODER_PORT, GPIO_ENCODER_ENCODER_LEFT_A_PIN) != 0U) {
                Encoder_HandleEdge(0U);
                DL_GPIO_clearInterruptStatus(
                    GPIO_ENCODER_PORT, GPIO_ENCODER_ENCODER_LEFT_A_PIN);
            }
            if (DL_GPIO_getEnabledInterruptStatus(
                    GPIO_ENCODER_PORT, GPIO_ENCODER_ENCODER_RIGHT_A_PIN) != 0U) {
                Encoder_HandleEdge(1U);
                DL_GPIO_clearInterruptStatus(
                    GPIO_ENCODER_PORT, GPIO_ENCODER_ENCODER_RIGHT_A_PIN);
            }
            break;
        default:
            break;
    }
}
