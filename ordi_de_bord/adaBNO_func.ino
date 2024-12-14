#define BNO055_SAMPLERATE_DELAY_MS (100)

unsigned long lastTime_caliBNO = 0;

static uint8_t systeme, gyro, accel, mag = 0;

static void init_adaBNO(){
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    // BUG_BNO = 1;
  }

  bno.setExtCrystalUse(true);
  // bno.set

  Serial.println("Calibrer l'accélero !");
  while(accel < 3){
    Serial.print("En calibration: ");
    bno.getCalibration(&systeme, &gyro, &accel, &mag);
    Serial.println(accel, DEC);
    delay(BNO055_SAMPLERATE_DELAY_MS*2);
  }
}

unsigned long lastTime_BNO = 0;

static void read_adaBNO(){
  if ((millis() - lastTime_BNO) >= BNO055_SAMPLERATE_DELAY_MS) //To stream at 10Hz without using additional timers
  {
    lastTime_BNO = millis();

    acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    fed_GPS();
    euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  }
}