static void init_altimetre(){
  Serial.println("init altimètre!");

  MPL3115A2_Initialize();

  if (MPL3115A2_ID() != 0xC4) {  // L'ID attendu est 0xC4
    Serial.println("Could not find sensor. Check wiring.");
    while (1);
  }

  MPL3115A2_StandbyMode();    // Mettre en mode veille avant de configurer
  MPL3115A2_AltimeterMode();  // Mode altimètre pour mesurer l'altitude
  MPL3115A2_setSeaPressure(1028.0);
  MPL3115A2_ActiveMode();     // Passer en mode actif pour commencer les mesures

  delay(1000);

  Serial.print("Calibration Altimètre");
  for(int i=0; i<60; i++){
    baseAltitude += MPL3115A2_ReadAltitude();
    Serial.print(".");
    delay(100);
  }
  
  baseAltitude = baseAltitude/60;
  Serial.print("\n Altitude base: ");
  Serial.println(baseAltitude);
}

unsigned long lastTime_alti = 0;

static void read_altimetre(){
  if ((millis() - lastTime_alti ) >= 100) //To stream at 10Hz without using additional timers
  {
    lastTime_alti = millis();

    alt = MPL3115A2_ReadAltitude() - baseAltitude;
  }
}