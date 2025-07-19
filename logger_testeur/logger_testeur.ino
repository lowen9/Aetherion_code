#include <HardwareSerial.h>

#define TX_PIN D9  // TX de l'ESP32-S3 Xiao
#define RX_PIN D10 // RX de l'ESP32-S3 Xiao

HardwareSerial mySerial(2);  // UART2

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Attente de l'ouverture du moniteur série

  mySerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);  // Initialisation UART1
  delay(1000);  // Laisser le temps au port UART de s'initialiser

  mySerial.flush();  // Vide le buffer UART

  Serial.println("Test loopback UART...");
}

void loop() {
  mySerial.println("Test UART ufiufoiogoufpupohmigluflu");

  delay(50);
}