# Normal PCB Pinmap Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make the balance-ball PID project use the normal vehicle PCB pins while preserving its control interfaces.

**Architecture:** SysConfig owns all physical assignments. The motor abstraction consumes generated PWM-index macros, and the encoder abstraction changes internally from QEI/polling to GPIO interrupt counting without changing its public API.

**Tech Stack:** MSPM0G3507, TI SysConfig, DriverLib, CCS/gmake, PowerShell static contract.

## Global Constraints

- Edit `empty.syscfg`, never generated SysConfig outputs.
- Keep OpenMV UART at 115200 and MPU6050 I2C unchanged.
- TB6612 STBY is hardware-fixed high and must have no MCU pin or code reference.
- Do not flash or run motors during validation.

---

### Task 1: Pinmap regression contract

**Files:**
- Create: `tests/verify_normal_pcb_pinmap.ps1`

**Interfaces:**
- Consumes: `empty.syscfg`, `src/motor.c`, `src/encoder.c`, `inc/hw_map.h`
- Produces: exit code 0 only when the normal-PBC mapping and source contracts agree.

- [ ] **Step 1: Write the failing test**

Require PB9/PB10/PB7/PB6, PB14/PA7, PB11/PB12/PB4/PB5, UART2 PB15/PB16, no `DRV_STBY`, no `QEI`, and generated CCP0/CCP2 mapping use.

- [ ] **Step 2: Run it to verify RED**

Run: `powershell -ExecutionPolicy Bypass -File tests/verify_normal_pcb_pinmap.ps1`

Expected: nonzero exit because the checkout still has PB0/PB1/PB8/PB9, QEI, and CCP1.

- [ ] **Step 3: Implement the smallest configuration/code change**

Complete Tasks 2 and 3 only after the expected RED result.

- [ ] **Step 4: Run the contract to verify GREEN**

Run: `powershell -ExecutionPolicy Bypass -File tests/verify_normal_pcb_pinmap.ps1`

Expected: `Normal PCB pinmap contract passed.`

### Task 2: SysConfig normal PCB allocation

**Files:**
- Modify: `empty.syscfg`

**Interfaces:**
- Produces: `GPIO_TB6612_CTRL`, `PWM_TB6612` CCP0/CCP2, `GPIO_ENCODER`, and `UART_OPENMV` macros during SysConfig generation.

- [ ] **Step 1: Change only assignments**

Set direction pins PB9/PB10/PB7/PB6; configure TIMA0 CCP0 PB14 and CCP2 PA7; use PB11/PB12/PB4/PB5 as pulled-up GPIO encoder inputs with both A phases on rising/falling interrupts; set UART_OPENMV to UART2 PB15/PB16; remove QEI and STBY.

- [ ] **Step 2: Generate configuration and inspect macros**

Run: `C:\Ti\sysconfig_1.26.2\sysconfig_cli.bat -s C:\Ti\ti\mspm0_sdk_2_11_00_07\.metadata\product.json --script empty.syscfg -o . --compiler ticlang`

Expected: generated header exposes TIMA0, `GPIO_PWM_TB6612_C0_IDX`, `GPIO_PWM_TB6612_C2_IDX`, GPIOB encoder interrupt definitions, and UART2.

### Task 3: Preserve motor and encoder interfaces

**Files:**
- Modify: `inc/hw_map.h`
- Modify: `src/motor.c`
- Modify: `src/encoder.c`

**Interfaces:**
- Consumes: generated SysConfig macros.
- Produces: unchanged `Motor_*` and `Encoder_*` APIs.

- [ ] **Step 1: Make motor output channel-aware**

Use `GPIO_PWM_TB6612_C0_IDX` and `GPIO_PWM_TB6612_C2_IDX` through named `hw_map.h` aliases; use the TIMA DriverLib API; remove the STBY set operation.

- [ ] **Step 2: Replace encoder internals**

Count left/right A-phase edges in `GROUP1_IRQHandler`, read the corresponding B phases for direction, and calculate existing per-second filtered feedback in `Encoder_UpdateSpeed(uint32_t dt_ms)`. Keep `Encoder_PollGpio()` callable as a no-op.

- [ ] **Step 3: Verify build**

Run: `& 'C:\ti\ccs\utils\bin\gmake.exe' -C Debug -B all`

Expected: exit code 0 and updated `Debug\ball PID.out`; no flash step.

### Task 4: Final evidence

- [ ] **Step 1: Run static check**

Run: `python E:\TI_work\TI_Project\.codex\skills\mspm0-ccs\scripts\check_syscfg.py E:\TI_work\TI_Project\git-dmc-balance-ball-PID`

- [ ] **Step 2: Run contract and build again**

Run the Task 1 contract and Task 3 build commands after all source changes.

- [ ] **Step 3: Review diff and commit each affected repository**

Run `git diff --check` and commit the root configuration/documentation plus the affected `src`, `inc`, and `tests` repositories separately.
