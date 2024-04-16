
SRCS_S := ts.s
SRCS_C := t.c uart.c
OBJS := $(SRCS_S:.s=.o)
OBJS += $(SRCS_C:.c=.o)
$(info $(OBJS))

TARGET = prog

TOOL_AS = arm-none-eabi-as 
TOOL_LD = arm-none-eabi-ld
TOOL_NM = arm-none-eabi-nm
TOOL_CC = arm-none-eabi-gcc
TOOL_OBJCPY = arm-none-eabi-objcopy
TOOL_RDELF = arm-none-eabi-readelf
TOOL_OBJDMP = arm-none-eabi-objdump


#------
# FLAGS
#-------
C_FLAGS= -mcpu=arm926ej-s
L_FLAGS= -T
B_FLAGS= -O binary

FILE_LS = ../common/t.ld
FILE_OUT_INFO = info.txt
FILE_OUT_DISASS = disassemble.txt
FILE_OUT_SYMS = symbols.txt


all : $(TARGET).bin


# convert executable file to binary file using objcopy
$(TARGET).bin : $(TARGET).elf
	$(TOOL_OBJCPY) $(B_FLAGS) $^ $@
	$(TOOL_NM) $^ > $(FILE_OUT_SYMS)          # store symbols in output file
	$(TOOL_RDELF) -a $^ > $(FILE_OUT_INFO)    # store info of target file in output file
	$(TOOL_OBJDMP) -d $^ > $(FILE_OUT_DISASS) # store disassemble file of target file


$(TARGET).elf : $(OBJS)
	$(TOOL_LD) $(L_FLAGS) $(FILE_LS) -o $@ $^


%.o : %.s 
	$(TOOL_AS) $(C_FLAGS) -o $@ $^


%.o : %.c 
	$(TOOL_CC) -c  $(C_FLAGS) -o $@ $^

# remove all object files
fclean:
	rm -rf $(OBJS)

# remove final target files
clean: fclean
	rm -rf $(TARGET)* *.txt

# recompile everything
re: clean all

test: $(TARGET).bin
	@echo
	@echo Testing application
	@cat $(FILE_OUT_SYMS)
	@qemu-system-arm -M versatilepb -m 128M -kernel $(TARGET).bin -serial mon:stdio 
#	@qemu-system-arm -M versatilepb -m 128M -kernel $(TARGET).bin -serial /dev/null -nographic

.PHONY : all fclean clean re test 
