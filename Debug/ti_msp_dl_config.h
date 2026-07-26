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



/* Defines for PWM_TB6612 */
#define PWM_TB6612_INST                                                    TIMG0
#define PWM_TB6612_INST_IRQHandler                              TIMG0_IRQHandler
#define PWM_TB6612_INST_INT_IRQN                                (TIMG0_INT_IRQn)
#define PWM_TB6612_INST_CLK_FREQ                                        32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_TB6612_C0_PORT                                            GPIOA
#define GPIO_PWM_TB6612_C0_PIN                                    DL_GPIO_PIN_12
#define GPIO_PWM_TB6612_C0_IOMUX                                 (IOMUX_PINCM34)
#define GPIO_PWM_TB6612_C0_IOMUX_FUNC                IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_TB6612_C0_IDX                               DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_TB6612_C1_PORT                                            GPIOA
#define GPIO_PWM_TB6612_C1_PIN                                    DL_GPIO_PIN_13
#define GPIO_PWM_TB6612_C1_IOMUX                                 (IOMUX_PINCM35)
#define GPIO_PWM_TB6612_C1_IOMUX_FUNC                IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_TB6612_C1_IDX                               DL_TIMER_CC_1_INDEX




/* Defines for QEI_ENC1 */
#define QEI_ENC1_INST                                                      TIMG8
#define QEI_ENC1_INST_IRQHandler                                TIMG8_IRQHandler
#define QEI_ENC1_INST_INT_IRQN                                  (TIMG8_INT_IRQn)
/* Pin configuration defines for QEI_ENC1 PHA Pin */
#define GPIO_QEI_ENC1_PHA_PORT                                             GPIOA
#define GPIO_QEI_ENC1_PHA_PIN                                     DL_GPIO_PIN_29
#define GPIO_QEI_ENC1_PHA_IOMUX                                   (IOMUX_PINCM4)
#define GPIO_QEI_ENC1_PHA_IOMUX_FUNC                  IOMUX_PINCM4_PF_TIMG8_CCP0
/* Pin configuration defines for QEI_ENC1 PHB Pin */
#define GPIO_QEI_ENC1_PHB_PORT                                             GPIOA
#define GPIO_QEI_ENC1_PHB_PIN                                     DL_GPIO_PIN_30
#define GPIO_QEI_ENC1_PHB_IOMUX                                   (IOMUX_PINCM5)
#define GPIO_QEI_ENC1_PHB_IOMUX_FUNC                  IOMUX_PINCM5_PF_TIMG8_CCP1


/* Defines for TIMER_CTRL_TICK */
#define TIMER_CTRL_TICK_INST                                             (TIMG6)
#define TIMER_CTRL_TICK_INST_IRQHandler                         TIMG6_IRQHandler
#define TIMER_CTRL_TICK_INST_INT_IRQN                           (TIMG6_INT_IRQn)
#define TIMER_CTRL_TICK_INST_LOAD_VALUE                                 (62499U)




/* Defines for I2C_MPU6050 */
#define I2C_MPU6050_INST                                                    I2C1
#define I2C_MPU6050_INST_IRQHandler                              I2C1_IRQHandler
#define I2C_MPU6050_INST_INT_IRQN                                  I2C1_INT_IRQn
#define I2C_MPU6050_BUS_SPEED_HZ                                          400000
#define GPIO_I2C_MPU6050_SDA_PORT                                          GPIOB
#define GPIO_I2C_MPU6050_SDA_PIN                                   DL_GPIO_PIN_3
#define GPIO_I2C_MPU6050_IOMUX_SDA                               (IOMUX_PINCM16)
#define GPIO_I2C_MPU6050_IOMUX_SDA_FUNC                IOMUX_PINCM16_PF_I2C1_SDA
#define GPIO_I2C_MPU6050_SCL_PORT                                          GPIOB
#define GPIO_I2C_MPU6050_SCL_PIN                                   DL_GPIO_PIN_2
#define GPIO_I2C_MPU6050_IOMUX_SCL                               (IOMUX_PINCM15)
#define GPIO_I2C_MPU6050_IOMUX_SCL_FUNC                IOMUX_PINCM15_PF_I2C1_SCL


