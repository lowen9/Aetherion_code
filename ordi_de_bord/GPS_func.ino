#define GPS_BAUD 9600

static void GPSDelay(unsigned long ms) //bloquant ? --oui totalement
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms); 
}

static void init_GPS(){
  Serial.println("init GPS");
  gpsSerial.begin(GPS_BAUD);
  Serial.print("Starting GPS");

  unsigned int time_start = millis();

  while(!gpsSerial.available() && millis() < 5000 + time_start){
    Serial.print(".");
    delay(10);
  }
  if(!gpsSerial.available() && millis() >= 5000 + time_start){
    Serial.println("\nNo GPS data received: check wiring");
    while(1);
  }
  Serial.println("\nGPS Started !");
}

unsigned long lastTime_GPS = 0;

static void read_GPS(){
  if ((millis() - lastTime_GPS) >= 50) //To stream at 20Hz without using additional timers  50
  {
    lastTime_GPS = millis();

    // Serial.println(gps.failedChecksum());

    if(gpsSerial.available()){
      gps.encode(gpsSerial.read());
    }

    if(gps.location.isValid()){
      if(BUG_GPS) BUG_GPS = 0;
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      gps_number = gps.satellites.value();
    }
    else{
      // Serial.println("Gps invalid");
    } 
    gps_number ? gps_status = 1 : gps_status = 0;

    if(gps.altitude.isValid()) alt = gps.altitude.meters();
    if(gps.speed.isValid()) speed = gps.speed.kmph();
    if(gps.hdop.isValid()) hdop = gps.hdop.hdop();
  }
}

static void fed_GPS(){
  while(gpsSerial.available()){
    gps.encode(gpsSerial.read());
  }
}