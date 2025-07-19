uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

void start_cap(uint8_t cp_nb){
  Wire.beginTransmission(cp_nb);
  Wire.write(cmd, 3); //write cmd to sensor
  Wire.endTransmission();
}

uint8_t data[7]; //Stockage des valeurs envoyé par le capteur
double outputmax = 15099494; // output at maximum pressure [counts]
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 25; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]
float PSI_to_hPa = 68.947572932; // Constant: PSI to hPa conversion

double readPressureP(uint8_t cp_nb){ //Pipile version of reading capteur
  double pression = 0;
  double press_counts = 0; // digital pressure reading [counts]

  Wire.requestFrom(cp_nb, 4);
  for(int i = 0; i < 4; i++){
    data[i] = Wire.read();
  }
  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  //calculation of pressure value according to equation 2 of datasheet
  pression = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  pression = pression*PSI_to_hPa;
  return pression;
}

double readPressure(uint8_t cp_nb){
  double pression = 0;
  double press_counts = 0; // digital pressure reading [counts]
  Wire.beginTransmission(cp_nb);
  Wire.write(cmd, 3); //write cmd to sensor
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(cp_nb, 4);
  for(int i = 0; i < 4; i++){
    data[i] = Wire.read();
  }
  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  //calculation of pressure value according to equation 2 of datasheet
  pression = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  pression = pression*PSI_to_hPa;
  return pression;
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