/* Defines for UART_OPENMV */
#define UART_OPENMV_INST                                                   UART0
#define UART_OPENMV_INST_FREQUENCY                                      32000000
#define UART_OPENMV_INST_IRQHandler                             UART0_IRQHandler
#define UART_OPENMV_INST_INT_IRQN                                 UART0_INT_IRQn
#define GPIO_UART_OPENMV_RX_PORT                                           GPIOA
#define GPIO_UART_OPENMV_TX_PORT                                           GPIOA
#define GPIO_UART_OPENMV_RX_PIN                                   DL_GPIO_PIN_11
#define GPIO_UART_OPENMV_TX_PIN                                   DL_GPIO_PIN_10
#define GPIO_UART_OPENMV_IOMUX_RX                                (IOMUX_PINCM22)
#define GPIO_UART_OPENMV_IOMUX_TX                                (IOMUX_PINCM21)
#define GPIO_UART_OPENMV_IOMUX_RX_FUNC                 IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_OPENMV_IOMUX_TX_FUNC                 IOMUX_PINCM21_PF_UART0_TX
#define UART_OPENMV_BAUD_RATE                                           (115200)
#define UART_OPENMV_IBRD_32_MHZ_115200_BAUD                                 (17)
#define UART_OPENMV_FBRD_32_MHZ_115200_BAUD                                 (23)





/* Port definition for Pin Group GPIO_TB6612_CTRL */
#define GPIO_TB6612_CTRL_PORT                                            (GPIOB)

/* Defines for M1_AIN1: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_TB6612_CTRL_M1_AIN1_PIN                             (DL_GPIO_PIN_0)
#define GPIO_TB6612_CTRL_M1_AIN1_IOMUX                           (IOMUX_PINCM12)
/* Defines for M1_AIN2: GPIOB.1 with pinCMx 13 on package pin 48 */
#define GPIO_TB6612_CTRL_M1_AIN2_PIN                             (DL_GPIO_PIN_1)
#define GPIO_TB6612_CTRL_M1_AIN2_IOMUX                           (IOMUX_PINCM13)
/* Defines for M2_BIN1: GPIOB.8 with pinCMx 25 on package pin 60 */
#define GPIO_TB6612_CTRL_M2_BIN1_PIN                             (DL_GPIO_PIN_8)
#define GPIO_TB6612_CTRL_M2_BIN1_IOMUX                           (IOMUX_PINCM25)
/* Defines for M2_BIN2: GPIOB.9 with pinCMx 26 on package pin 61 */
#define GPIO_TB6612_CTRL_M2_BIN2_PIN                             (DL_GPIO_PIN_9)
#define GPIO_TB6612_CTRL_M2_BIN2_IOMUX                           (IOMUX_PINCM26)
/* Defines for DRV_STBY: GPIOB.16 with pinCMx 33 on package pin 4 */
#define GPIO_TB6612_CTRL_DRV_STBY_PIN                           (DL_GPIO_PIN_16)
#define GPIO_TB6612_CTRL_DRV_STBY_IOMUX                          (IOMUX_PINCM33)
/* Port definition for Pin Group GPIO_ENC2_INPUT */
#define GPIO_ENC2_INPUT_PORT                                             (GPIOB)

/* Defines for ENC2_A: GPIOB.6 with pinCMx 23 on package pin 58 */
#define GPIO_ENC2_INPUT_ENC2_A_PIN                               (DL_GPIO_PIN_6)
#define GPIO_ENC2_INPUT_ENC2_A_IOMUX                             (IOMUX_PINCM23)
/* Defines for ENC2_B: GPIOB.7 with pinCMx 24 on package pin 59 */
#define GPIO_ENC2_INPUT_ENC2_B_PIN                               (DL_GPIO_PIN_7)
#define GPIO_ENC2_INPUT_ENC2_B_IOMUX                             (IOMUX_PINCM24)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_TB6612_init(void);
void SYSCFG_DL_QEI_ENC1_init(void);
void SYSCFG_DL_TIMER_CTRL_TICK_init(void);
void SYSCFG_DL_I2C_MPU6050_init(void);
void SYSCFG_DL_UART_OPENMV_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
