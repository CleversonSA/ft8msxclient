# FT8MSXClient

A simple terminal FT8 client for MSX 1, using the ft8modem_network server via network, using UNAPI library.


## Prerequirements

The MSX itself is not capable (?) to decode and encode FT8 signals, so It uses a external server to do it. 

* Raspberry PI 3b+ or superior (I've used my Raspberry PI 400)
* USB Sound Card (the cheapest one :)) with a P2-P2 audio cable
* A HF Transceiver (if you are a licensed Ham Radio Operator) or a SW Receiver with BFO (USB) feature (Like Tecsun PL 600, ATS-20, ATS-25, CCrane, RXData) for RX only.
* MSX 1 OR MSX 2 with MSX-DOS 1.x, with Mapper
* Network cartridge or Wifi
* Running the ft8modem_network server at Raspberry PI (check https://github.com/CleversonSA/ft8modem_network)


## Installing on MSX (Easy)

* Copy the "ft8msxcli.COM" file inside "dist" folder to a S.D (If you are using a SD Mapper or Carnivore) or a difloppy drive
* Power on the MSX and access the storage
* Start client:
```
FT8MSXCL.COM <FT8MODEM SERVER IP> 6666
```

## Client keyboard shortcut commands
```
 - 'w' key

    Request to wipe the log list from server (better for reading)

 - 'u' key

    Request a receive log update from server

 - 'b' key

    Enables or disable the BEEP signal

 - 'c' key

    Enabled or disable the CQ only filter

 - 'q' key

    Quits the application
```

## COMPILING

If you wanna improve the project, I'll notice you that compile and run in a emulator only will be possible if it has network support (like Obsonet or Gr8net emulation). Otherwise you have to use a real machine with ethernet support.

* You have to use the SDCC 4.0 compiler. Since the project uses the Fusion C 1.3 library, it won't work with newer SDCC versions. You could be able to compile and generate the COM file, using the --sdccall 0 directive, but some printf issues will lead you to a corruped or a "damaged" COM file (if you run it in a real machine, It will generate \garbage readings...). At this time of compilation (2024/07) there's no updated version to support newer SDCCs :(

* I recommend build it on a Linux enviroment, but if you have Windows SDCC 4.0 installed, no problem. If you want compile SDCC 4.0 (it will be done once :) )
```
    IF YOU ARE COMPILING INTO A RASPBERRY PI 3B+ OR OLDER, create a SWAP file, because compilation takes 2-3GB of RAM memory:

        a) Generate SWAP file:

             sudo dd if=/dev/zero of=/swapfile bs=1024 count=4048576
    
        b) Change file permissions:

             sudo chmod 600 /swapfile

        c) Make the file as a swap:

             sudo mkswap /swapfile

        d) Add the swap file to memory:

             sudo swapon /swapfile

    IF YOU HAVE 4GB RAM OR MORE, OR CREATED THE SWAP FILE, Follow the steps bellow to compile SDCC

    1) Download the SDCC 4.0 source: https://sourceforge.net/projects/sdcc/files/sdcc/4.0.0/
    2) Check you have installed the following packages:

        sudo apt update
        sudo apt install build-essential cmake git autoconf bison flex texinfo

    3) Unzip the source:

        tar -xvf  sdcc-src-4.0.0.tar.bz2

    4) Access the sdcc source folder:

        cd sdcc-4.0.0
    
    5) Prepare for install:

        ./configure

    6) If no errors, compile and install:

        make -j4 && sudo make install -j4

        Press ENTER and go to life...because It will take 4-5h in a RPI 3b+

    7) If all things worked:

        sdcc --version

        Will output a similar result like this:

        
        SDCC : mcs51/z80/z180/r2k/r3ka/gbz80/tlcs90/ez80_z80/ds390/pic16/pic14/TININative/ds400/hc08/s08/stm8/pdk13/pdk14/pdk15 4.0.0 #11528 (Linux)
        published under GNU General Public License (GPL)


    8) Modify standard SDCC Z80 Library to avoid printf, sprintf, vprintf, putchar and getchar library conflits with Fusion C, otherwide the COM file will be corrupted (IMPORTANT!!!):

        sudo su

        cd /usr/share/sdcc/lib/z80/

        cp z80.lib z80.lold

        sdar -d z80.lib printf.rel
        sdar -d z80.lib vprintf.rel
        sdar -d z80.lib sprintf.rel
        sdar -d z80.lib getchar.rel
        sdar -d z80.lib putchar.rel

    9) Install hex2bin to convert Intel Hex to Z80:

        - Download hex2bin sources: https://sourceforge.net/projects/hex2bin/files/hex2bin/Hex2bin-2.5.tar.bz2/download

        - Extract files:

            tar -xvf Hex2bin-2.5.tar.bz2

        - Compile it

            cd Hex2bin-2.5.tar.bz2
            make
            sudo make install

    NOW YOU ARE ABLE TO COMPILE THE FT8MSXCLIENT APP, IT COULD BE USED TO ANY MSX APP YOU COULD CREATE AT FUTURE:


    1) Access the ft8msxclient source and run:

        make

        The final binary file will be generated at "dist" folder, with the filename:

        ft8msxcl.COM

```


## Sending COM file over TFTP

Just another option, special if you are compiling and testing this project, you could send file to MSX using TFTP utility (see http://www.konamiman.com/msx/networking/tftp.com )
```

    At MSX, supposed you are connected to the RPi with OBSONET or Wifi (this steps will the use the RPI Ethernet Port with OBSONET), the IP RANGE is pre-configured at RPI ft8modem image.

    Please check on MSX-DOS Ethernet utitity (Ex: INL2 S) the MSX IPv4 address.

    After that, at MSX-DOS prompt:

       TFTP /S

    At this point, the server was UP at MSX.

    At RPi:

        sudo apt update
        sudo apt install tftp
        cd (main folder of ft8msxclient project)
        tftp (IP of MSX, default: 192.168.113.20 if connected over cable) 

    At TFTP prompt:

        binary[ENTER]
        rexmt1[ENTER]
        put ft8msxcl.COM[ENTER]

    Sometimes you could receive timeout error...just try put command again. After uploaded file:

        quit[ENTER]


    
## LICENSE

    Apache License V2
