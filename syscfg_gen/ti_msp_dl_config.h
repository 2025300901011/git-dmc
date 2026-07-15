/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_MOTOR */
#define PWM_MOTOR_INST                                                     TIMA1
#define PWM_MOTOR_INST_IRQHandler                               TIMA1_IRQHandler
#define PWM_MOTOR_INST_INT_IRQN                                 (TIMA1_INT_IRQn)
#define PWM_MOTOR_INST_CLK_FREQ                                         32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_MOTOR_C0_PORT                                             GPIOB
#define GPIO_PWM_MOTOR_C0_PIN                                      DL_GPIO_PIN_4
#define GPIO_PWM_MOTOR_C0_IOMUX                                  (IOMUX_PINCM17)
#define GPIO_PWM_MOTOR_C0_IOMUX_FUNC                 IOMUX_PINCM17_PF_TIMA1_CCP0
#define GPIO_PWM_MOTOR_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_MOTOR_C1_PORT                                             GPIOB
#define GPIO_PWM_MOTOR_C1_PIN                                      DL_GPIO_PIN_1
#define GPIO_PWM_MOTOR_C1_IOMUX                                  (IOMUX_PINCM13)
#define GPIO_PWM_MOTOR_C1_IOMUX_FUNC                 IOMUX_PINCM13_PF_TIMA1_CCP1
#define GPIO_PWM_MOTOR_C1_IDX                                DL_TIMER_CC_1_INDEX

/* Defines for PWM_SERVO */
#define PWM_SERVO_INST                                                     TIMG7
#define PWM_SERVO_INST_IRQHandler                               TIMG7_IRQHandler
#define PWM_SERVO_INST_INT_IRQN                                 (TIMG7_INT_IRQn)
#define PWM_SERVO_INST_CLK_FREQ                                         32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_SERVO_C0_PORT                                             GPIOA
#define GPIO_PWM_SERVO_C0_PIN                                     DL_GPIO_PIN_28
#define GPIO_PWM_SERVO_C0_IOMUX                                   (IOMUX_PINCM3)
#define GPIO_PWM_SERVO_C0_IOMUX_FUNC                  IOMUX_PINCM3_PF_TIMG7_CCP0
#define GPIO_PWM_SERVO_C0_IDX                                DL_TIMER_CC_0_INDEX



/* Defines for TIMER_CONTROL */
#define TIMER_CONTROL_INST                                               (TIMG0)
#define TIMER_CONTROL_INST_IRQHandler                           TIMG0_IRQHandler
#define TIMER_CONTROL_INST_INT_IRQN                             (TIMG0_INT_IRQn)
#define TIMER_CONTROL_INST_LOAD_VALUE                                     (327U)




/* Defines for I2C_MPU6050 */
#define I2C_MPU6050_INST                                                    I2C0
#define I2C_MPU6050_INST_IRQHandler                              I2C0_IRQHandler
#define I2C_MPU6050_INST_INT_IRQN                                  I2C0_INT_IRQn
#define I2C_MPU6050_BUS_SPEED_HZ                                          400000
#define GPIO_I2C_MPU6050_SDA_PORT                                          GPIOA
#define GPIO_I2C_MPU6050_SDA_PIN                                   DL_GPIO_PIN_0
#define GPIO_I2C_MPU6050_IOMUX_SDA                                (IOMUX_PINCM1)
#define GPIO_I2C_MPU6050_IOMUX_SDA_FUNC                 IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_MPU6050_SCL_PORT                                          GPIOA
#define GPIO_I2C_MPU6050_SCL_PIN                                   DL_GPIO_PIN_1
#define GPIO_I2C_MPU6050_IOMUX_SCL                                (IOMUX_PINCM2)
#define GPIO_I2C_MPU6050_IOMUX_SCL_FUNC                 IOMUX_PINCM2_PF_I2C0_SCL


