#define SCLK_PIN 5
#define MISO_PIN 3
#define MOSI_PIN 6
#define CS_PIN 7
#define VSPI FSPI

#define LORA_RST_PIN 8
#define LORA_DIO 33
#define FREQUENCY 433E6 

SPIClass *vspi = NULL;

static void init_lora(){
  vspi = new SPIClass(VSPI);
  vspi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  LoRa.setPins(CS_PIN, LORA_RST_PIN,LORA_DIO);
  LoRa.setSPI(*vspi);

  LoRa.setTxPower(20);            //10dBm = 10mW, 14dBm ~= 25mW
  LoRa.setSpreadingFactor(10);    //de base 7
  LoRa.setSignalBandwidth(20.8E3); //de base 125E3
  LoRa.crc();                     //de nocrc()

  Serial.println("init LoRa Sensor");

  if(!LoRa.begin(FREQUENCY)){
    Serial.println("Starting LoRa Failed!");
    BUG_LoRa = 1;
  }
}

unsigned long lastTime_dlora = 0;
char msgBuffer[200];
byte local_addr = 0xAA;

static void lora_data(){
  if ((millis() - lastTime_dlora) >= 50) //To stream at 20Hz without using additional timers
  {
    lastTime_dlora = millis();

    if(BUG_LoRa) BUG_LoRa = 0;

    sprintf(msgBuffer, "%d,%d,%.6f,%.6f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", gps_status, gps_number, latitude, longitude, alt, speed, euler.x(), euler.y(), euler.z(), acc.x(), acc.y(), acc.z(),hdop);
    Serial.println(msgBuffer); //For debuging
    LoRa.beginPacket();
    LoRa.write(local_addr);
    LoRa.write(strlen(msgBuffer));
    fed_GPS();
    LoRa.print(msgBuffer);
    LoRa.endPacket(); 
  }
}

unsigned long lastTime_slora = 0;
String new_msg = "Boujour";
String msg = "New";

static void send_lora(){
  if ((millis() - lastTime_slora) >= 100) //To stream at 5s without using additional timers
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
      LoRa.print(msg);
      LoRa.endPacket(); 
    }
  }
}