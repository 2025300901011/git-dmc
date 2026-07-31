$ErrorActionPreference = 'Stop'

$project = Split-Path -Parent $PSScriptRoot
$header = Join-Path $project 'inc\UART_Hand_PID.h'
$source = Join-Path $project 'src\UART_Hand_PID.c'
$controlHeader = Join-Path $project 'inc\control.h'
$controlSource = Join-Path $project 'src\control.c'
$mainSource = Join-Path $project 'src\main.c'
$syscfg = Join-Path $project 'empty.syscfg'

foreach ($file in @($header, $source, $controlHeader, $controlSource, $mainSource, $syscfg)) {
    if (-not (Test-Path -LiteralPath $file)) {
        throw "missing required file: $file"
    }
}

$uartHeaderText = Get-Content -LiteralPath $header -Raw
$uartSourceText = Get-Content -LiteralPath $source -Raw
$controlHeaderText = Get-Content -LiteralPath $controlHeader -Raw
$controlSourceText = Get-Content -LiteralPath $controlSource -Raw
$mainText = Get-Content -LiteralPath $mainSource -Raw
$syscfgText = Get-Content -LiteralPath $syscfg -Raw

$requirements = @(
    @($uartHeaderText, 'UART_Hand_PID_Service'),
    @($uartSourceText, 'SETALL'),
    @($uartSourceText, 'RUN'),
    @($uartSourceText, '"FORMAT"'),
    @($uartSourceText, '"CHECKSUM"'),
    @($uartSourceText, '"RANGE"'),
    @($uartSourceText, '"STATE"'),
    @($uartSourceText, '"BUSY"'),
    @($uartSourceText, 'isfinite'),
    @($uartSourceText, 'Control_LoadPipeMotorPid'),
    @($uartSourceText, 'Control_Enable(false)'),
    @($uartSourceText, '20000U'),
    @($controlHeaderText, 'Control_LoadPipeMotorPid'),
    @($controlHeaderText, 'Control_IsEnabled'),
    @($controlSourceText, 'PID_LoadConfig(&g_pid_pipe'),
    @($mainText, 'Control_Enable(false)'),
    @($mainText, 'UART_Hand_PID_Service(now_ms)'),
    @($syscfgText, 'UART_HAND_PID'),
    @($syscfgText, 'UART2'),
    @($syscfgText, 'PB15'),
    @($syscfgText, 'PB16'),
    @($syscfgText, 'targetBaudRate           = 115200')
)

foreach ($requirement in $requirements) {
    if (-not $requirement[0].Contains($requirement[1])) {
        throw "missing UART hand PID contract: $($requirement[1])"
    }
}

Write-Output 'UART hand PID static contract passed.'
