#include "Wire.h"

#define PCAADDR 0x70
#define MPRADDR 0x18

// #define pin_SDA 8
// #define pin_SCL 9

#define pin_RST 4

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

uint8_t data[7]; //Stockage des valeurs envoyé par le capteur
uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent
double pression = 0;
double temperature = 0;
double outputmax = 15099494; // output at maximum pressure [counts]
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 25; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]
float PSI_to_hPa = 68.947572932; // Constant: PSI to hPa conversion

void readPressure(){
  double press_counts = 0; // digital pressure reading [counts]
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
  pression = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  pression = pression*PSI_to_hPa;
  return;
}

// void readPresTemp(){
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
// }

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
  Wire.setClock(200000UL); //SET I2C clk to 30kHz

  // pcaScanner();

  // init_reset_pin();

  // reset_sensor();

  pcaselect(3);
}

void loop() 
{
  //test_pcb_v1();

  lecture_multi();
}