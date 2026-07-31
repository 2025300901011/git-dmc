# Normal PCB Pinmap Design

## Goal

Move the balance-ball PID firmware to the PCB wiring used by `Legacy\Clean_Car` without changing the OpenMV control, dual-motor PID, or encoder public interfaces.

## Hardware mapping

| Function | Normal PCB mapping | Implementation consequence |
| --- | --- | --- |
| TB6612 directions | AIN1 PB9, AIN2 PB10, BIN1 PB7, BIN2 PB6 | Keep the existing motor-direction API and rename only SysConfig bindings. |
| TB6612 PWM | PWMA TIMA0 CCP0 PB14, PWMB TIMA0 CCP2 PA7 | Use generated CCP0/CCP2 index macros; do not write CCP1. |
| TB6612 STBY | Hardware fixed high | Remove all MCU STBY references; motor stop continues to set both PWM outputs to zero and clears direction pins. |
| Encoder A | left A/B PB11/PB12; right A/B PB4/PB5 | Replace the QEI plus polling split with GPIO A-edge interrupts and B-phase direction reads. |
| OpenMV UART | UART2 TX PB15, RX PB16 at 115200 | Keep the `UART_OPENMV` SysConfig name so `vision.c` remains unchanged. |
| MPU6050 I2C | I2C1 SCL PB2, SDA PB3 | Keep unchanged. |

## Design

`empty.syscfg` remains the only pinmux source. It will define four motor GPIOs, two TIMA0 PWM channels (`CCP0` and `CCP2`), a four-pin encoder GPIO group with both A phases interrupt-enabled, UART2, and I2C1. Generated SysConfig files will not be edited.

`motor.c` will stop asserting STBY and will use the generated PWM channel-index macros so the right motor reaches CCP2. `encoder.c` retains its public API and reported units (filtered edge counts per second), but records both encoders with the known-good GPIO interrupt pattern from `Legacy\Clean_Car`; the legacy busy-loop polling call remains a harmless no-op so the main-loop structure is unchanged. The normal PCB's right-encoder polarity is retained as reversed, matching the existing vehicle wiring convention.

## Validation

A PowerShell static contract test will first fail against the old pin assignment, then require the final SysConfig and source to agree on direction pins, PWM pins/channel indices, encoder pins/interrupts, UART2, no STBY macro, and no QEI. SysConfig generation and the existing CCS `gmake -C Debug -B all` build will provide source/build evidence only. No flashing or hardware actuation is included.
