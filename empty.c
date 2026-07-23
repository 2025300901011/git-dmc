/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MPU6050_ADDR        (0x68)
#define MPU6050_WHO_AM_I    (0x75)
#define MPU6050_PWR_MGMT_1  (0x6B)
#define MPU6050_SMPLRT_DIV  (0x19)
#define MPU6050_CONFIG      (0x1A)
#define MPU6050_GYRO_CONFIG (0x1B)
#define MPU6050_ACCEL_CONFIG (0x1C)
#define MPU6050_ACCEL_XOUT_H (0x3B)

#define MPU6050_OK_WHO_AM_I (0x68)
#define MPU6050_ACCEL_LSB_PER_G (16384)
#define MPU6050_GYRO_LSB_PER_DPS (131)
#define I2C_TIMEOUT         (3200000U)
#define IMU_SAMPLE_PERIOD_S (0.01f)
#define IMU_SAMPLE_DELAY_CYCLES (320000U)
#define IMU_CALIBRATION_SAMPLES (500U)
#define IMU_ACCEL_LPF_ALPHA (0.20f)
#define IMU_COMPLEMENTARY_ALPHA (0.98f)
#define IMU_STILL_ACCEL_MIN_MG (850U)
#define IMU_STILL_ACCEL_MAX_MG (1150U)
#define IMU_STILL_GYRO_MAX_RAW (180)
#define IMU_GYRO_BIAS_TRIM_ALPHA (0.0005f)
#define RAD_TO_DEG (57.2957795f)

volatile uint8_t gWhoAmI;
volatile uint8_t gMpuRaw[14];
volatile int16_t gAccelX;
volatile int16_t gAccelY;
volatile int16_t gAccelZ;
volatile int16_t gTempRaw;
volatile int16_t gGyroX;
volatile int16_t gGyroY;
volatile int16_t gGyroZ;
volatile int32_t gAccelXmg;
volatile int32_t gAccelYmg;
volatile int32_t gAccelZmg;
volatile int32_t gGyroXdps;
volatile int32_t gGyroYdps;
volatile int32_t gGyroZdps;
volatile int32_t gTempCentiC;
volatile int32_t gGyroXBiasRaw;
volatile int32_t gGyroYBiasRaw;
volatile int32_t gGyroZBiasRaw;
volatile int32_t gPitchCdeg;
volatile int32_t gRollCdeg;
volatile int32_t gAccelPitchCdeg;
volatile int32_t gAccelRollCdeg;
volatile float gPitchDeg;
volatile float gRollDeg;
volatile float gAccelPitchDeg;
volatile float gAccelRollDeg;
volatile uint8_t gAccelConfig;
volatile uint8_t gGyroConfig;
volatile uint32_t gAccelMagnitudeSq;
volatile uint32_t gAccelTotalRaw;
volatile uint32_t gAccelTotalMg;
volatile uint32_t gI2cError;
volatile uint32_t gLoopCount;
volatile uint32_t gReadOkCount;
volatile uint32_t gReadFailCount;
volatile uint32_t gCalibrationSamplesDone;
volatile bool gLastReadOk;
volatile bool gImuCalibrating;
volatile bool gImuStill;
volatile bool gMpu6050ConfigOk;
volatile bool gMpu6050Online;

static float sAccelXmgFilt;
static float sAccelYmgFilt;
static float sAccelZmgFilt;
static float sPitchDeg;
static float sRollDeg;
static float sGyroXBiasRaw;
static float sGyroYBiasRaw;
static float sGyroZBiasRaw;

static void uart_putc(char c)
{
    DL_UART_transmitDataBlocking(UART_0_INST, (uint8_t) c);
}

static void uart_puts(const char *text)
{
    while (*text != '\0') {
        uart_putc(*text++);
    }
}

static void uart_print_sample(void)
{
    char line[160];
    int count = snprintf(line, sizeof(line),
        "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%d,%d\r\n",
        gAccelX, gAccelY, gAccelZ, gGyroX, gGyroY, gGyroZ, gTempRaw,
        gAccelXmg, gAccelYmg, gAccelZmg, gAccelTotalMg, gPitchCdeg,
        gRollCdeg);

    if (count > 0) {
        uart_puts(line);
    }
}

