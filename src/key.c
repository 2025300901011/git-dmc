#include "key.h"

#ifdef KEY_EMBEDDED

#include "app_config.h"
#include "hw_map.h"

typedef struct {
    bool stable_pressed;
    bool last_raw_pressed;
    uint8_t debounce_count;
    bool pressed_event;
} Key_State_t;

static Key_State_t g_keys[KEY_ID_COUNT];
static uint32_t g_next_scan_ms;

static bool key_read_raw_pressed(Key_Id_t key)
{
    switch (key) {
        case KEY_ID_1:
            return (DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY1_PIN) == 0U);
        case KEY_ID_2:
            return (DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY2_PIN) == 0U);
        case KEY_ID_3:
            return (DL_GPIO_readPins(GPIO_KEYS_KEY3_PORT, GPIO_KEYS_KEY3_PIN) == 0U);
        case KEY_ID_4:
            return (DL_GPIO_readPins(GPIO_KEYS_PORT, GPIO_KEYS_KEY4_PIN) == 0U);
        default:
            return false;
    }
}

void Key_Init(void)
{
    for (uint8_t i = 0U; i < (uint8_t) KEY_ID_COUNT; i++) {
        g_keys[i].stable_pressed = false;
        g_keys[i].last_raw_pressed = false;
        g_keys[i].debounce_count = 0U;
        g_keys[i].pressed_event = false;
    }
    g_next_scan_ms = 0U;
}

void Key_Task(uint32_t now_ms)
{
    if ((int32_t) (now_ms - g_next_scan_ms) < 0) {
        return;
    }
    g_next_scan_ms = now_ms + APP_KEY_SCAN_PERIOD_MS;

    for (uint8_t i = 0U; i < (uint8_t) KEY_ID_COUNT; i++) {
        Key_State_t *state = &g_keys[i];
        bool raw_pressed = key_read_raw_pressed((Key_Id_t) i);

        if (raw_pressed == state->last_raw_pressed) {
            if (state->debounce_count < APP_KEY_DEBOUNCE_TICKS) {
                state->debounce_count++;
            }
        } else {
            state->last_raw_pressed = raw_pressed;
            state->debounce_count = 0U;
        }

        if (state->debounce_count >= APP_KEY_DEBOUNCE_TICKS &&
            raw_pressed != state->stable_pressed) {
            state->stable_pressed = raw_pressed;
            if (raw_pressed) {
                state->pressed_event = true;
            }
        }
    }
}

bool Key_WasPressed(Key_Id_t key)
{
    bool event;

    if (key >= KEY_ID_COUNT) {
        return false;
    }

    event = g_keys[key].pressed_event;
    g_keys[key].pressed_event = false;
    return event;
}

#endif /* KEY_EMBEDDED */
