static void init_altimetre(){
  Serial.println("init altimètre!");

  if (!mpl.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  // set mode before starting a conversion
  Serial.println("Setting mode to altimeter (meter).");
  mpl.setMode(MPL3115A2_ALTIMETER);

  delay(1000);

  Serial.print("Calibration Altimètre");
  for(int i=0; i<30; i++){
    digitalWrite(pinLEDServo, i%2);
    // digitalWrite(pinDecollage, i%2);
    mpl.startOneShot();
    while(!mpl.conversionComplete());
    baseAltitude += mpl.getLastConversionResults(MPL3115A2_ALTITUDE);
    Serial.print(".");
  }
  
  baseAltitude = baseAltitude/30;
  Serial.print("\n Altitude base: ");
  Serial.println(baseAltitude);
}

unsigned long lastTime_alti = 0;

//For EWMA
float alt = 0.0;

float alpha = 0.4;
float alt_m = 0.0;

//For calculate the vertical speed;
unsigned long t_now;
unsigned long t_prev;
float alt_prev = baseAltitude;

float alpha_v = 0.2;
float v_z_prev = 0;


static void read_altimetre(){
  if ((millis() - lastTime_alti ) >= 400) //To stream at 10Hz without using additional timers
  {
    lastTime_alti = millis();

    mpl.startOneShot();

    if(mpl.conversionComplete()){
      t_now = millis();
      alt = mpl.getLastConversionResults(MPL3115A2_ALTITUDE) - baseAltitude;
      alt_m = alpha*alt + (1-alpha)*alt_prev;
      v_z = (alt_m-alt_prev)/((t_now - t_prev)/1000.0);
      v_z = alpha_v*v_z + (1-alpha_v)*v_z_prev;

      alt_prev = alt_m;
      t_prev = t_now;
      v_z_prev = v_z; 

      Serial.print("$seq: ");
      Serial.print(alt);
      Serial.print(",");
      Serial.print(alt_m);
      Serial.print(",");
      Serial.println(v_z);
      // Serial.print("pressure = "); Serial.print(pressure); Serial.println(" hPa");
      // Serial.print("temperature = "); Serial.print(temperature); Serial.println(" C");
    }
  }
}