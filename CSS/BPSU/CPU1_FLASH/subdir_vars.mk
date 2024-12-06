################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28003x_generic_flash_lnk.cmd \
../f28003x_headers_nonBIOS.cmd 

SYSCFG_SRCS += \
../c2000.syscfg 

LIB_SRCS += \
D:/Programs/TI/c2000/C2000Ware_5_04_00_00/driverlib/f28003x/driverlib/ccs/Debug/driverlib.lib 

C_SRCS += \
../MUCI_bsp.c \
../Term_defs.c \
./syscfg/board.c \
./syscfg/c2000ware_libraries.c \
../main.c 

GEN_FILES += \
./syscfg/board.c \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt \
./syscfg/c2000ware_libraries.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./MUCI_bsp.d \
./Term_defs.d \
./syscfg/board.d \
./syscfg/c2000ware_libraries.d \
./main.d 

GEN_OPTS += \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt 

OBJS += \
./MUCI_bsp.obj \
./Term_defs.obj \
./syscfg/board.obj \
./syscfg/c2000ware_libraries.obj \
./main.obj 

GEN_MISC_FILES += \
./syscfg/board.h \
./syscfg/board.cmd.genlibs \
./syscfg/board.json \
./syscfg/pinmux.csv \
./syscfg/epwm.dot \
./syscfg/adc.dot \
./syscfg/c2000ware_libraries.cmd.genlibs \
./syscfg/c2000ware_libraries.h \
./syscfg/clocktree.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"MUCI_bsp.obj" \
"Term_defs.obj" \
"syscfg\board.obj" \
"syscfg\c2000ware_libraries.obj" \
"main.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\board.h" \
"syscfg\board.cmd.genlibs" \
"syscfg\board.json" \
"syscfg\pinmux.csv" \
"syscfg\epwm.dot" \
"syscfg\adc.dot" \
"syscfg\c2000ware_libraries.cmd.genlibs" \
"syscfg\c2000ware_libraries.h" \
"syscfg\clocktree.h" 

C_DEPS__QUOTED += \
"MUCI_bsp.d" \
"Term_defs.d" \
"syscfg\board.d" \
"syscfg\c2000ware_libraries.d" \
"main.d" 

GEN_FILES__QUOTED += \
"syscfg\board.c" \
"syscfg\board.opt" \
"syscfg\c2000ware_libraries.opt" \
"syscfg\c2000ware_libraries.c" 

C_SRCS__QUOTED += \
"../MUCI_bsp.c" \
"../Term_defs.c" \
"./syscfg/board.c" \
"./syscfg/c2000ware_libraries.c" \
"../main.c" 

SYSCFG_SRCS__QUOTED += \
"../c2000.syscfg" 


