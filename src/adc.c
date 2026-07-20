#include <stdint.h>

#include "ti_msp_dl_config.h"
#include "adc.h"

uint8_t LineSensor_ReadRaw(void)
{
    uint8_t value = 0;

    if (DL_GPIO_readPins(GPIO_LINE_LINE_D0_PORT, GPIO_LINE_LINE_D0_PIN)) {
        value |= (1U << 0);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D1_PORT, GPIO_LINE_LINE_D1_PIN)) {
        value |= (1U << 1);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D2_PORT, GPIO_LINE_LINE_D2_PIN)) {
        value |= (1U << 2);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D3_PORT, GPIO_LINE_LINE_D3_PIN)) {
        value |= (1U << 3);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D4_PORT, GPIO_LINE_LINE_D4_PIN)) {
        value |= (1U << 4);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D5_PORT, GPIO_LINE_LINE_D5_PIN)) {
        value |= (1U << 5);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D6_PORT, GPIO_LINE_LINE_D6_PIN)) {
        value |= (1U << 6);
    }
    if (DL_GPIO_readPins(GPIO_LINE_LINE_D7_PORT, GPIO_LINE_LINE_D7_PIN)) {
        value |= (1U << 7);
    }

#if LINE_SENSOR_ACTIVE_LOW
    value = (uint8_t) (~value);
#endif

    value &= (uint8_t) ((1U << LINE_SENSOR_COUNT) - 1U);
    return value;
}

uint8_t LineSensor_GetActiveCount(void)
{
    uint8_t raw = LineSensor_ReadRaw();
    uint8_t activeCount = 0;
    uint8_t i;

    for (i = 0; i < LINE_SENSOR_COUNT; i++) {
        if ((raw & (1U << i)) != 0U) {
            activeCount++;
        }
    }

    return activeCount;
}

int16_t LineSensor_GetError(void)
{
    static const int16_t weights[LINE_SENSOR_COUNT] = {
        -3500, -2500, -1500, -500, 500, 1500, 2500, 3500
    };
    uint8_t raw = LineSensor_ReadRaw();
    int32_t weightedSum = 0;
    int16_t activeCount = 0;
    uint8_t i;

    for (i = 0; i < LINE_SENSOR_COUNT; i++) {
        if ((raw & (1U << i)) != 0U) {
#if LINE_SENSOR_REVERSE_ORDER
            weightedSum += weights[(LINE_SENSOR_COUNT - 1U) - i];
#else
            weightedSum += weights[i];
#endif
            activeCount++;
        }
    }

    if (activeCount == 0) {
        return 0;
    }

    return (int16_t) (weightedSum / activeCount);
}
