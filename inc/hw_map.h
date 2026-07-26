#ifndef HW_MAP_H
#define HW_MAP_H

#include "ti_msp_dl_config.h"

/* UART mapping */
#ifndef UART_OPENMV_INST
#if defined(UART_OPENMV_INST)
#elif defined(UART_0_INST)
#define UART_OPENMV_INST UART_0_INST
#else
#error "UART instance macro not found from SysConfig."
#endif
#endif

/* PWM mapping */
#ifndef PWM_TB6612_INST
#if defined(PWM_TB6612_INST)
#elif defined(PWM_0_INST)
#define PWM_TB6612_INST PWM_0_INST
#else
#error "PWM instance macro not found from SysConfig."
#endif
#endif

/* QEI mapping */
#ifndef QEI_ENC1_INST
#if defined(QEI_ENC1_INST)
#elif defined(QEI_0_INST)
#define QEI_ENC1_INST QEI_0_INST
#else
#error "QEI instance macro not found from SysConfig."
#endif
#endif

/* Port/pin fallbacks if group macros are unavailable */
#ifndef GPIO_TB6612_CTRL_PORT
#define GPIO_TB6612_CTRL_PORT               GPIOB
#define GPIO_TB6612_CTRL_M1_AIN1_PIN        DL_GPIO_PIN_0
#define GPIO_TB6612_CTRL_M1_AIN2_PIN        DL_GPIO_PIN_1
#define GPIO_TB6612_CTRL_M2_BIN1_PIN        DL_GPIO_PIN_8
#define GPIO_TB6612_CTRL_M2_BIN2_PIN        DL_GPIO_PIN_9
#define GPIO_TB6612_CTRL_DRV_STBY_PIN       DL_GPIO_PIN_16
#endif

#ifndef GPIO_ENC2_INPUT_PORT
#define GPIO_ENC2_INPUT_PORT                GPIOB
#define GPIO_ENC2_INPUT_ENC2_A_PIN          DL_GPIO_PIN_6
#define GPIO_ENC2_INPUT_ENC2_B_PIN          DL_GPIO_PIN_7
#endif

#endif /* HW_MAP_H */
