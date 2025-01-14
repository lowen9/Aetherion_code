#include <SX126x.h>

#define SCLK_PIN 5
#define MISO_PIN 3
#define MOSI_PIN 6
#define CS_PIN 7
#define VSPI FSPI

#define LORA_RST_PIN 8
#define LORA_BUSY_PIN 34
#define LORA_DIO 33

SX126x LoRa;

SPIClass *vspi = NULL;

void setup() {

  // Begin serial communication
  Serial.begin(115200);
  delay(1000);

  vspi = new SPIClass(VSPI);
  vspi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  LoRa.setSPI(*vspi, 16E6);
  // Begin LoRa radio and set NSS, reset, busy
  LoRa.setPins(CS_PIN, LORA_RST_PIN, LORA_BUSY_PIN, LORA_DIO);

  Serial.println("Begin LoRa radio");
  if (!LoRa.begin()){
    Serial.println("Something wrong, can't begin LoRa radio");
    while(1);
  }

  // Configure TCXO or XTAL used in RF module
  Serial.println("Set RF module to use TCXO as clock reference");
  uint8_t dio3Voltage = SX126X_DIO3_OUTPUT_1_8;
  uint32_t tcxoDelay = SX126X_TCXO_DELAY_10;
  LoRa.setDio3TcxoCtrl(dio3Voltage, tcxoDelay);
  
  // Set frequency to 915 Mhz
  Serial.println("Set frequency to 868 Mhz");
  LoRa.setFrequency(868000000);

  // Set RX gain to boosted gain
  Serial.println("Set RX gain to boosted gain");
  LoRa.setRxGain(SX126X_RX_GAIN_BOOSTED);

  // Configure modulation parameter including spreading factor (SF), bandwidth (BW), and coding rate (CR)
  Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
  uint8_t sf = 7;
  uint32_t bw = 125000;
  uint8_t cr = 4;
  LoRa.setLoRaModulation(sf, bw, cr);

  // Configure packet parameter including header type, preamble length, payload length, and CRC type
  Serial.println("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on");
  uint8_t headerType = SX126X_HEADER_EXPLICIT;                        // Explicit header mode
  uint16_t preambleLength = 12;                                       // Set preamble length to 12
  uint8_t payloadLength = 16;                                         // Initialize payloadLength to 15
  bool crcType = true;                                                // Set CRC enable
  LoRa.setLoRaPacket(headerType, preambleLength, payloadLength, crcType);

  // Set syncronize word for public network (0x3444)
  Serial.println("Set syncronize word to 0x3444");
  LoRa.setSyncWord(0x3444);

  Serial.println("\n-- LORA RECEIVER LISTEN --\n");
  
}

void loop() {

  LoRa.request();
  LoRa.wait();
  // Check for incoming LoRa packet
  // if(lora_flag){
    // lora_flag = false;
    // Put received packet to message and counter variable
    // const uint8_t msgLen = LoRa.available();
    // if (msgLen) {
      const uint8_t msgLen = LoRa.available();
      char message[msgLen];
      uint8_t counter;
      uint8_t i=0;
      while (LoRa.available()){
        message[i++] = LoRa.read();
      }
      // counter = LoRa.read();

      // Print received message and counter in serial
      Serial.print(message);
      Serial.print("  ");
      Serial.println(counter);

      // Print packet/signal status including package RSSI and SNR
      Serial.print("Packet status: RSSI = ");
      Serial.print(LoRa.packetRssi());
      Serial.print(" dBm | SNR = ");
      Serial.print(LoRa.snr());
      Serial.println(" dB");

      // Show received status in case CRC or header error occur
      uint8_t status = LoRa.status();
      if (status == SX126X_STATUS_RX_TIMEOUT) Serial.println("Receive timeout");
      else if (status == SX126X_STATUS_CRC_ERR) Serial.println("CRC error");
      else if (status == SX126X_STATUS_HEADER_ERR) Serial.println("Packet header error");
      Serial.println();
    // }
  // }
 
}