/* Defines for UART_DEBUG */
#define UART_DEBUG_INST                                                    UART0
#define UART_DEBUG_INST_FREQUENCY                                       32000000
#define UART_DEBUG_INST_IRQHandler                              UART0_IRQHandler
#define UART_DEBUG_INST_INT_IRQN                                  UART0_INT_IRQn
#define GPIO_UART_DEBUG_RX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_TX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_RX_PIN                                    DL_GPIO_PIN_11
#define GPIO_UART_DEBUG_TX_PIN                                    DL_GPIO_PIN_10
#define GPIO_UART_DEBUG_IOMUX_RX                                 (IOMUX_PINCM22)
#define GPIO_UART_DEBUG_IOMUX_TX                                 (IOMUX_PINCM21)
#define GPIO_UART_DEBUG_IOMUX_RX_FUNC                  IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_DEBUG_IOMUX_TX_FUNC                  IOMUX_PINCM21_PF_UART0_TX
#define UART_DEBUG_BAUD_RATE                                            (115200)
#define UART_DEBUG_IBRD_32_MHZ_115200_BAUD                                  (17)
#define UART_DEBUG_FBRD_32_MHZ_115200_BAUD                                  (23)





/* Port definition for Pin Group GPIO_MOTOR */
#define GPIO_MOTOR_PORT                                                  (GPIOB)

/* Defines for MOTOR_AIN1: GPIOB.12 with pinCMx 29 on package pin 64 */
#define GPIO_MOTOR_MOTOR_AIN1_PIN                               (DL_GPIO_PIN_12)
#define GPIO_MOTOR_MOTOR_AIN1_IOMUX                              (IOMUX_PINCM29)
/* Defines for MOTOR_AIN2: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_MOTOR_MOTOR_AIN2_PIN                               (DL_GPIO_PIN_13)
#define GPIO_MOTOR_MOTOR_AIN2_IOMUX                              (IOMUX_PINCM30)
/* Defines for MOTOR_BIN1: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_MOTOR_MOTOR_BIN1_PIN                               (DL_GPIO_PIN_14)
#define GPIO_MOTOR_MOTOR_BIN1_IOMUX                              (IOMUX_PINCM31)
/* Defines for MOTOR_BIN2: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_MOTOR_MOTOR_BIN2_PIN                               (DL_GPIO_PIN_15)
#define GPIO_MOTOR_MOTOR_BIN2_IOMUX                              (IOMUX_PINCM32)
/* Defines for MOTOR_STBY: GPIOB.16 with pinCMx 33 on package pin 4 */
#define GPIO_MOTOR_MOTOR_STBY_PIN                               (DL_GPIO_PIN_16)
#define GPIO_MOTOR_MOTOR_STBY_IOMUX                              (IOMUX_PINCM33)
/* Defines for LINE_D0: GPIOA.2 with pinCMx 7 on package pin 42 */
#define GPIO_LINE_LINE_D0_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D0_PIN                                    (DL_GPIO_PIN_2)
#define GPIO_LINE_LINE_D0_IOMUX                                   (IOMUX_PINCM7)
/* Defines for LINE_D1: GPIOA.3 with pinCMx 8 on package pin 43 */
#define GPIO_LINE_LINE_D1_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D1_PIN                                    (DL_GPIO_PIN_3)
#define GPIO_LINE_LINE_D1_IOMUX                                   (IOMUX_PINCM8)
/* Defines for LINE_D2: GPIOA.4 with pinCMx 9 on package pin 44 */
#define GPIO_LINE_LINE_D2_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D2_PIN                                    (DL_GPIO_PIN_4)
#define GPIO_LINE_LINE_D2_IOMUX                                   (IOMUX_PINCM9)
/* Defines for LINE_D3: GPIOA.5 with pinCMx 10 on package pin 45 */
#define GPIO_LINE_LINE_D3_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D3_PIN                                    (DL_GPIO_PIN_5)
#define GPIO_LINE_LINE_D3_IOMUX                                  (IOMUX_PINCM10)
/* Defines for LINE_D4: GPIOA.6 with pinCMx 11 on package pin 46 */
#define GPIO_LINE_LINE_D4_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D4_PIN                                    (DL_GPIO_PIN_6)
#define GPIO_LINE_LINE_D4_IOMUX                                  (IOMUX_PINCM11)
/* Defines for LINE_D5: GPIOA.7 with pinCMx 14 on package pin 49 */
#define GPIO_LINE_LINE_D5_PORT                                           (GPIOA)
#define GPIO_LINE_LINE_D5_PIN                                    (DL_GPIO_PIN_7)
#define GPIO_LINE_LINE_D5_IOMUX                                  (IOMUX_PINCM14)
/* Defines for LINE_D6: GPIOB.8 with pinCMx 25 on package pin 60 */
#define GPIO_LINE_LINE_D6_PORT                                           (GPIOB)
#define GPIO_LINE_LINE_D6_PIN                                    (DL_GPIO_PIN_8)
#define GPIO_LINE_LINE_D6_IOMUX                                  (IOMUX_PINCM25)
/* Defines for LINE_D7: GPIOB.9 with pinCMx 26 on package pin 61 */
#define GPIO_LINE_LINE_D7_PORT                                           (GPIOB)
#define GPIO_LINE_LINE_D7_PIN                                    (DL_GPIO_PIN_9)
#define GPIO_LINE_LINE_D7_IOMUX                                  (IOMUX_PINCM26)
/* Port definition for Pin Group GPIO_ENCODER */
#define GPIO_ENCODER_PORT                                                (GPIOA)

