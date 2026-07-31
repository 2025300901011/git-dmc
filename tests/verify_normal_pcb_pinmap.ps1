param(
    [string]$ProjectRoot = (Split-Path -Parent $PSScriptRoot)
)

$ErrorActionPreference = 'Stop'

$syscfg = Get-Content -Raw (Join-Path $ProjectRoot 'empty.syscfg')
$motor = Get-Content -Raw (Join-Path $ProjectRoot 'src\motor.c')
$encoder = Get-Content -Raw (Join-Path $ProjectRoot 'src\encoder.c')
$hwMap = Get-Content -Raw (Join-Path $ProjectRoot 'inc\hw_map.h')

function Require-Match([string]$Text, [string]$Pattern, [string]$Name) {
    if ($Text -notmatch $Pattern) { throw "Missing $Name" }
}

function Require-NoMatch([string]$Text, [string]$Pattern, [string]$Name) {
    if ($Text -match $Pattern) { throw "Unexpected $Name" }
}

Require-Match $syscfg 'M1_AIN1[\s\S]*assignedPin\s*=\s*"9"' 'M1 AIN1 PB9'
Require-Match $syscfg 'M1_AIN2[\s\S]*assignedPin\s*=\s*"10"' 'M1 AIN2 PB10'
Require-Match $syscfg 'M2_BIN1[\s\S]*assignedPin\s*=\s*"7"' 'M2 BIN1 PB7'
Require-Match $syscfg 'M2_BIN2[\s\S]*assignedPin\s*=\s*"6"' 'M2 BIN2 PB6'
Require-Match $syscfg 'ccp0Pin\.\$assign\s*=\s*"PB14"' 'PWMA PB14'
Require-Match $syscfg 'ccp2Pin\.\$assign\s*=\s*"PA7"' 'PWMB PA7'
Require-Match $syscfg 'ccIndex\s*=\s*\[0,2\]' 'PWM CCP0/CCP2 channels'
Require-Match $syscfg 'ENCODER_LEFT_A[\s\S]*assignedPin\s*=\s*"11"' 'left encoder A PB11'
Require-Match $syscfg 'ENCODER_LEFT_B[\s\S]*assignedPin\s*=\s*"12"' 'left encoder B PB12'
Require-Match $syscfg 'ENCODER_RIGHT_A[\s\S]*assignedPin\s*=\s*"4"' 'right encoder A PB4'
Require-Match $syscfg 'ENCODER_RIGHT_B[\s\S]*assignedPin\s*=\s*"5"' 'right encoder B PB5'
Require-Match $syscfg 'UART1\.peripheral\.\$assign\s*=\s*"UART0"' 'OpenMV UART0'
Require-Match $syscfg 'UART1\.peripheral\.txPin\.\$assign\s*=\s*"PA10"' 'OpenMV TX PA10'
Require-Match $syscfg 'UART1\.peripheral\.rxPin\.\$assign\s*=\s*"PA11"' 'OpenMV RX PA11'
Require-Match $syscfg 'UART2\.peripheral\.\$assign\s*=\s*"UART2"' 'hand PID UART2'
Require-Match $syscfg 'UART2\.peripheral\.txPin\.\$assign\s*=\s*"PB15"' 'hand PID TX PB15'
Require-Match $syscfg 'UART2\.peripheral\.rxPin\.\$assign\s*=\s*"PB16"' 'hand PID RX PB16'
Require-NoMatch $syscfg 'DRV_STBY|QEI' 'MCU STBY or QEI configuration'
Require-NoMatch $motor 'DRV_STBY' 'MCU STBY write'
Require-NoMatch $encoder 'QEI_ENC1|DL_Timer_getTimerCount' 'QEI encoder implementation'
Require-Match $motor 'PWM_TB6612_M1_CC_INDEX' 'motor A generated channel mapping'
Require-Match $motor 'PWM_TB6612_M2_CC_INDEX' 'motor B generated channel mapping'
Require-Match $hwMap 'GPIO_PWM_TB6612_C2_IDX' 'CCP2 generated macro alias'

Write-Output 'Normal PCB pinmap contract passed.'
