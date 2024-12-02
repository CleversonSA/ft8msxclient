CFLAGS = --code-loc 0x180 --data-loc 0x0 --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size --verbose --allow-unsafe-read
CFLAGS_REL = --out-fmt-ihx --no-std-crt0 -mz80 --opt-code-speed --verbose --code-loc 0x180 --data-loc 0x0 --disable-warning 196 --allow-unsafe-read

# Identify if it´s Windows or linux
ifeq ($(OS),Windows_NT)
# ------------- WINDOWS ----------------
HEADERS= -I fusion-c\\header\\ fusion-c\\lib\\fusion.lib fusion-c\\lib\\asm.lib -L fusion-c\\lib\\ 
CRT0 = ..\\fusion-c\\include\\crt0_msxdos_advanced.rel
WINMODE = 1
else
# -------------- LINUX -----------------
HEADERS= -I fusion-c/header/ fusion-c/lib/fusion.lib fusion-c/lib/asm.lib -L fusion-c/lib/
CRT0 = ../fusion-c/include/crt0_msxdos_advanced.rel
WINMODE = 0
endif

HEX2BIN= hex2bin
EXTENSION= COM
DEFAULT_HEX2BIN_SIZE=

CC = sdcc

all: clean clean-binaries build check binaries

dev: all emulator

build:
ifeq ($(WINMODE),1)
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c\\include\\printf.rel -o UnapiHelper.rel -c UnapiHelper.c
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c\\include\\printf.rel -o CustomCharmap.rel -c CustomCharmap.c
	-$(CC) $(CFLAGS) $(HEADERS) --out-fmt-ihx fusion-c\\include\\printf.rel fusion-c\\include\\crt0_msxdos_advanced.rel UnapiHelper.rel CustomCharmap.rel ft8msxcl.c -o ft8msxcl.ihx
else
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c/include/printf.rel -o UnapiHelper.rel -c UnapiHelper.c
	-$(CC) $(CFLAGS) $(HEADERS)  fusion-c/include/printf.rel -o CustomCharmap.rel -c CustomCharmap.c
	-$(CC) $(CFLAGS) $(HEADERS) --out-fmt-ihx fusion-c/include/printf.rel fusion-c/include/crt0_msxdos_advanced.rel UnapiHelper.rel CustomCharmap.rel ft8msxcl.c -o ft8msxcl.ihx
endif	

binaries: hex
ifeq ($(WINMODE),1)
	-copy ft8msxcl.COM .\\dsk\\dska
	-copy ft8msxcl.COM .\\dist
	-make clean
else
	-cp ft8msxcl.COM ./dsk/dska
	-cp ft8msxcl.COM ./dist
	-make clean
endif

hex:
	-${HEX2BIN} -e ${EXTENSION} ${DEFAULT_HEX2BIN_SIZE} ft8msxcl.ihx


check: UnapiHelper.rel ft8msxcl.ihx

clean-binaries:
ifeq ($(WINMODE),1)
	-del /y .\\dsk\\dska\\ft8msxcl.COM
	-del /y .\\dsk\\dska\\UnapiHelper.*
	-del /y .\\dsk\\dist\\ft8msxcl.COM
else
	-rm -f ./dsk/dska/ft8msxcl.COM
	-rm -f ./dsk/dska/UnapiHelper.*
	-rm -f ./dsk/dist/ft8msxcl.COM
endif

clean:
ifeq ($(WINMODE),1)
	-del /f *.rel
	-del /f *.lst
	-del /f *.noi
	-del /f *.sym
	-del /f *.ihx
	-del /f *.map
	-del /f *.lk
	-del /f *.asm
	-del /f *.COM
else
	-rm -f *.rel
	-rm -f *.lst
	-rm -f *.noi
	-rm -f *.sym
	-rm -f *.ihx
	-rm -f *.map
	-rm -f *.lk
	-rm -f *.asm
	-rm -f *.COM
	-rm -f *.com
endif

emulator:
ifeq ($(WINMODE),1)
	openmsx -script .\\openMSX\\emul_start_config.txt
else
	openmsx -script ./openMSX/emul_start_config.txt
endif
