###############################################################################
# Makefile for the CDC project
###############################################################################

## General Flags
PROJECT = cdc
MCU = at90usb1287
TARGET = $(PROJECT).elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -D AVRGCC -Wall -gdwarf-2  -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Tat90usbxxx.x
LDFLAGS += -Wl,-Map=$(PROJECT).map,--cref

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Include Directories
INCLUDES = -I"./.." -I"../conf" -I"../../../../at90usb128" -I"../../../../common" 

## Objects that must be built in order to link
OBJECTS = usb_specific_request.o cdc_task.o usb_descriptors.o main.o power_drv.o usb_drv.o usb_task.o usb_standard_request.o usb_device_task.o scheduler.o stk_525.o uart_lib.o uart_usb_lib.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep size

## Compile
usb_specific_request.o: ../usb_specific_request.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cdc_task.o: ../cdc_task.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_descriptors.o: ../usb_descriptors.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

power_drv.o: ../../../lib_mcu/power/power_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_drv.o: ../../../lib_mcu/usb/usb_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

stk_525.o: ../../../lib_board/stk_525/stk_525.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart_drv.o: ../../../../common/lib_mcu/uart_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart_lib.o: ../../../../common/lib_mcu/uart/uart_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_task.o: ../../../modules/usb/usb_task.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_standard_request.o: ../../../modules/usb/device_chap9/usb_standard_request.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb_device_task.o: ../../../modules/usb/device_chap9/usb_device_task.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

scheduler.o: ../../../../common/modules/scheduler/scheduler.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart_usb_lib.o: ../uart_usb_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@  || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).map

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

