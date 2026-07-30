#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Control loop periods (milliseconds) */
#define CTRL_POS_LOOP_MS                 (10U)  /* 100 Hz */
#define CTRL_VEL_LOOP_MS                 (2U)   /* 500 Hz */

/* Control mode */
#define CONTROL_MODE_PIPE_BALANCE        (1U)

/* Motor/PWM limits */
#define MOTOR_PWM_MAX                    (999U)
#define MOTOR_PWM_MIN                    (0U)
#define MOTOR_PWM_DEADBAND               (40U)
#define MOTOR_PWM_PERIOD_COUNTS          (2000U)

/* Vision protocol (OpenMV UART) */
#define VISION_UART_BAUD                 (115200U)
#define VISION_FRAME_HEADER              (0xAAU)
#define VISION_FRAME_TAIL                (0x55U)
#define VISION_TIMEOUT_MS                (100U)

/* Image center and target (pixel coordinates) */
#define VISION_IMG_W                     (320.0f)
#define VISION_IMG_H                     (240.0f)
#define TARGET_X_DEFAULT                 (VISION_IMG_W * 0.5f)
#define TARGET_Y_DEFAULT                 (VISION_IMG_H * 0.5f)

/* Speed estimator */
#define ENCODER_SPEED_FILTER_ALPHA       (0.20f)

/* Position PID defaults (ball position -> speed reference) */
#define PID_POS_KP_DEFAULT               (0.60f)
#define PID_POS_KI_DEFAULT               (0.00f)
#define PID_POS_KD_DEFAULT               (0.10f)
#define PID_POS_OUT_MAX_DEFAULT          (300.0f)
#define PID_POS_I_MAX_DEFAULT            (120.0f)

/* Pipe balance mode: ball position -> rack/motor encoder position */
#define PIPE_USE_X_AXIS                  (1U)
#define PIPE_MOTOR_CH                    MOTOR_CH_A
#define PIPE_ENCODER_IDX                 (0U)
#define PIPE_TARGET_DEFAULT              TARGET_X_DEFAULT
#define PIPE_ENCODER_COUNTS_PER_PIXEL    (0.60f)
#define PIPE_TARGET_COUNT_MAX            (120.0f)
#define PIPE_TARGET_COUNT_SLEW_STEP      (2.0f)
#define PIPE_MOTOR_POS_KP_DEFAULT        (1.60f)
#define PIPE_MOTOR_POS_KI_DEFAULT        (0.00f)
#define PIPE_MOTOR_POS_KD_DEFAULT        (0.06f)
#define PIPE_PWM_MAX_DEFAULT             (260.0f)
#define PIPE_MOTOR_POS_I_MAX_DEFAULT     (120.0f)
#define PIPE_PWM_MIN_EFFECTIVE           (70.0f)
#define PIPE_TARGET_COUNT_TOLERANCE      (5.0f)
#define PIPE_CONTROL_INVERT              (0U)

/* Velocity PID defaults (speed reference -> PWM) */
#define PID_VEL_KP_DEFAULT               (1.20f)
#define PID_VEL_KI_DEFAULT               (0.20f)
#define PID_VEL_KD_DEFAULT               (0.00f)
#define PID_VEL_OUT_MAX_DEFAULT          (950.0f)
#define PID_VEL_I_MAX_DEFAULT            (400.0f)

/* Safety behavior */
#define SAFE_LOST_FRAME_STOP_MS          (120U)
#define SAFE_STARTUP_RAMP_MS             (300U)

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
