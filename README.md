# Pavotube 2.4GHz RF Link Reverse Engineering

This is the results of my hackinig the Nanlite PavoTube II 30C RGB LED Tube to develop my own hardware to control it over an RF link.

# Demo Hardware
The demo sketch was loaded onto a Teensy 3.2 which was wired to an NRF24L01+ module.<br />

Teensy 3.2  <--> NRF24L01+ module<br />
3.3V <--> VCC<br />
GND  <--> GND<br />
SCLK <--> SCL<br />
MOSI <--> MOSI<br />
MISO <--> MISO<br />
D10  <--> /CS<br />
D0   <--> CE<br />
N/C  <--> IRQ<br />

# Transceiver Configuration

The Nanlink WSRCC2 remote was used to glean insight into the protocol. The remote uses the Nordic RF NRF24L01 controlled by an STM8L151 microcontroller. The Nordic IC is on a castellated via module, connected to the main PCB via a flex cable.<br />

The NRF24L01 is configured as follows:
- TX_ADDR = 12<br />
- RX_ADDR_P0 = 12<br />
- ENAA_P0 set to 1 (enable auto ACK on data pipe 0)<br />
- ERX_P0 set to 1 (enable RX data pipe 0)<br />
- SETUP_RETR = 0x1f (15 retransmits, 200us delay)<br />
- RF_CH = 0x73 (2515MHz)<br />
- RF_SETUP = 6 (output power = 0dBm)<br />
- CONFIG = 0xE (enable CRC, CRC is 2 bytes, POWER_UP)<br />
- Other register values set to defaults<br />

# SPI Bus Sniffing

The SPI bus and NRF24L01 CE pin signals were sniffed by soldering wires onto the castellated vias on the RF module, then captured using a Saleae logic analyzer. The captures were saved and can be viewed using Saleae's Logic software.

The remote first sends commands over 2460MHz, which are not acknowledged by the Pavotube and don't appear to accomplish anything.

The remote then configures the NRF24L01 for 2515MHz, sends a series of bytes which are acknowledged by the Pavotube but do not seem to do anything.

It then sends the key command which updates the settings in the Pavotube. The command is sent using W_TX_PAYLOAD, consisting of 4 bytes:

CCT Mode: INTENSITY, COLORTEMP, (INTENSITY + COLORTEMP), 255 - (INTENSITY + COLORTEMP)<br />
INTENSITY = [0 100]<br />
COLORTEMP = [0 100]<br />

HSI Mode: COLORBAND, INTENSITY, HUE, SATURATION<br />
COLORBAND = 240 OR 241(hue = 0 TO 104)<br />
INTENSITY = [0 100]<br />
HUE = [0 255]<br />
SATURATION = [0 100]<br />


Capture of setting max brightness and max colortemp in CCI mode: [a relative link](spi_captures/CCI_max_brightness_max_colortemp.sal)<br />

Capture of setting min brightness and max colortemp in CCI mode: [a relative link](spi_captures/CCI_min_brightness_max_colortemp.sal)<br />


## PNG images of SPI bus sniffing:

### CCT mode, max brightness, max color temperature:
[Protocol image with notations](spi_captures/CCT_max_brightness_max_colortemp_overview.png)<br />
[Zoomed into command 1](spi_captures/CCT_max_brightness_max_colortemp_cmd1.png)<br />
[Zoomed into command 2](spi_captures/CCT_max_brightness_max_colortemp_cmd2.png)<br />
[Zoomed into command 3a](spi_captures/CCT_max_brightness_max_colortemp_cmd3a.png)<br />
[Zoomed into command 3b](spi_captures/CCT_max_brightness_max_colortemp_cmd3b.png)<br />
[Zoomed into command 4](spi_captures/CCT_max_brightness_max_colortemp_cmd4.png)<br />
[Zoomed into command 5](spi_captures/CCT_max_brightness_max_colortemp_cmd5.png)<br />
[Zoomed into command 6](spi_captures/CCT_max_brightness_max_colortemp_cmd6.png)<br />

### CCT mode, min brightness, max color temperature:
[Zoomed into command 5](spi_captures/CCT_min_brightness_max_colortemp_cmd.png)

### CCT, HSI, FX buttons:
The CCT and HSI buttons on the remote do cause the Pavotube to change between CCT and HSI modes. The RF payloads are not acknowledged however. I didn't dig into this further as I have no interest in switching tube modes in that way. The tube changes modes automatically when the corresponding HSI or CCT color/intensity/hue command is sent.

The FX button sends commands on the 2460MHz channel but doesn't have any noticeable effect on the Pavotube.

## Saleae captures
These can be loaded into Saleae Logic to be viewed in detail:
[CCT Max Brightness Max Colortemp](C:\Users\Ray\Documents\GitHub\pavotube\spi_captures\CCT_max_brightness_max_colortemp.sal)<br />
[CCT Max Brightness Min Colortemp](C:\Users\Ray\Documents\GitHub\pavotube\spi_captures\CCT_max_brightness_min_colortemp.sal)<br />
[CCT Min Brightness Max Colortemp](C:\Users\Ray\Documents\GitHub\pavotube\spi_captures\CCT_min_brightness_max_colortemp.sal)<br />
[CCT Min Brightness Min Colortemp](C:\Users\Ray\Documents\GitHub\pavotube\spi_captures\CCT_min_brightness_min_colortemp.sal)<br />


# Software Demo

This sketch was developed on a Teensy 3.2 using the Arduino IDE plugins. It should work on a Nano, Uno, etc.
[Code folder](C:\Users\Ray\Documents\GitHub\pavotube\pavotube_demo_sketch\pavotube_demo_sketch.ino)


## Versioning

I am not discplined about versioning, let's call this 1.0

## License

Public domain
