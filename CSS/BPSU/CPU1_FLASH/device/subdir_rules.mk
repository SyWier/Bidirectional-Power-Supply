################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device/driverlib" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/DeviceSupport/include" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/MUCI" --include_path="D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/device/driverlib" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/DeviceSupport/include" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/MUCI" --include_path="D:/Programs/TI/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/Daniel/CodeComposer/UPS_MUCI/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


