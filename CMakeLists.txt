cmake_minimum_required(VERSION 3.25)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr/avr-gcc.toolchain.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

project(Midi_Fighter_Twister C CXX ASM)

add_compile_definitions(
	F_USB=48000000UL
	F_CPU=32000000UL
	USE_LUFA_CONFIG_HEADER
	ARCH=ARCH_XMEGA
	BOARD=USER_BOARD
)

set(MCU_TYPE ATxmega128a4u)

file(GLOB_RECURSE c_sources src/*.c)
file(GLOB_RECURSE asm_sources src/*.s)

set_property(SOURCE ${asm_sources} APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")

# specify include toolchain file
add_avr_executable(${PROJECT_NAME}
	${MCU_TYPE} #specify MCU type as second argument
	src/main.c
	${c_sources}
	${asm_sources}
)

target_include_directories(${elf_file}
	PRIVATE src
	PRIVATE src/ASF
	PRIVATE src/ASF/common
	PRIVATE src/ASF/common/boards
	PRIVATE src/ASF/common/boards/user_board
	PRIVATE src/ASF/common/drivers
	PRIVATE src/ASF/common/drivers/nvm
	PRIVATE src/ASF/common/drivers/nvm/xmega
	PRIVATE src/ASF/common/services
	PRIVATE src/ASF/common/services/clock
	PRIVATE src/ASF/common/services/clock/xmega
	PRIVATE src/ASF/common/services/delay
	PRIVATE src/ASF/common/services/delay/xmega
	PRIVATE src/ASF/common/services/fifo
	PRIVATE src/ASF/common/services/hugemem
	PRIVATE src/ASF/common/services/hugemem/avr8
	PRIVATE src/ASF/common/services/hugemem/generic
	PRIVATE src/ASF/common/services/ioport
	PRIVATE src/ASF/common/services/ioport/xmega
	PRIVATE src/ASF/common/services/serial
	PRIVATE src/ASF/common/services/serial/xmega_usart
	PRIVATE src/ASF/common/services/sleepmgr
	PRIVATE src/ASF/common/services/sleepmgr/xmega
	PRIVATE src/ASF/common/utils
	PRIVATE src/ASF/common/utils/interrupt
	PRIVATE src/ASF/common/utils/make
	PRIVATE src/ASF/xmega
	PRIVATE src/ASF/xmega/drivers
	PRIVATE src/ASF/xmega/drivers/cpu
	PRIVATE src/ASF/xmega/drivers/dma
	PRIVATE src/ASF/xmega/drivers/nvm
	PRIVATE src/ASF/xmega/drivers/pmic
	PRIVATE src/ASF/xmega/drivers/sleep
	PRIVATE src/ASF/xmega/drivers/tc
	PRIVATE src/ASF/xmega/drivers/usart
	PRIVATE src/ASF/xmega/drivers/wdt
	PRIVATE src/ASF/xmega/utils
	PRIVATE src/ASF/xmega/utils/assembler
	PRIVATE src/ASF/xmega/utils/bit_handling
	PRIVATE src/ASF/xmega/utils/preprocessor
	PRIVATE src/config
	PRIVATE src/LUFA
	PRIVATE src/LUFA/LUFA
	PRIVATE src/LUFA/LUFA/Common
	PRIVATE src/LUFA/LUFA/Drivers
	PRIVATE src/LUFA/LUFA/Drivers/USB
	PRIVATE src/LUFA/LUFA/Drivers/USB/Class
	PRIVATE src/LUFA/LUFA/Drivers/USB/Class/Common
	PRIVATE src/LUFA/LUFA/Drivers/USB/Class/Device
	PRIVATE src/LUFA/LUFA/Drivers/USB/Class/Host
	PRIVATE src/LUFA/LUFA/Drivers/USB/Core
	PRIVATE src/LUFA/LUFA/Drivers/USB/Core/XMEGA
	PRIVATE src/LUFA/LUFA/Drivers/USB/Core/XMEGA/Template
	PRIVATE src/LUFA/LUFA/Platform
	PRIVATE src/LUFA/LUFA/Platform/XMEGA
)
