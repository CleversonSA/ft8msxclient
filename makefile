

CFLAGS = --code-loc 0x180 --data-loc 0x0 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size --verbose --allow-unsafe-read
CFLAGS_REL = --out-fmt-ihx --no-std-crt0 -mz80 --opt-code-speed --verbose --code-loc 0x180 --data-loc 0x0 --disable-warning 196 --allow-unsafe-read
HEADERS= -I fusion-c/header/ fusion-c/lib/fusion.lib fusion-c/lib/asm.lib -L fusion-c/lib/
CRT0 = ../fusion-c/include/crt0_msxdos_advanced.rel
HEX2BIN= hex2bin
EXTENSION= COM
DEFAULT_HEX2BIN_SIZE=

CC = sdcc

all: ft8msxcl.c
	-make clean
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c/include/printf.rel -o UnapiHelper.rel -c UnapiHelper.c
	-$(CC) $(CFLAGS) $(HEADERS) --out-fmt-ihx fusion-c/include/printf.rel fusion-c/include/crt0_msxdos_advanced.rel UnapiHelper.rel ft8msxcl.c -o ft8msxcl.ihx
	-${HEX2BIN} -e ${EXTENSION} ${DEFAULT_HEX2BIN_SIZE} ft8msxcl.ihx
	-rm ./dsk/dska/ft8msxcl.*
	-rm ./dsk/dska/UnapiHelper.*
	-cp ft8msxcl.COM ./dsk/dska
	-cp ft8msxcl.COM ./dist


clean:
	-rm *.rel
	-rm *.com 
	-rm *.COM 
	-rm *.lst
	-rm *.noi
	-rm *.sym
	-rm *.ihx
	-rm *.map
	-rm *.lk
	-rm *.asm
	-rm ./dsk/dska/ft8msxcl.COM
	-rm ./dsk/dska/UnapiHelper.*
	-rm ./dist/ft8msxcl.COM
