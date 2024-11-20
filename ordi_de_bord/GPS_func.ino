#define GPS_RX_PIN 44
#define GPS_TX_PIN 43
#define GPS_BAUD 9600

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN); //Rx, Tx serial for GPS
TinyGPSPlus gps; //The TinyGPSPlus object

static void GPSDelay(unsigned long ms)
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
  delay(100);
  if(!gpsSerial.available()){
    Serial.println("Starting GPS Failed!");
  }
  delay(100);
}

static void read_GPS(){
  if(gpsSerial.available()){
    if(BUG) BUG = 0;
    if(gps.encode(gpsSerial.read())){
      if(gps.location.isValid()){
        if(!gps_status) gps_status = 1;
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        alt = gps.altitude.meters();
        speed = gps.speed.kmph();
      }
      else{
        gps_status = 0;
        latitude = 0;
        longitude = 0;
        alt = 0;
        speed = 0;
        Serial.println("INVALID");
      }
    }
  }

  if(millis() >5000 && gps.charsProcessed() < 10){
    Serial.println("No GPS detected: check wiring.");
    BUG = 1;
  }

  GPSDelay(200);
}