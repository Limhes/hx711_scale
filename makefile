CC = avr-g++
OBJCOPY = avr-objcopy
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f

MCU = atmega328p
F_CPU = 16000000

LFUSE = 0xFF
HFUSE = 0xDA

TARGET = firmware
SRC = main.cpp HX711_ADC.cpp millis.cpp uart.cpp
OBJ = $(SRC:.cpp=.o)
LST = $(SRC:.cpp=.lst)
#LIB = /home/rene/.arduino15/packages/arduino/hardware/avr/1.8.5/cores/arduino/

FORMAT = ihex

OPTLEVEL = s

CDEFS =

CFLAGS = -DF_CPU=$(F_CPU)UL
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPTLEVEL)
CFLAGS += -mmcu=$(MCU)
CFLAGS += -std=c++11
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall
CFLAGS += -Wa,-adhlns=$(<:.cpp=.lst)
#CFLAGS += -I$(LIB)

LDFLAGS = -Wl,--gc-sections
LDFLAGS += -Wl,--print-gc-sections

AVRDUDE_MCU = atmega328p
#AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_PORT = /dev/ttyACM0
AVRDUDE_FLAGS = -p $(AVRDUDE_MCU) -c arduino -P $(AVRDUDE_PORT) -b 115200

MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_FLASH = Preparing HEX file:

all: gccversion $(TARGET).elf $(TARGET).hex size

.SECONDARY: $(TARGET).elf
.PRECIOUS: $(OBJ)

%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -j .text -j .data $< $@

%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) -mmcu=$(MCU) $(LDFLAGS) $^ --output $(@F)

%.o : %.cpp
	@echo $(MSG_COMPILING) $<
	$(CC) $(CFLAGS) -c $< -o $(@F)

gccversion:
	@$(CC) --version

size: $(TARGET).elf
	@echo
	$(SIZE) -C --mcu=$(AVRDUDE_MCU) $(TARGET).elf

analyze: $(TARGET).elf
	$(NM) -S --size-sort -t decimal $(TARGET).elf

isp: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex:i

fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

release: fuses isp

clean:
	$(REMOVE) $(TARGET).hex $(TARGET).elf $(OBJ) $(LST) *~
