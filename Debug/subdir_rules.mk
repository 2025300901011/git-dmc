################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-854838029: ../empty.syscfg
	@echo 'SysConfig - building file: "$<"'
	"E:/sysconfig_1.26.2/sysconfig_cli.bat" --script "C:/Users/l/workspace_ccstheia/TI-car/empty.syscfg" -o "../syscfg_gen" --product E:/mspm0_sdk_2_10_00_04/.metadata/product.json --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/device_linker.cmd: build-854838029 ../empty.syscfg
C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/device.opt: build-854838029
C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/device.cmd.genlibs: build-854838029
C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/ti_msp_dl_config.c: build-854838029
C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/ti_msp_dl_config.h: build-854838029
C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/Event.dot: build-854838029

syscfg_gen/ti_msp_dl_config.o: C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen/ti_msp_dl_config.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"E:/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"../syscfg_gen/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/l/workspace_ccstheia/TI-car" -I"C:/Users/l/workspace_ccstheia/TI-car/Debug" -I"E:/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"E:/mspm0_sdk_2_10_00_04/source" -I"C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen" -I"C:/Users/l/workspace_ccstheia/TI-car/inc" -gdwarf-3 -Wall -MMD -MP -MF"syscfg_gen/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: E:/mspm0_sdk_2_10_00_04/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"E:/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"../syscfg_gen/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/l/workspace_ccstheia/TI-car" -I"C:/Users/l/workspace_ccstheia/TI-car/Debug" -I"E:/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"E:/mspm0_sdk_2_10_00_04/source" -I"C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen" -I"C:/Users/l/workspace_ccstheia/TI-car/inc" -gdwarf-3 -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


