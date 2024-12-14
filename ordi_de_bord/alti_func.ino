

static void init_altimetre(){
  Serial.println("init altimètre!");

  if(!baro.begin()){
    Serial.println("Could not find sensor. Check wiring.");
    return;
  }

  // use to set sea level pressure for current location
  // this is needed for accurate altitude measurement
  // STD SLP = 1013.26 hPa
  baro.setSeaPressure(1013.25);

  Serial.print("Calibration altimètre: ");
  for(int i = 0; i < 40; i++){
    baseAltitude += baro.getAltitude();
    Serial.print(".");
  }
  baseAltitude /= 40; 
  Serial.print("Base Altitude = "); Serial.print(baseAltitude); Serial.println(" m");
  Serial.println("-----------------");
  Serial.println("-----------------");
}

unsigned long lastTime_alti = 0;

static void read_altimetre(){
  if ((millis() - lastTime_alti ) >= 50) //To stream at 20Hz without using additional timers
  {
    lastTime_alti = millis();

    alt = baro.getAltitude() - baseAltitude ;
    // pressure = baro.getPressure();
    temperature = baro.getTemperature();

    // Serial.println("-----------------");
    // Serial.print("altitude = "); Serial.print(altitude); Serial.println(" m");
    // Serial.print("pressure = "); Serial.print(pressure); Serial.println(" hPa");
    // Serial.print("temperature = "); Serial.print(temperature); Serial.println(" C");
  }
}