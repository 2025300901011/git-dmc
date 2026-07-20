#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define LINE_SENSOR_COUNT      (8)
/* Set to 1 if the sensor outputs 0 when it detects the target line. */
#define LINE_SENSOR_ACTIVE_LOW (1)
/* Set to 1 if D0 is mounted on the car's right side instead of left side. */
#define LINE_SENSOR_REVERSE_ORDER (0)

uint8_t LineSensor_ReadRaw(void);
uint8_t LineSensor_GetActiveCount(void);
int16_t LineSensor_GetError(void);

#endif /* ADC_H */
