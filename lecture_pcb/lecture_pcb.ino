#include "Wire.h"

#define PCAADDR 0x70
#define MPRADDR 0x18

// #define pin_SDA 4
// #define pin_SCL 5

#define pin_RST 3

void pcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void pcaScanner(){
  Serial.println("\nPCAScanner ready!");
  
  for (uint8_t t=0; t<4; t++) {
    pcaselect(1);
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

uint8_t status;

uint8_t readStatus(){
  Wire.requestFrom(MPRADDR, 1);
  return Wire.read();
}

// double temperature = 0;

void readPresTemp(){
  //   double press_counts = 0; // digital pressure reading [counts]
  //   double temp_counts = 0; // digital temperature reading [counts]
  //   Wire.beginTransmission(MPRADDR);
  //   Wire.write (cmd, 3); // write command to the sensor
  //   Wire.endTransmission();
  //   delay(10);

  //   Wire.requestFrom(MPRADDR, 7); // read back Sensor data 7 bytes
  //   for (int i=0; i<7; i++){
  //     data[i] = Wire.read();
  //   }
  //   press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  //   pression = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  //   pression = pression*PSI_to_hPa;

  //   temp_counts = data[6] + data[5] * 256 + data[4] * 65536; // calculate digital temperature counts
  //   temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
  //   return;
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  
  Wire.begin();
  Wire.setClock(200000UL); //SET I2C clk to 30kHz

  pcaselect(0);
}

void loop() 
{
  lecture_pipeline();
}