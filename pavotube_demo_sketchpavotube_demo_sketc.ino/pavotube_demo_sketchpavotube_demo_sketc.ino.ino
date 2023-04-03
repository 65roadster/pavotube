// Demo of Pavotube II 30C control using Nordic nRF24L01+ module and Teensy 3.2 microcontroller
// https://github.com/65roadster/pavotube
//

#include <SPI.h>
void spiSendByte(byte, byte);
void sendCommand4Byte(byte, byte, byte, byte, byte);
void spiSendCommand(byte, byte *, uint);
void setBrightness(byte, byte);
void setHSI(byte, byte, byte);
void setHSI240(byte, byte, byte);
void setHSI241(byte, byte, byte);

const int CE{0};
const int CSN{10};
int delayTime{500};
byte colorTemp{0};
const byte ADDR[] = {0, 0, 0, 0, 12};
void setup() {
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  digitalWrite(CE, LOW);
  digitalWrite(CSN, HIGH);

  SPI.begin();
  
  // configure the transceiver
  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff); // FLUSH_TX
  spiSendByte(39, 16); // clear MAX_RT
  spiSendCommand(48, ADDR, sizeof(ADDR)); // set TX_ADDR
  spiSendCommand(42, ADDR, sizeof(ADDR)); // set RX_ADDR_P0
  spiSendByte(33, 1); // Auto Ack P0
  spiSendByte(34, 1); // Enable RX P0
  spiSendByte(36, 31); // Set retransmits and delay
  spiSendByte(37, 115); // set 2515MHz
  spiSendByte(38, 6); // set Pout = 0dBm
  spiSendByte(32, 14); // setup CRC and POWER_UP

  delay (1);

  // CCT Demo - cycle brightness at constant color temp
  for (i : {0, 25, 50, 75, 100}) {
    setBrightness(25, i);
    delay (delayTime);
  }
  setBrightness(0, 50);
  delay (delayTime);
  delay (delayTime);

  // CCT Demo - cycle color temp at constant brightness
  for (i : {1, 25, 50, 75, 100}) {
    setBrightness(i, 50);
    delay (delayTime);
  }
  setBrightness(0, 50);
  delay (delayTime);

  // HSI Demo
  for (int i=0 ; i < 256 ; i++) {
    setHSI240(50, i, 100);
    delay(20);
  }
  setBrightness(0, 50);
  delay (delayTime);
  delay (delayTime);
  for (int i=0 ; i < 105 ; i++) {
    setHSI241(50, i, 100);
    delay(20);
  }
  setBrightness(0, 50);
}

void loop() {
}

void spiSendByte(byte cmd, byte data)
{
  digitalWrite(CSN, LOW);
  SPI.transfer(cmd); // set TX addr = 12
  SPI.transfer(data);
  digitalWrite(CSN, HIGH);
}

void sendCommand4Byte(byte cmd, byte data1, byte data2, byte data3, byte data4)
{
  digitalWrite(CSN, LOW);
  SPI.transfer(cmd);
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  SPI.transfer(data4);
  digitalWrite(CSN, HIGH);
}

//
// val1 from 0 to 254 is accepted by nanlight
//
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

void setHSI(byte intensity, byte hue, byte saturation)
{
  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
  sendCommand4Byte(160, 240, intensity, hue, saturation);
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
  sendCommand4Byte(160, 240, intensity, hue, saturation);
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
  sendCommand4Byte(160, 241, intensity, hue, saturation);
  digitalWrite(CE, HIGH);
  delayMicroseconds (500); // needed about 250us minimum here
  digitalWrite(CE, LOW);

  spiSendByte(7, 0xff);
  spiSendByte(225, 0xff);
  spiSendByte(226, 0xff);
  spiSendByte(39, 32);
}

void spiSendCommand(byte cmd, byte *data, uint nData)
{
  digitalWrite(CSN, LOW);
  SPI.transfer(cmd);
  for (int i = 0 ; i < nData ; i++) {
    SPI.transfer(data[i]);
  }
  digitalWrite(CSN, HIGH);
}
