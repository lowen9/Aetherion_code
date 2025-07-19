#include "Wire.h"

#define PCAADDR0 0x72 //0x73 A0A1 0x72 A0!A1 
#define PCAADDR1 0x73

#define PCAADDRC 0x73
#define PCAADDRC1 0x70

#define MPRADDR0 0x18
#define MPRADDR1 0x28

#define pin_RST 4

void i2cScanner(){
  Serial.println("\nI2CScanner ready!");
  for (uint8_t i = 0; i<255; i++){
    Wire.beginTransmission(i);
    if (!Wire.endTransmission()) {
      Serial.print("Found I2C 0x");  Serial.println(i,HEX);
    }
  }
  Serial.println("\ndone");
}

void pcaselect(uint8_t i, uint8_t PCAADDR) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void pcaScanner(uint8_t PCAADDR){
  Serial.println("\nPCAScanner ready!");
  
  for (uint8_t t=0; t<4; t++) {
    pcaselect(t, PCAADDR);
    Serial.print("PCA Port #"); Serial.println(t);
    for (uint8_t i = 0; i<255; i++){
      Wire.beginTransmission(i);
      if (!Wire.endTransmission()) {
        Serial.print("Found I2C 0x");  Serial.println(i,HEX);
      }
    }
  }
  Serial.println("\ndone");
}

void init_reset_pin(){
  pinMode(pin_RST, OUTPUT);
  digitalWrite(pin_RST, 1);
  return;
}

void reset_sensor(){
  Serial.println("Reseting...");
  digitalWrite(pin_RST, 0);
  delay(3);
  digitalWrite(pin_RST, 1);
  delay(3);
  return;
}

// standard Arduino setup()
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  
  Wire.begin();
  Wire.setClock(200000UL); //SET I2C clk to 20kHz
  
  // pcaselect(0, PCAADDR1);
  i2cScanner();

  // pcaselect(0, PCAADDR0);


  // pcaScanner(PCAADDR0);
  pcaScanner(PCAADDRC);
}

double pressur_moy = 0;
String new_com = "Rien";
String com = "CAPTEUR 0";

void loop() 
{
  for(int t = 0; t<4; t++){
    // // pcaselect(t, PCAADDR0);
    // // Serial.print("PCAADDR0 Switch to:");
    // // Serial.println(t);
    // // i2cScanner();
    
    // pcaselect(t, PCAADDRC);
    // Serial.print("PCAADDR1 Switch to:");
    // Serial.println(t);
    i2cScanner();  

    delay(2000);
  }

  // while(true);

}