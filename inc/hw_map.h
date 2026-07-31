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

/* Motor direction fallbacks if group macros are unavailable. */
#ifndef GPIO_TB6612_CTRL_PORT
#define GPIO_TB6612_CTRL_PORT               GPIOB
#define GPIO_TB6612_CTRL_M1_AIN1_PIN        DL_GPIO_PIN_9
#define GPIO_TB6612_CTRL_M1_AIN2_PIN        DL_GPIO_PIN_10
#define GPIO_TB6612_CTRL_M2_BIN1_PIN        DL_GPIO_PIN_7
#define GPIO_TB6612_CTRL_M2_BIN2_PIN        DL_GPIO_PIN_6
#endif

/* Button fallbacks if SysConfig group macros are unavailable. */
#ifndef GPIO_KEYS_PORT
#define GPIO_KEYS_PORT                      GPIOA
#define GPIO_KEYS_KEY1_PIN                  DL_GPIO_PIN_23
#define GPIO_KEYS_KEY2_PIN                  DL_GPIO_PIN_21
#define GPIO_KEYS_KEY4_PIN                  DL_GPIO_PIN_17
#endif

#ifndef GPIO_KEYS_KEY3_PORT
#define GPIO_KEYS_KEY3_PORT                 GPIOB
#define GPIO_KEYS_KEY3_PIN                  DL_GPIO_PIN_18
#endif

/* TIMA0 exposes normal-PCB PWMA on CCP0 and PWMB on CCP2. */
#ifndef PWM_TB6612_M1_CC_INDEX
#define PWM_TB6612_M1_CC_INDEX              GPIO_PWM_TB6612_C0_IDX
#endif

#ifndef PWM_TB6612_M2_CC_INDEX
#define PWM_TB6612_M2_CC_INDEX              GPIO_PWM_TB6612_C2_IDX
#endif

#endif /* HW_MAP_H */
