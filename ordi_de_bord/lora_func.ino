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

  LoRa.setGain(20);               //de base 17
  LoRa.setTxPower(10);            //10dBm = 10mW, 14dBm ~= 25mW
  LoRa.setSpreadingFactor(10);    //de base 7
  LoRa.setSignalBandwidth(7.8E3); //de base 125E3
  LoRa.crc();                     //de nocrc()

  Serial.println("init LoRa Sensor");

  if(!LoRa.begin(FREQUENCY)){
    Serial.println("Starting LoRa Failed!");
    delay(1000);
  }
}

unsigned long lastTime_dlora = 0;
static void lora_data(){
  if ((millis() - lastTime_dlora) >= 100) //To stream at 10Hz without using additional timers
  {
    lastTime_dlora = millis();
    // Serial.println("Send message to base");
    LoRa.beginPacket();
    //Gps status
    LoRa.print(gps_status);
    LoRa.print(",");
    //latitude
    LoRa.print(latitude,6);
    LoRa.print(",");
    //longitude
    LoRa.print(longitude,6);
    LoRa.print(",");
    //altitude
    LoRa.print(alt);
    LoRa.print(",");
    //vitesse
    LoRa.print(speed);
    LoRa.print(",");
    //gyro x
    LoRa.print(g_x);
    LoRa.print(",");
    //gyro y
    LoRa.print(g_y);
    LoRa.print(",");
    //gyro z
    LoRa.print(g_z);
    LoRa.print(",");
    //accel x
    LoRa.print(a_x);
    LoRa.print(",");
    //accel y
    LoRa.print(a_y);
    LoRa.print(",");
    //accel z
    LoRa.print(a_z);
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