#define LOG_BAUD 115200

void init_log(){
  Serial.println("init logger");
  logSerial.begin(LOG_BAUD, SERIAL_8N1, -1,LOG_TX_PIN);
  delay(1000);
  logSerial.flush();  // Vide le buffer UART
}