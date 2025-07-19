#define BNO055_SAMPLERATE_DELAY_MS (100)

static uint8_t systeme, gyro, accel, mag = 0;

static void init_adaBNO(){
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }

  bno.setExtCrystalUse(true);

  Serial.println("Calibrer l'accélero !");
  // while(accel < 3){
  //   Serial.print("En calibration: ");
  //   bno.getCalibration(&systeme, &gyro, &accel, &mag);
  //   Serial.println(accel, DEC);
  //   delay(BNO055_SAMPLERATE_DELAY_MS*2);
  // }
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