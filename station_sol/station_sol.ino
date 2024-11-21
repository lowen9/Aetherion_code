#include <SPI.h>
#include <LoRa.h>

// #define SCLK_PIN 5
// #define MISO_PIN 19
// #define MOSI_PIN 27
#define CS_PIN 18
// #define VSPI FSPI

#define LORA_RST_PIN 14
#define LORA_DIO 26

#define FREQUENCY 433E6

SPIClass *vspi = NULL;
uint8_t LoRa_status = 0;

void init_led_lora(){
  pinMode(LED_BUILTIN, OUTPUT);
}

void init_lora(){
  digitalWrite(LED_BUILTIN, 0);
  // vspi = new SPIClass(VSPI);
  // vspi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  LoRa.setPins(CS_PIN, LORA_RST_PIN,LORA_DIO);
  // LoRa.setSPI(*vspi);

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

unsigned int packetSize = 0;
unsigned long lastTime_lora = 0;

void loop_lora(){
  if ((millis() - lastTime_lora) >= 10) //To stream at 100Hz without using additional timers
  {
    lastTime_lora = millis();

    packetSize = LoRa.parsePacket();
    if(packetSize){
      digitalWrite(LED_BUILTIN, 1);
      Serial.print("$Aetherion: ");
      while(LoRa.available()){
        Serial.print((char)LoRa.read());
      }
      Serial.print(",");
      Serial.print(LoRa.packetRssi());
      Serial.println(" */");
    }
    digitalWrite(LED_BUILTIN, 0);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  init_led_lora();
  init_lora();

}

void loop() {
  loop_lora();
}
