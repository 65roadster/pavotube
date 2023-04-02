# Pavotube 2.4GHz RF Link Reverse Engineering

This is the results of my hackinig the Nanlite PavoTube II 30C RGB LED Tube to develop my own hardware to control it over an RF link.

## Demo Hardware
The demo sketch was loaded onto a Teensy 3.2 which was wired to an NRF24L01+ module.

Teensy 3.2  <--> NRF24L01+ module
3.3V <--> VCC
GND  <--> GND
SCLK <--> SCL	K
MOSI <--> MOSI
MISO <--> MISO
D10  <--> /CS
D0   <--> CE
N/C  <--> IRQ

## Transceiver Configuration

The Nanlink WSRCC2 remote was used to glean insight into the protocol. The remote uses the Nordic RF NRF24L01 controlled by an STM8L151 microcontroller. The Nordic IC is on a castellated via module, connected to the main PCB via a flex cable.

The NRF24L01 is configured as follows:
- TX_ADDR = 12
- RX_ADDR_P0 = 12
- ENAA_P0 set to 1 (enable auto ACK on data pipe 0)
- ERX_P0 set to 1 (enable RX data pipe 0)
- SETUP_RETR = 0x1f (15 retransmits, 200us delay)
- RF_CH = 0x73 (2515MHz)
- RF_SETUP = 6 (output power = 0dBm)
- CONFIG = 0xE (enable CRC, CRC is 2 bytes, POWER_UP)
- Other register values set to defaults

## SPI Bus Sniffing

The SPI bus and NRF24L01 CE pin signals were sniffed by soldering wires onto the castellated vias on the RF module, then captured using a Saleae logic analyzer. The captures were saved and can be viewed using Saleae's Logic software.

The remote first sends commands over 2460MHz, which are not acknowledged by the Pavotube and don't appear to accomplish anything.

The remote then configures the NRF24L01 for 2515MHz, sends a series of bytes which are acknowledged by the Pavotube but do not seem to do anything.

It then sends the key command which updates the settings in the Pavotube. The command is sent using W_TX_PAYLOAD, consisting of 4 bytes:

CCT Mode: INTENSITY, COLORTEMP, (INTENSITY + COLORTEMP), 255 - (INTENSITY + COLORTEMP)
INTENSITY = [0 100]
COLORTEMP = [0 100]

HSI Mode: COLORBAND, INTENSITY, HUE, SATURATION
COLORBAND = 240 OR 241(hue = 0 TO 104)
INTENSITY = [0 100]
HUE = [0 255]
SATURATION = [0 100]


Capture of setting max brightness and max colortemp in CCI mode: [a relative link](spi_captures/CCI_max_brightness_max_colortemp.sal)

Capture of setting min brightness and max colortemp in CCI mode: [a relative link](spi_captures/CCI_min_brightness_max_colortemp.sal)


PNG images of SPI bus sniffing:

CCI mode, max brightness, max color temperature:
Protocol image with notations: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_overview.png)
Zoomed into command 1: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd1.png)
Zoomed into command 2: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd2.png)
Zoomed into command 3a: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd3a.png)
Zoomed into command 3b: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd3b.png)
Zoomed into command 4: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd4.png)
Zoomed into command 5: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd5.png)
Zoomed into command 6: [a relative link](spi_captures/CCI_max_brightness_max_colortemp_cmd6.png)

CCI mode, min brightness, max color temperature:
Zoomed into command 5: [a relative link](spi_captures/CCI_min_brightness_max_colortemp_cmd.png)

The CCT and HSI buttons on the remote do cause the Pavotube to change between CCT and HSI modes. The RF payloads are not acknowledged however. I didn't debug this further as I have no interest in switching tube modes in that way. The tube changes modes automatically when the corresponding HSI or CCT color/intensity/hue command is sent.

The FX button sends commands on the 2460MHz channel but doesn't have any effect on the Pavotube.

## Software Demo

#include <SPI.h>
const int CE{0};
const int CSN{10};

#include <SPI.h>



## Versioning

I am not discplined about versioning, let's call this 1.0

## License

Public domain
