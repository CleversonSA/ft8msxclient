

CFLAGS = --code-loc 0x106 --data-loc 0x0 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size --verbose 
CFLAGS_REL = --out-fmt-ihx --no-std-crt0 -mz80 --opt-code-speed --verbose --code-loc 0x180 --data-loc 0x0 --disable-warning 196 --allow-unsafe-read
HEADERS= -I fusion-c/header/ fusion-c/lib/fusion.lib fusion-c/lib/asm.lib fusion-c/include/printf.rel -L fusion-c/lib/ 
HEX2BIN= hex2bin
EXTENSION= COM
DEFAULT_HEX2BIN_SIZE=

CC = /usr/bin/sdcc

all: hello.c
	-make clean
	-$(CC) $(CFLAGS) $(HEADERS) -o UnapiHelper.rel -c UnapiHelper.c
	-$(CC) $(CFLAGS) $(HEADERS) --out-fmt-ihx fusion-c/include/crt0_msxdos.rel UnapiHelper.rel hello.c -o hello.ihx

install:
	-${HEX2BIN} -e ${EXTENSION} ${DEFAULT_HEX2BIN_SIZE} hello.ihx

test:
	-rm ./dsk/dska/hello.*
	-rm ./dsk/dska/UnapiHelper.*
	-cp hello.COM ./dsk/dska
	-openmsx -script ./openMSX/emul_start_config.txt

ftp:
	-cp hello.COM ~/FTP/msx

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
