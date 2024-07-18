################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/jenda/ti/ccstheia131/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/jenda/Bakalarka/src" -I"/home/jenda/Bakalarka/src/Debug" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1277612689: ../main.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/home/jenda/ti/ccstheia131/ccs/utils/sysconfig_1.19.0/sysconfig_cli.sh" --script "/home/jenda/Bakalarka/src/main.syscfg" -o "." -s "/home/jenda/ti/mspm0_sdk_2_00_01_00/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1277612689 ../main.syscfg
device.opt: build-1277612689
device.cmd.genlibs: build-1277612689
ti_msp_dl_config.c: build-1277612689
ti_msp_dl_config.h: build-1277612689
Event.dot: build-1277612689

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/jenda/ti/ccstheia131/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/jenda/Bakalarka/src" -I"/home/jenda/Bakalarka/src/Debug" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: /home/jenda/ti/mspm0_sdk_2_00_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/jenda/ti/ccstheia131/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/jenda/Bakalarka/src" -I"/home/jenda/Bakalarka/src/Debug" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"/home/jenda/ti/mspm0_sdk_2_00_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


