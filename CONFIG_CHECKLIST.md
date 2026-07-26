# Ball PID Configuration Checklist (MSPM0G3507)

This file is for pre-coding configuration only.

## 1. SysConfig Peripheral Plan

Configure the following in `empty.syscfg` (GUI is recommended):

1. `UART` x1 (OpenMV)
- Baud: `115200`
- Mode: `8N1`
- Enable RX interrupt or DMA RX.

2. `TIMER` x2 (PWM for TB6612 PWMA/PWMB)
- PWM frequency: `20 kHz`
- Duty range mapped to `0..999` (see `MOTOR_PWM_MAX`).

3. `GPIO` x5 (TB6612 logic pins)
- `AIN1`, `AIN2`, `BIN1`, `BIN2`: output
- `STBY`: output, default high after init

4. `TIMER` x2 (encoder counting A/B channels)
- Use QEI/encoder mode if available.
- If unavailable, use GPIO interrupt on A/B and software decode.

5. `TIMER` x1 (control tick base)
- Base tick: `1 ms`
- Software divider:
  - Position loop: every `10 ms`
  - Velocity loop: every `2 ms`

6. Optional `I2C` x1 (MPU6050)
- `400 kHz`
- Polling or interrupt both acceptable.

## 2. Current Pin Allocation (Preliminary)

Already assigned in `empty.syscfg`:

- OpenMV UART TX -> MCU UART RX: `PA11` (`UART0_RX`)
- OpenMV UART RX -> MCU UART TX: `PA10` (`UART0_TX`)
- TB6612 `PWMA`: `PA12` (`TIMG0_CCP0`)
- TB6612 `PWMB`: `PA13` (`TIMG0_CCP1`)
- TB6612 `AIN1`: `PB0` (GPIO output)
- TB6612 `AIN2`: `PB1` (GPIO output)
- TB6612 `BIN1`: `PB8` (GPIO output)
- TB6612 `BIN2`: `PB9` (GPIO output)
- TB6612 `STBY`: `PB16` (GPIO output, init high)
- Encoder1 A/B (motor A): `PA29/PA30` (`QEI`, TIMG8)
- Encoder2 A/B (motor B): `PB6/PB7` (GPIO input, software decode)
- Optional MPU6050 `SCL/SDA`: `PB2/PB3` (`I2C`)

## 3. Project Compile Config

Already configured:

1. Include path added: `${PROJECT_ROOT}/inc`
2. Module headers created:
- `inc/app_config.h`
- `inc/control.h`
- `inc/encoder.h`
- `inc/motor.h`
- `inc/pid.h`
- `inc/safty.h`
- `inc/vision.h`

## 4. Parameter Source of Truth

Tune runtime constants only in:
- `inc/app_config.h`

Recommended first-pass values are already set for:
- PID defaults
- Loop rates
- Vision timeout
- PWM limits

## 5. Naming Note

Current file naming follows your existing `safty.*`.
If you want consistency with `safety`, rename both files together later to avoid include mismatch.