static bool i2c_wait_idle(void)
{
    uint32_t timeout = I2C_TIMEOUT;

    while (!(DL_I2C_getControllerStatus(I2C_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE)) {
        if (timeout-- == 0U) {
            gI2cError = 1U;
            return false;
        }
    }

    return true;
}

static bool i2c_wait_not_busy(void)
{
    uint32_t timeout = I2C_TIMEOUT;

    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY) {
        if (timeout-- == 0U) {
            gI2cError = 2U;
            return false;
        }
    }

    if (DL_I2C_getControllerStatus(I2C_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        gI2cError = DL_I2C_getControllerStatus(I2C_INST);
        return false;
    }

    return true;
}

static bool mpu6050_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t tx[2] = {reg, value};

    DL_I2C_flushControllerTXFIFO(I2C_INST);
    DL_I2C_flushControllerRXFIFO(I2C_INST);

    if (!i2c_wait_idle()) {
        return false;
    }

    DL_I2C_fillControllerTXFIFO(I2C_INST, tx, sizeof(tx));
    DL_I2C_startControllerTransfer(I2C_INST, MPU6050_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_TX, sizeof(tx));
    delay_cycles(1000);

    return i2c_wait_not_busy();
}

static bool mpu6050_read_regs(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t rxCount = 0U;
    uint32_t timeout;

    DL_I2C_flushControllerTXFIFO(I2C_INST);
    DL_I2C_flushControllerRXFIFO(I2C_INST);

    if (!i2c_wait_idle()) {
        return false;
    }

    DL_I2C_fillControllerTXFIFO(I2C_INST, &reg, 1U);
    DL_I2C_startControllerTransferAdvanced(I2C_INST, MPU6050_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_TX, 1U, DL_I2C_CONTROLLER_START_ENABLE,
        DL_I2C_CONTROLLER_STOP_DISABLE, DL_I2C_CONTROLLER_ACK_DISABLE);
    delay_cycles(1000);

    if (!i2c_wait_not_busy()) {
        return false;
    }

    DL_I2C_startControllerTransfer(I2C_INST, MPU6050_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_RX, len);
    delay_cycles(1000);

    timeout = I2C_TIMEOUT;
    while (rxCount < len) {
        if (!DL_I2C_isControllerRXFIFOEmpty(I2C_INST)) {
            data[rxCount++] = DL_I2C_receiveControllerData(I2C_INST);
            timeout = I2C_TIMEOUT;
        } else if (timeout-- == 0U) {
            gI2cError = 3U;
            return false;
        }
    }

    return i2c_wait_not_busy();
}

static int16_t make_i16(uint8_t high, uint8_t low)
{
    return (int16_t) (((uint16_t) high << 8) | low);
}

static int32_t accel_raw_to_mg(int16_t raw)
{
    return ((int32_t) raw * 1000L) / MPU6050_ACCEL_LSB_PER_G;
}

static int32_t gyro_raw_to_dps(int16_t raw)
{
    return ((int32_t) raw) / MPU6050_GYRO_LSB_PER_DPS;
}

static int32_t temp_raw_to_centi_c(int16_t raw)
{
    return (((int32_t) raw * 100L) / 340L) + 3653L;
}

