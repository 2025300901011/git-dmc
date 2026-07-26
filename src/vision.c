#include "vision.h"

#include "app_config.h"
#include "hw_map.h"

static Vision_Point_t g_pt;
static uint8_t g_frame[6];
static uint8_t g_state;
static uint32_t g_now_ms;

void Vision_Init(void)
{
    g_pt.x     = TARGET_X_DEFAULT;
    g_pt.y     = TARGET_Y_DEFAULT;
    g_pt.ts_ms = 0U;
    g_pt.valid = false;
    g_state    = 0U;
    g_now_ms   = 0U;
}

void Vision_SetNowMs(uint32_t now_ms)
{
    g_now_ms = now_ms;
}

void Vision_PollUartRx(void)
{
    while (!DL_UART_isRXFIFOEmpty(UART_OPENMV_INST)) {
        uint8_t b = DL_UART_receiveData(UART_OPENMV_INST);
        Vision_RxFeed(b);
    }
}

void Vision_RxFeed(uint8_t byte_in)
{
    switch (g_state) {
        case 0:
            if (byte_in == (uint8_t) VISION_FRAME_HEADER) {
                g_frame[0] = byte_in;
                g_state    = 1U;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            g_frame[g_state] = byte_in;
            g_state++;
            break;
        case 5:
            g_frame[5] = byte_in;
            if (g_frame[5] == (uint8_t) VISION_FRAME_TAIL) {
                uint16_t raw_x = (uint16_t) g_frame[1] | ((uint16_t) g_frame[2] << 8);
                uint16_t raw_y = (uint16_t) g_frame[3] | ((uint16_t) g_frame[4] << 8);
                g_pt.x         = (float) raw_x;
                g_pt.y         = (float) raw_y;
                g_pt.ts_ms     = g_now_ms;
                g_pt.valid     = true;
            }
            g_state = 0U;
            break;
        default:
            g_state = 0U;
            break;
    }
}

bool Vision_GetPoint(Vision_Point_t *pt_out)
{
    if (pt_out == 0) {
        return false;
    }
    *pt_out = g_pt;
    return g_pt.valid;
}

bool Vision_IsTimeout(uint32_t now_ms, uint32_t timeout_ms)
{
    if (!g_pt.valid) {
        return true;
    }
    return ((now_ms - g_pt.ts_ms) > timeout_ms);
}
