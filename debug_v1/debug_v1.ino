#include "Wire.h"

#define PCAADDR 0x70
#define MPRADDR 0x18

// #define pin_SDA 8
// #define pin_SCL 9

#define pin_RST 4

void i2cScanner(){
  Serial.println("\nI2CScanner ready!");
  for (uint8_t i = 0; i<120; i++){
    Wire.beginTransmission(i);
    if (!Wire.endTransmission()) {
      Serial.print("Found I2C 0x");  Serial.println(i,HEX);
    }
  }
  Serial.println("\ndone");
}

void pcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void pcaScanner(){
  Serial.println("\nPCAScanner ready!");
  
  for (uint8_t t=0; t<4; t++) {
    pcaselect(t);
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

uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent
uint8_t data[4]; //Stockage des valeurs envoyé par le capteur
double outputmax = 15099494; // output at maximum pressure [counts]
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 100; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]

double readPressure(uint8_t id){
  double press_counts = 0; // digital pressure reading [counts]
  double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
  Wire.beginTransmission(MPRADDR);
  Wire.write(cmd, 3); //write cmd to sensor
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(MPRADDR, 4);
  for(int i = 0; i < 4; i++){
    data[i] = Wire.read();
  }
  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  //calculation of pressure value according to equation 2 of datasheet
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

  // Serial.print("DEBUG | ");
  // Serial.print("data[0]:");
  // Serial.print(data[0]);
  // Serial.print(" data[1]:");
  // Serial.print(data[1]);
  // Serial.print(" data[2]:");
  // Serial.print(data[2]);
  // Serial.print(" data[3]:");
  // Serial.print(data[3]);
  // Serial.println(" ");
  return pressure;
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

void calibration_pressure(){
  Serial.print("Calibrating");
}

// standard Arduino setup()
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  
  Wire.begin();
  Wire.setClock(200000UL); //SET I2C clk to 20kHz
  
  i2cScanner();

  pcaselect(0);

  // init_reset_pin();

  // reset_sensor();
}

double pressur_moy = 0;
String new_com = "Rien";
String com = "CAPTEUR 0";

void loop() 
{
  for(int t = 0; t<4; t++){
    pcaselect(t);
    Serial.print("Switch to:");
    Serial.println(t);
    i2cScanner();
    delay(50);      
  }

}