#include "encoder.h"

#include "app_config.h"
#include "hw_map.h"

static Encoder_State_t g_enc[2];
static uint32_t g_qei_last_raw;
static uint8_t g_enc2_last_state;
static int32_t g_enc2_last_count;

static const int8_t g_quad_lut[16] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

static uint8_t enc2_read_state(void)
{
    uint8_t s = 0U;
    if (DL_GPIO_readPins(GPIO_ENC2_INPUT_PORT, GPIO_ENC2_INPUT_ENC2_A_PIN) != 0U) {
        s |= 0x01U;
    }
    if (DL_GPIO_readPins(GPIO_ENC2_INPUT_PORT, GPIO_ENC2_INPUT_ENC2_B_PIN) != 0U) {
        s |= 0x02U;
    }
    return s;
}

static int32_t wrap_delta_16(uint32_t now, uint32_t last)
{
    int32_t d = (int32_t) now - (int32_t) last;
    if (d > 32767) {
        d -= 65536;
    } else if (d < -32768) {
        d += 65536;
    }
    return d;
}

void Encoder_Init(void)
{
    g_enc[0].count_raw = 0;
    g_enc[1].count_raw = 0;
    g_enc[0].delta     = 0;
    g_enc[1].delta     = 0;
    g_enc[0].speed_rps = 0.0f;
    g_enc[1].speed_rps = 0.0f;

    g_qei_last_raw   = DL_Timer_getTimerCount(QEI_ENC1_INST);
    g_enc2_last_state = enc2_read_state();
    g_enc2_last_count = 0;

    DL_Timer_startCounter(QEI_ENC1_INST);
}

void Encoder_PollGpio(void)
{
    uint8_t now_state = enc2_read_state();
    uint8_t idx       = (uint8_t) ((g_enc2_last_state << 2U) | now_state);
    int8_t step       = g_quad_lut[idx & 0x0FU];

    if (step != 0) {
        g_enc[1].count_raw += (int32_t) step;
    }
    g_enc2_last_state = now_state;
}

void Encoder_UpdateSpeed(uint32_t dt_ms)
{
    uint32_t qei_now;
    int32_t d0;
    int32_t d1;
    float dt_s;
    float alpha;

    if (dt_ms == 0U) {
        return;
    }

    qei_now = DL_Timer_getTimerCount(QEI_ENC1_INST);
    d0      = wrap_delta_16(qei_now, g_qei_last_raw);
    d1      = g_enc[1].count_raw - g_enc2_last_count;

    g_qei_last_raw    = qei_now;
    g_enc[0].count_raw += d0;
    g_enc[0].delta     = d0;

    g_enc[1].delta = d1;
    g_enc2_last_count = g_enc[1].count_raw;

    dt_s  = (float) dt_ms * 0.001f;
    alpha = ENCODER_SPEED_FILTER_ALPHA;

    g_enc[0].speed_rps = (1.0f - alpha) * g_enc[0].speed_rps + alpha * ((float) d0 / dt_s);
    g_enc[1].speed_rps = (1.0f - alpha) * g_enc[1].speed_rps + alpha * ((float) d1 / dt_s);
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
