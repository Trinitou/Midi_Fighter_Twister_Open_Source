# Midi Fighter Twister - Open Source Firmware
The MF Twister Project makes use of the Atmel ASF extensions & LUFA Module.

## Compilation
To compile following symbols must be defined

DEBUG
F_USB=48000000UL
F_CPU=32000000UL
USE_LUFA_CONFIG_HEADER
ARCH=ARCH_XMEGA
BOARD=USER_BOARD

The compiler must also link libm

The project also requires a user board configuration header file, if this is missing add the following to conf_board.h

#define CONF_BOARD_ENABLE_USARTD0

## Installation
1. Connect the Midi Fighter Twister to your computer
2. Open the Midifighter Utility software (should automatically detect the device)
3. Tools -> Midifighter -> Load Custom Firmware -> For a Twister
4. Navigate to the Midi_Fighter_Twister.hex file and open it
5. Choose "Yes" to proceed
6. Wait until the firmware update process is completed
