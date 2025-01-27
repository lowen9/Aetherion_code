uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

void start_cap(){
  Wire.beginTransmission(MPRADDR);
  Wire.write(cmd, 3); //write cmd to sensor
  Wire.endTransmission();
}

uint8_t data[7]; //Stockage des valeurs envoyé par le capteur
double outputmax = 15099494; // output at maximum pressure [counts]
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 25; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]
float PSI_to_hPa = 68.947572932; // Constant: PSI to hPa conversion

double readPressure(){
  double pression = 0;
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
  return pression;
}

double readPressureP(){ //Pipile version of reading capteur
  double pression = 0;
  double press_counts = 0; // digital pressure reading [counts]

  Wire.requestFrom(MPRADDR, 4);
  for(int i = 0; i < 4; i++){
    data[i] = Wire.read();
  }
  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  //calculation of pressure value according to equation 2 of datasheet
  pression = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  pression = pression*PSI_to_hPa;
  return pression;
}