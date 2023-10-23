* FT8MSXClient

A simple terminal FT8 client for MSX 1.1 using the ft8modem_network server via network, using UNAPI library.


* PREREQUISITES

The MSX itself is not capable (?) to decode and encode FT8 signals, so It uses a external server to do it. In my project, I used:

    - Raspberry PI 3b+ or superior (I've used my Raspberry PI 400)

    - USB Sound Card (the cheapest one :))

    - A HF Transceiver (if you are a licensed Ham Radio Operator) or a SW Receiver with BFO (USB) feature (Like Tecsun PL 600, ATS-20, ATS-25, CCrane, RXData) for RX only.
    
    - MSX Expert 1.1 (I've not tested with other machine)

    - SD Mapper for MSX

    - Network card

    - Running the ft8modem_network server at Raspberry PI


** The MSX part:

    - You need MSX-DOS 1.0 or newer or NEXTOR running at the MSX

    - In my project, I've used the OBSONET Network cartridge connected directly to the Raspberry PI Ethernet port. At the RPi I'm not using DHCP service.

        https://theretrohacker.com/2022/11/04/the-obsonet-cartridge-for-the-msx/

    - I'll reproduce my MSX commands steps:

        (at OBSONET files folder)
        RAMHELPR.COM
        INL2 I
        INL2 IP L 169.254.3.103


** The Raspberry PI part:

    - You need the SDCC compiler installed with Fusion C:

        https://www.youtube.com/watch?v=rVddcrPMvtg

    - Compile and run the ft8modem_network project:

        https://github.com/CleversonSA/ft8modem_network

    - Check if the Ethernet port of RPi has no dhcp enabled and it has the IP 169.254.3.104 . This will be modified later, it's my fault =(

    - Start the ft8modem service:

        $ ft8modem ft8 <device ID>

    - Connects the line-out or the headphone jack from the receiver to the mic input of usb soundcard. Tune it at a hf band (I like the 28.074Mhz (10m ham radio band)) Be careful to the receiver volume level (or nothing will be decoded).


* INSTALLING AT MSX

    You could copy the file to the SD/Floppy/Tape(:)):

    dist/ft8msxcl.COM

    - Via TFTP

        1) At RPi install the TFTP client:

            sudo apt update
            sudo apt install tftp

        2) Run TFTP server

            (at the project folder)
            cd dist
            tftp 169.254.3.103 [ENTER]
            binary [ENTER]

            (It's important to be binary mode or the file will be corrupted)

        3) At MSX-DOS type 

            tftp /s[ENTER]

        4) At RPI, in TFTP server

            put ft8msxcl.COM

        5) At RPI, hit CTRL+C

        6) At MSX, press any key and OK


* RUNNING AT MSX

    ft8msxcl

    Just it


* BASIC KEY COMMANDS:

    * 'u' key

        Request a receive log update from server

    * 'b' key

        Enables or disable the BEEP signal

    * 'c' key

        Enabled or disable the CQ only filter

    * 'q' key

        Quits the application


* COMPILING:

    1) Compile the application with SDCC:
    
        make

    2) Create the binary COM file:

        make install


* LICENSE

    Apache License V2