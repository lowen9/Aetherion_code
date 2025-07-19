#define SCLK_PIN 5
#define MISO_PIN 3
#define MOSI_PIN 6
#define CS_PIN 7
#define VSPI FSPI

#define LORA_RST_PIN 8
#define LORA_BUSY_PIN 34
#define LORA_DIO 33

#define FREQUENCY 865937500

SX126x LoRa;

SPIClass *vspi = NULL;

static void init_lora(){
  digitalWrite(LED_BUILTIN, 0);

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
  
  // Set frequency to 868 Mhz
  Serial.println("Set frequency to 868 Mhz");
  LoRa.setFrequency(FREQUENCY);

  // Set RX gain to boosted gain
  Serial.println("Set RX gain to boosted gain");
  LoRa.setRxGain(SX126X_RX_GAIN_BOOSTED);

  // Configure modulation parameter including spreading factor (SF), bandwidth (BW), and coding rate (CR)
  Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
  uint8_t sf = 7;
  uint32_t bw = 250000;
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

  // LoRa.onReceive(irq_lora); //Appel de la routine d'interruption;
  Serial.println("init LoRa succeeded");
}

void irq_lora(int packetSize){
  // if(!packetSize) return;

  // lora_flag = true;
  // sender = LoRa.read();
  // msg_lenght = LoRa.read();

  // incoming = "";

  // while(LoRa.available()){
  //   incoming += (char)LoRa.read();
  // }

  // if(msg_lenght != incoming.length()){
  //   lora_flag = false;
  //   return;
  // }

  // if(sender != sender_addr){
  //   lora_flag = false;
  //   return;
  // }

  // lora_SRRI = LoRa.packetRssi();

  //   return;
}

unsigned long lastTime_dlora = 0;

static byte msg_lenght = 0;
static byte gps_status = 0;

String incoming = "";
String msg = "";
String default_msg = "0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0";
// String default_msg = "0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0";

void lora_update(){
  if ((millis() - lastTime_dlora) >= 80) //To stream at 12,5Hz without using additional timers
  {
    lastTime_dlora = millis();

    LoRa.request();
    LoRa.wait(150);

    lora_status = 0;
    msg_lenght = LoRa.read();
    gps_status = LoRa.read();

    incoming = "";
    while(LoRa.available()){
      if(!lora_status) lora_status = 1;
      incoming += (char)LoRa.read();
    }

    if(incoming.length()) msg = incoming;

    if(msg_lenght != incoming.length()){
      return;
    }

    // Serial.print("lml: ");
    // Serial.print(msg_lenght);
    // Serial.print(",");
    // Serial.print("lmc: ");
    // Serial.print(incoming.length());
    // Serial.print(",");
  }
}

unsigned long lastTime_clora = 0;

void lora_print(){
  if ((millis() - lastTime_clora) >= 25) //To stream at 10Hz without using additional timers
  {
    lastTime_clora = millis();

    Serial.print("$Aetherion: ");
    lora_status ? Serial.print(gps_status) : Serial.print("0");
    Serial.print(",");
    lora_status ? Serial.print(msg) : Serial.print(default_msg);
    Serial.print(",");
    Serial.print(lora_status);
    Serial.print(",");
    lora_status ? Serial.print(LoRa.packetRssi()) : Serial.print("NaN");
    Serial.println(" */");

  }
}