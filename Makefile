

CFLAGS = --code-loc 0x180 --data-loc 0x0 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size --verbose --allow-unsafe-read
CFLAGS_REL = --out-fmt-ihx --no-std-crt0 -mz80 --opt-code-speed --verbose --code-loc 0x180 --data-loc 0x0 --disable-warning 196 --allow-unsafe-read
HEADERS= -I fusion-c/header/ fusion-c/lib/fusion.lib fusion-c/lib/asm.lib -L fusion-c/lib/
CRT0 = ../fusion-c/include/crt0_msxdos_advanced.rel
HEX2BIN= hex2bin
EXTENSION= COM
DEFAULT_HEX2BIN_SIZE=

CC = sdcc

all: clean clean-binaries build check binaries clean


build:
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c/include/printf.rel -o UnapiHelper.rel -c UnapiHelper.c
	-$(CC) $(CFLAGS) $(HEADERS) --out-fmt-ihx fusion-c/include/printf.rel fusion-c/include/crt0_msxdos_advanced.rel UnapiHelper.rel ft8msxcl.c -o ft8msxcl.ihx
	

binaries: hex
	-cp ft8msxcl.COM ./dsk/dska
	-cp ft8msxcl.COM ./dist


hex:
	-${HEX2BIN} -e ${EXTENSION} ${DEFAULT_HEX2BIN_SIZE} ft8msxcl.ihx


check: UnapiHelper.rel ft8msxcl.ihx

clean-binaries:
	-rm -f ./dsk/dska/ft8msxcl.COM
	-rm -f ./dsk/dska/UnapiHelper.*
	-rm -f ./dsk/dist/ft8msxcl.COM


clean:
	-rm -f *.rel
	-rm -f *.com 
	-rm -f *.COM 
	-rm -f *.lst
	-rm -f *.noi
	-rm -f *.sym
	-rm -f *.ihx
	-rm -f *.map
	-rm -f *.lk
	-rm -f *.asm
