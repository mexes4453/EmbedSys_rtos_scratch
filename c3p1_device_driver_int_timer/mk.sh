#!/bin/bash

TARGET="dd_lcd"
OBJ="${TARGET}.o"
ASM="${TARGET}.s"
ELF="${TARGET}.elf"
BIN="${TARGET}.bin"
LD="linker.ld"

#arm-none-eabi-as -o ${OBJ} ${ASM}		#  assemble ${target}.s to ${target}.o
#arm-none-eabi-ld -T ${LD} -o ${ELF} ${OBJ}	# link ${target}.o to ${target}.elf file
#arm-none-eabi-nm ${ELF}				# show symbols in ${target}.elf
#arm-none-eabi-objcopy -O binary ${ELF} ${BIN}	# objcopy ${target}.elf to ${target}.bin
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm img1.bmp img1.o


arm-none-eabi-as ts.s -mcpu=arm926ej-s -o ts.o
arm-none-eabi-gcc -mcpu=arm926ej-s -c t.c -I./ -o t.o
#arm-none-eabi-gcc -c -mcpu=arm926ej-s vid.c  -o vid.o

#arm-none-eabi-gcc -c -o t.o t.c
#arm-none-eabi-as -o ts.o ts.s			#  assemble ${target}.s to ${target}.o
arm-none-eabi-ld -T ${LD} img1.o ts.o t.o -o ${ELF} 	 	# link ${target}.o to ${target}.elf file
arm-none-eabi-nm ${ELF}				# show symbols in ${target}.elf
arm-none-eabi-objcopy -O binary ${ELF} ${BIN}   # objcopy ${target}.elf to ${target}.bin


echo ready to go?
read dummy

qemu-system-arm -M versatilepb -m 128M -kernel dd_lcd.bin -serial mon:stdio