static uint32_t isqrt_u32(uint32_t value)
{
    uint32_t result = 0U;
    uint32_t bit = 1UL << 30;

    while (bit > value) {
        bit >>= 2;
    }

    while (bit != 0U) {
        if (value >= result + bit) {
            value -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }

    return result;
}

static int32_t float_to_cdeg(float degrees)
{
    return (int32_t) ((degrees >= 0.0f) ? (degrees * 100.0f + 0.5f) :
                                      (degrees * 100.0f - 0.5f));
}

static bool mpu6050_init(void)
{
    if (!mpu6050_write_reg(MPU6050_PWR_MGMT_1, 0x80)) {
        return false;
    }
    delay_cycles(3200000);

    /* Use PLL as clock source after reset; leaving sleep mode is not enough on
     * some MPU6050 boards to get stable first readings. */
    if (!mpu6050_write_reg(MPU6050_PWR_MGMT_1, 0x01)) {
        return false;
    }
    delay_cycles(320000);

    if (!mpu6050_write_reg(MPU6050_SMPLRT_DIV, 0x07)) {
        return false;
    }

    if (!mpu6050_write_reg(MPU6050_CONFIG, 0x03)) {
        return false;
    }

    if (!mpu6050_write_reg(MPU6050_GYRO_CONFIG, 0x00)) {
        return false;
    }

    if (!mpu6050_write_reg(MPU6050_ACCEL_CONFIG, 0x00)) {
        return false;
    }

    if (!mpu6050_read_regs(MPU6050_WHO_AM_I, (uint8_t *) &gWhoAmI, 1U)) {
        return false;
    }

    if (!mpu6050_read_regs(
            MPU6050_ACCEL_CONFIG, (uint8_t *) &gAccelConfig, 1U)) {
        return false;
    }

    if (!mpu6050_read_regs(
            MPU6050_GYRO_CONFIG, (uint8_t *) &gGyroConfig, 1U)) {
        return false;
    }

    gMpu6050ConfigOk = (gWhoAmI == MPU6050_OK_WHO_AM_I) &&
                       (gAccelConfig == 0x00U) &&
                       (gGyroConfig == 0x00U);

    return gMpu6050ConfigOk;
}

static bool mpu6050_read_all(void)
{
    if (!mpu6050_read_regs(MPU6050_ACCEL_XOUT_H, (uint8_t *) gMpuRaw,
            sizeof(gMpuRaw))) {
        return false;
    }

    gAccelX  = make_i16(gMpuRaw[0], gMpuRaw[1]);
    gAccelY  = make_i16(gMpuRaw[2], gMpuRaw[3]);
    gAccelZ  = make_i16(gMpuRaw[4], gMpuRaw[5]);
    gTempRaw = make_i16(gMpuRaw[6], gMpuRaw[7]);
    gGyroX   = make_i16(gMpuRaw[8], gMpuRaw[9]);
    gGyroY   = make_i16(gMpuRaw[10], gMpuRaw[11]);
    gGyroZ   = make_i16(gMpuRaw[12], gMpuRaw[13]);
    gAccelXmg = accel_raw_to_mg(gAccelX);
    gAccelYmg = accel_raw_to_mg(gAccelY);
    gAccelZmg = accel_raw_to_mg(gAccelZ);
    gGyroXdps = gyro_raw_to_dps(gGyroX);
    gGyroYdps = gyro_raw_to_dps(gGyroY);
    gGyroZdps = gyro_raw_to_dps(gGyroZ);
    gTempCentiC = temp_raw_to_centi_c(gTempRaw);
    gAccelMagnitudeSq =
        (uint32_t) ((int32_t) gAccelX * (int32_t) gAccelX) +
        (uint32_t) ((int32_t) gAccelY * (int32_t) gAccelY) +
        (uint32_t) ((int32_t) gAccelZ * (int32_t) gAccelZ);
    gAccelTotalRaw = isqrt_u32(gAccelMagnitudeSq);
    gAccelTotalMg = (gAccelTotalRaw * 1000U) / MPU6050_ACCEL_LSB_PER_G;

    return true;
}

static bool imu_is_still(void)
{
    int32_t gyroX = (int32_t) gGyroX - gGyroXBiasRaw;
    int32_t gyroY = (int32_t) gGyroY - gGyroYBiasRaw;
    int32_t gyroZ = (int32_t) gGyroZ - gGyroZBiasRaw;

    if ((gAccelTotalMg < IMU_STILL_ACCEL_MIN_MG) ||
        (gAccelTotalMg > IMU_STILL_ACCEL_MAX_MG)) {
        return false;
    }

    return (gyroX > -IMU_STILL_GYRO_MAX_RAW) &&
           (gyroX < IMU_STILL_GYRO_MAX_RAW) &&
           (gyroY > -IMU_STILL_GYRO_MAX_RAW) &&
           (gyroY < IMU_STILL_GYRO_MAX_RAW) &&
           (gyroZ > -IMU_STILL_GYRO_MAX_RAW) &&
           (gyroZ < IMU_STILL_GYRO_MAX_RAW);
}

static bool imu_calibrate_stationary(void)
{
    int64_t gyroXSum = 0;
    int64_t gyroYSum = 0;
    int64_t gyroZSum = 0;
    uint32_t samples = 0U;

    gImuCalibrating = true;
    gCalibrationSamplesDone = 0U;

    while (samples < IMU_CALIBRATION_SAMPLES) {
        if (mpu6050_read_all()) {
            gyroXSum += gGyroX;
            gyroYSum += gGyroY;
            gyroZSum += gGyroZ;
            samples++;
            gCalibrationSamplesDone = samples;
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        } else {
            gReadFailCount++;
            gMpu6050Online = false;
            gImuCalibrating = false;
            return false;
        }

        delay_cycles(IMU_SAMPLE_DELAY_CYCLES);
    }

    sGyroXBiasRaw = (float) gyroXSum / (float) IMU_CALIBRATION_SAMPLES;
    sGyroYBiasRaw = (float) gyroYSum / (float) IMU_CALIBRATION_SAMPLES;
    sGyroZBiasRaw = (float) gyroZSum / (float) IMU_CALIBRATION_SAMPLES;
    gGyroXBiasRaw = (int32_t) sGyroXBiasRaw;
    gGyroYBiasRaw = (int32_t) sGyroYBiasRaw;
    gGyroZBiasRaw = (int32_t) sGyroZBiasRaw;

    sAccelXmgFilt = (float) gAccelXmg;
    sAccelYmgFilt = (float) gAccelYmg;
    sAccelZmgFilt = (float) gAccelZmg;
    gAccelRollDeg = atan2f(sAccelYmgFilt, sAccelZmgFilt) * RAD_TO_DEG;
    gAccelPitchDeg = atan2f(-sAccelXmgFilt,
        sqrtf((sAccelYmgFilt * sAccelYmgFilt) +
              (sAccelZmgFilt * sAccelZmgFilt))) * RAD_TO_DEG;
    sRollDeg = gAccelRollDeg;
    sPitchDeg = gAccelPitchDeg;
    gRollDeg = sRollDeg;
    gPitchDeg = sPitchDeg;
    gRollCdeg = float_to_cdeg(gRollDeg);
    gPitchCdeg = float_to_cdeg(gPitchDeg);
    gAccelRollCdeg = float_to_cdeg(gAccelRollDeg);
    gAccelPitchCdeg = float_to_cdeg(gAccelPitchDeg);
    gImuCalibrating = false;

    return true;
}

static void imu_update_filters(float dt)
{
    float gyroXRateDps = ((float) gGyroX - sGyroXBiasRaw) /
                         (float) MPU6050_GYRO_LSB_PER_DPS;
    float gyroYRateDps = ((float) gGyroY - sGyroYBiasRaw) /
                         (float) MPU6050_GYRO_LSB_PER_DPS;
    float gyroZRateDps = ((float) gGyroZ - sGyroZBiasRaw) /
                         (float) MPU6050_GYRO_LSB_PER_DPS;

    sAccelXmgFilt += IMU_ACCEL_LPF_ALPHA *
                     ((float) gAccelXmg - sAccelXmgFilt);
    sAccelYmgFilt += IMU_ACCEL_LPF_ALPHA *
                     ((float) gAccelYmg - sAccelYmgFilt);
    sAccelZmgFilt += IMU_ACCEL_LPF_ALPHA *
                     ((float) gAccelZmg - sAccelZmgFilt);

    gAccelRollDeg = atan2f(sAccelYmgFilt, sAccelZmgFilt) * RAD_TO_DEG;
    gAccelPitchDeg = atan2f(-sAccelXmgFilt,
        sqrtf((sAccelYmgFilt * sAccelYmgFilt) +
              (sAccelZmgFilt * sAccelZmgFilt))) * RAD_TO_DEG;

    sRollDeg = IMU_COMPLEMENTARY_ALPHA * (sRollDeg + gyroXRateDps * dt) +
               (1.0f - IMU_COMPLEMENTARY_ALPHA) * gAccelRollDeg;
    sPitchDeg = IMU_COMPLEMENTARY_ALPHA * (sPitchDeg + gyroYRateDps * dt) +
                (1.0f - IMU_COMPLEMENTARY_ALPHA) * gAccelPitchDeg;

    gImuStill = imu_is_still();
    if (gImuStill) {
        sGyroXBiasRaw += IMU_GYRO_BIAS_TRIM_ALPHA *
                         ((float) gGyroX - sGyroXBiasRaw);
        sGyroYBiasRaw += IMU_GYRO_BIAS_TRIM_ALPHA *
                         ((float) gGyroY - sGyroYBiasRaw);
        sGyroZBiasRaw += IMU_GYRO_BIAS_TRIM_ALPHA *
                         ((float) gGyroZ - sGyroZBiasRaw);
    }

    gGyroXBiasRaw = (int32_t) sGyroXBiasRaw;
    gGyroYBiasRaw = (int32_t) sGyroYBiasRaw;
    gGyroZBiasRaw = (int32_t) sGyroZBiasRaw;
    gGyroXdps = (int32_t) gyroXRateDps;
    gGyroYdps = (int32_t) gyroYRateDps;
    gGyroZdps = (int32_t) gyroZRateDps;
    gRollDeg = sRollDeg;
    gPitchDeg = sPitchDeg;
    gRollCdeg = float_to_cdeg(gRollDeg);
    gPitchCdeg = float_to_cdeg(gPitchDeg);
    gAccelRollCdeg = float_to_cdeg(gAccelRollDeg);
    gAccelPitchCdeg = float_to_cdeg(gAccelPitchDeg);
}

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    delay_cycles(160000);
    uart_puts("ax,ay,az,gx,gy,gz,temp,ax_mg,ay_mg,az_mg,total_mg,pitch_cdeg,roll_cdeg\r\n");

    delay_cycles(3200000);

    gMpu6050Online = mpu6050_init();
    if (gMpu6050Online) {
        gMpu6050Online = imu_calibrate_stationary();
    }

    while (1) {
        gLoopCount++;

        if (!gMpu6050Online) {
            gMpu6050Online = mpu6050_init();
        }

        if (gMpu6050Online && mpu6050_read_all()) {
            gLastReadOk = true;
            gReadOkCount++;
            imu_update_filters(IMU_SAMPLE_PERIOD_S);
            uart_print_sample();
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        } else {
            gLastReadOk = false;
            gReadFailCount++;
            gMpu6050Online = false;
            DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }

        delay_cycles(IMU_SAMPLE_DELAY_CYCLES);
    }
}
