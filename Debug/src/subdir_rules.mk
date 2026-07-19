################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"E:/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"../syscfg_gen/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/l/workspace_ccstheia/TI-car" -I"C:/Users/l/workspace_ccstheia/TI-car/Debug" -I"E:/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"E:/mspm0_sdk_2_10_00_04/source" -I"C:/Users/l/workspace_ccstheia/TI-car/syscfg_gen" -I"C:/Users/l/workspace_ccstheia/TI-car/inc" -gdwarf-3 -Wall -MMD -MP -MF"src/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


