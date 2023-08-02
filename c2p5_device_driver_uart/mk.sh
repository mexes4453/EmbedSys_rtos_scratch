#!/bin/bash

TARGET="dd_uart"
OBJ="${TARGET}.o"
ASM="${TARGET}.s"
ELF="${TARGET}.elf"
BIN="${TARGET}.bin"
LD="linker.ld"

#arm-none-eabi-as -o ${OBJ} ${ASM}		#  assemble ${target}.s to ${target}.o
#arm-none-eabi-ld -T ${LD} -o ${ELF} ${OBJ}	# link ${target}.o to ${target}.elf file
#arm-none-eabi-nm ${ELF}				# show symbols in ${target}.elf
#arm-none-eabi-objcopy -O binary ${ELF} ${BIN}	# objcopy ${target}.elf to ${target}.bin

arm-none-eabi-gcc -c -o t.o t.c
arm-none-eabi-as -o ts.o ts.s			#  assemble ${target}.s to ${target}.o
arm-none-eabi-ld -T ${LD} -o ${ELF} ts.o  t.o 	# link ${target}.o to ${target}.elf file
arm-none-eabi-nm ${ELF}				# show symbols in ${target}.elf
arm-none-eabi-objcopy -O binary ${ELF} ${BIN}   # objcopy ${target}.elf to ${target}.bin
