#include "Wire.h"
#include "SPI.h"
#include "FS.h"
#include "SD.h"

//0x72 A0!A1 0x73 pour A0A1 0x7X !A0A1
#define PCAADDR0 0x70
#define PCAADDR1 0x73
#define MPRADDR0 0x18
#define MPRADDR1 0x28

// #define pin_SDA 4
// #define pin_SCL 5

#define pin_RST 3

static File log_file;

void pcaselect(uint8_t i, uint8_t PCAADDR) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void pcaScanner(uint8_t PCAADDR){
  Serial.println("\nPCAScanner ready!");
  
  for (uint8_t t=0; t<4; t++) {
    pcaselect(1,PCAADDR);
    Serial.print("PCA Port #"); Serial.println(t);
    for (uint8_t i = 0; i<60; i++){
      Wire.beginTransmission(i);
      if (!Wire.endTransmission()) {
        Serial.print("Found I2C 0x");  Serial.println(i,HEX);
      }
    }
  }
  Serial.println("\ndone");
}

bool led_value = 0;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  
  Wire.begin();
  Wire.setClock(200000UL); //SET I2C clk to 30kHz

  if(!init_SD()){
    Serial.println("ERROR: init SD");
    pinMode(LED_BUILTIN, OUTPUT);
    while(1){
      digitalWrite(LED_BUILTIN, led_value);
      led_value = !led_value;
      delay(250);
    }
  }

  pcaselect(0, PCAADDR0);
  // pcaselect(0, PCAADDR1);
}

String com;

void loop() 
{ 
  if(Serial.available()){
    com = Serial.readString();
    if(com == "FIN"){
      log_file.close();
      esp_deep_sleep_start();
    } 
  }
  lecture_pipeline();
}