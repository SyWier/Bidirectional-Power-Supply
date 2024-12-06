################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device/driverlib" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/DeviceSupport/include" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/MUCI" --include_path="D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-594137441: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/Programs/TI/ccs1281/ccs/utils/sysconfig_1.21.0/sysconfig_cli.bat" --script "C:/Users/Daniel/CodeComposer/UPS_MUCI/c2000.syscfg" -o "syscfg" -s "D:/Programs/TI/c2000/C2000Ware_5_04_00_00/.metadata/sdk.json" -d "F28003x" --package 100PZ --part F28003x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-594137441 ../c2000.syscfg
syscfg/board.h: build-594137441
syscfg/board.cmd.genlibs: build-594137441
syscfg/board.opt: build-594137441
syscfg/board.json: build-594137441
syscfg/pinmux.csv: build-594137441
syscfg/epwm.dot: build-594137441
syscfg/adc.dot: build-594137441
syscfg/c2000ware_libraries.cmd.genlibs: build-594137441
syscfg/c2000ware_libraries.opt: build-594137441
syscfg/c2000ware_libraries.c: build-594137441
syscfg/c2000ware_libraries.h: build-594137441
syscfg/clocktree.h: build-594137441
syscfg: build-594137441

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device/driverlib" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/DeviceSupport/include" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/MUCI" --include_path="D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