/* Defines for ENCODER_LEFT_A: GPIOA.12 with pinCMx 34 on package pin 5 */
// pins affected by this interrupt request:["ENCODER_LEFT_A","ENCODER_RIGHT_A"]
#define GPIO_ENCODER_INT_IRQN                                   (GPIOA_INT_IRQn)
#define GPIO_ENCODER_INT_IIDX                   (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_ENCODER_ENCODER_LEFT_A_IIDX                    (DL_GPIO_IIDX_DIO12)
#define GPIO_ENCODER_ENCODER_LEFT_A_PIN                         (DL_GPIO_PIN_12)
#define GPIO_ENCODER_ENCODER_LEFT_A_IOMUX                        (IOMUX_PINCM34)
/* Defines for ENCODER_LEFT_B: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_ENCODER_ENCODER_LEFT_B_PIN                         (DL_GPIO_PIN_13)
#define GPIO_ENCODER_ENCODER_LEFT_B_IOMUX                        (IOMUX_PINCM35)
/* Defines for ENCODER_RIGHT_A: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_ENCODER_ENCODER_RIGHT_A_IIDX                   (DL_GPIO_IIDX_DIO14)
#define GPIO_ENCODER_ENCODER_RIGHT_A_PIN                        (DL_GPIO_PIN_14)
#define GPIO_ENCODER_ENCODER_RIGHT_A_IOMUX                       (IOMUX_PINCM36)
/* Defines for ENCODER_RIGHT_B: GPIOA.15 with pinCMx 37 on package pin 8 */
#define GPIO_ENCODER_ENCODER_RIGHT_B_PIN                        (DL_GPIO_PIN_15)
#define GPIO_ENCODER_ENCODER_RIGHT_B_IOMUX                       (IOMUX_PINCM37)
/* Port definition for Pin Group GPIO_UI */
#define GPIO_UI_PORT                                                     (GPIOB)

/* Defines for USER_KEY: GPIOB.21 with pinCMx 49 on package pin 20 */
// pins affected by this interrupt request:["USER_KEY"]
#define GPIO_UI_INT_IRQN                                        (GPIOB_INT_IRQn)
#define GPIO_UI_INT_IIDX                        (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_UI_USER_KEY_IIDX                               (DL_GPIO_IIDX_DIO21)
#define GPIO_UI_USER_KEY_PIN                                    (DL_GPIO_PIN_21)
#define GPIO_UI_USER_KEY_IOMUX                                   (IOMUX_PINCM49)
/* Defines for STATUS_LED: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_UI_STATUS_LED_PIN                                  (DL_GPIO_PIN_22)
#define GPIO_UI_STATUS_LED_IOMUX                                 (IOMUX_PINCM50)
/* Defines for BUZZER: GPIOB.20 with pinCMx 48 on package pin 19 */
#define GPIO_UI_BUZZER_PIN                                      (DL_GPIO_PIN_20)
#define GPIO_UI_BUZZER_IOMUX                                     (IOMUX_PINCM48)
/* Defines for ULTRASONIC_TRIG: GPIOB.23 with pinCMx 51 on package pin 22 */
#define GPIO_UI_ULTRASONIC_TRIG_PIN                             (DL_GPIO_PIN_23)
#define GPIO_UI_ULTRASONIC_TRIG_IOMUX                            (IOMUX_PINCM51)
/* Defines for ULTRASONIC_ECHO: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GPIO_UI_ULTRASONIC_ECHO_PIN                             (DL_GPIO_PIN_24)
#define GPIO_UI_ULTRASONIC_ECHO_IOMUX                            (IOMUX_PINCM52)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_MOTOR_init(void);
void SYSCFG_DL_PWM_SERVO_init(void);
void SYSCFG_DL_TIMER_CONTROL_init(void);
void SYSCFG_DL_I2C_MPU6050_init(void);
void SYSCFG_DL_UART_DEBUG_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
