// Demo of Pavotube II 30C control using Nordic nRF24L01+ module and Teensy 3.2 microcontroller
// https://github.com/65roadster/pavotube
//

#include <SPI.h>
void spiSendByte(byte, byte);
void spiSendCommand(byte, const byte *, uint);
void setBrightness(byte, byte);
void setHSI240(byte, byte, byte);
void setHSI241(byte, byte, byte);

const int CE{0}; // chip enable pin
const int CSN{10}; // /CS pin

const int delayTime{500};
const byte ADDR[] = {0, 0, 0, 0, 12}; // Pavotube is expecting ADDR = 12

void setup() {
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  digitalWrite(CE, LOW);
  digitalWrite(CSN, HIGH);

  SPI.begin();
  
  // configure the transceiver
  spiSendByte(225, 0xff);     // FLUSH_TX
  spiSendByte(39, 16);        // clear MAX_RT
  spiSendCommand(48, ADDR, sizeof(ADDR)); // set TX_ADDR
  spiSendCommand(42, ADDR, sizeof(ADDR)); // set RX_ADDR_P0
  spiSendByte(33, 1);         // Auto Ack P0
  spiSendByte(34, 1);         // Enable RX P0
  spiSendByte(36, 31);        // Set retransmits and delay
  spiSendByte(37, 115);       // set 2515MHz
  spiSendByte(38, 6);         // set Pout = 0dBm
  spiSendByte(32, 14);        // setup CRC and POWER_UP

  setBrightness(0, 50);
}

void loop() {
    setHSI240(100, 240, 100);
    delay(5);
    setHSI241(0, 104, 100);
    delay(5);
}


void spiSendByte(byte cmd, byte data)
{
  digitalWrite(CSN, LOW);
  SPI.transfer(cmd);
  SPI.transfer(data);
  digitalWrite(CSN, HIGH);
}

void setBrightness(byte intensity, byte colorTemp)
{
  byte packet[4];
  packet[0] = intensity;
  packet[1] = colorTemp;
  packet[2] = intensity + colorTemp;
  packet[3] = 255 - packet[2];

  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
  spiSendCommand(160, packet, 4);
  digitalWrite(CE, HIGH);
  delayMicroseconds (500); // needed about 250us minimum here
  digitalWrite(CE, LOW);

  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
}

void setHSI240(byte intensity, byte hue, byte saturation)
{
  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
  byte packet[4];
  packet[0] = 240;
  packet[1] = intensity;
  packet[2] = hue;
  packet[3] = saturation;
  
  spiSendCommand(160, packet, sizeof(packet));
  digitalWrite(CE, HIGH);
  delayMicroseconds (500); // needed about 250us minimum here
  digitalWrite(CE, LOW);

  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
}

void setHSI241(byte intensity, byte hue, byte saturation)
{
  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
  byte packet[4];
  packet[0] = 241;
  packet[1] = intensity;
  packet[2] = hue;
  packet[3] = saturation;
  spiSendCommand(160, packet, sizeof(packet));
  digitalWrite(CE, HIGH);
  delayMicroseconds (500); // needed about 250us minimum here
  digitalWrite(CE, LOW);

  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
}

void spiSendCommand(byte cmd, const byte *data, uint nData)
{
  digitalWrite(CSN, LOW);
  SPI.transfer(cmd);
  for (uint i = 0 ; i < nData ; i++) {
    SPI.transfer(data[i]);
  }
  digitalWrite(CSN, HIGH);
}
