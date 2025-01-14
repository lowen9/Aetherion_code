#define SCLK_PIN 5
#define MISO_PIN 3
#define MOSI_PIN 6
#define CS_PIN 7
#define VSPI FSPI

#define LORA_RST_PIN 8
#define LORA_BUSY_PIN 34
#define LORA_DIO 33

#define FREQUENCY 868E6 

SX127x LoRa;

SPIClass *vspi = NULL;

static void init_lora(){
  vspi = new SPIClass(VSPI);
  vspi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  LoRa.setSPI(*vspi, 16E6);

  LoRa.setPins(CS_PIN, LORA_RST_PIN, LORA_BUSY_PIN, LORA_DIO);

  Serial.println("init LoRa Sensor");
  if(!LoRa.begin()){
    Serial.println("Starting LoRa Failed!");
    BUG_LoRa = 1;
  }

  // Set frequency to 868 Mhz
  Serial.println("Set frequency to 868 Mhz");
  LoRa.setFrequency(868E6);
  
  // Set TX power, this function will set PA config with optimal setting for requested TX power
  Serial.println("Set TX power to +25 dBm");
  LoRa.setTxPower(25, SX127X_TX_POWER_PA_BOOST); // TX power +25 dBm using PA boost pin

  Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
  LoRa.setSpreadingFactor(7);                    // LoRa spreading factor: 7
  LoRa.setBandwidth(250E3);                      // Bandwidth: 125 kHz
  LoRa.setCodeRate(5);                           // Coding rate: 4/5

  Serial.println("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on");
  LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);                       // Explicit header mode
  LoRa.setPreambleLength(12);                                       // Set preamble length to 12
  LoRa.setPayloadLength(16);                                        // Initialize payloadLength to 15
  LoRa.setCrcEnable(true);                                          // Set CRC enable

  Serial.println("Set syncronize word to 0x34");
  LoRa.setSyncWord(0x34);

  // Serial.print("waiting for radio ");
  // while(!LoRa.beginPacket()){
  //   Serial.print(".");
  //   delay(10);
  // }
}

unsigned long lastTime_dlora = 0;
char msgBuffer[200];

static void lora_data(){
  if ((millis() - lastTime_dlora) >= 80) //To stream at 12,5Hz without using additional timers 
  {
    lastTime_dlora = millis();

    sprintf(msgBuffer, "%d,%d,%.6f,%.6f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", gps_number, latitude, longitude, alt, speed, euler.x(), euler.y(), euler.z(), acc.x(), acc.y(), acc.z(),hdop);
    Serial.print(gps_status); //For debuging
    Serial.print(",");
    Serial.println(msgBuffer); 

    LoRa.beginPacket();
    LoRa.write(strlen(msgBuffer));
    LoRa.write(gps_status);
    fed_GPS();
    LoRa.write(msgBuffer,strlen(msgBuffer));
    LoRa.endPacket();

  }
}

unsigned long lastTime_slora = 0;
String new_msg = "Boujour";
String msg = "New";

static void send_lora(){
  if ((millis() - lastTime_slora) >= 50) //To stream at 5s without using additional timers
  {
    lastTime_slora = millis();
    if(Serial.available()){
      new_msg = Serial.readString();
    }
    else{
      Serial.println("Lora Not available");
    }
    if(new_msg != msg){
      msg = new_msg;
      Serial.print("Sending message: ");
      Serial.println(msg);

      LoRa.beginPacket();
      // LoRa.write(msg, strlen(msg));
      LoRa.endPacket(); 
    }
  }
}