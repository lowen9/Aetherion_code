#include <HardwareSerial.h>
#include <SD.h>
#include <Wire.h>

#define TX_PIN D6  // TX de l'ESP32-S3 Xiao 
#define RX_PIN D7 // RX de l'ESP32-S3 Xiao

HardwareSerial mySerial(2);  // UART1 (évite conflit avec Serial USB)

static File log_file;
bool led_value = 0;

char file_base[8] = "log"; //log Ordinateur de bord 
int file_nb = 0;
char file_ext[4] = "txt";
char file_name[50];


void setup() {
  Serial.begin(115200);
  while (!Serial);  // Attente de l'ouverture du moniteur série

  mySerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);  // Initialisation UART1
  delay(1000);  // Laisser le temps au port UART de s'initialiser

  mySerial.flush();  // Vide le buffer UART
  
  Wire.begin();
  Wire.setClock(200000UL); //SET I2C clk to 30kHz

  if(!init_SD()){
    Serial.println("ERROR: init SD");
    pinMode(LED_BUILTIN, OUTPUT);
    while(1){
      digitalWrite(LED_BUILTIN, led_value);
      led_value = !led_value;
      delay(250);
    }
  }

  Serial.println("Test loopback UART...");
}

String com;

void loop() {

  if (mySerial.available()) {
    com = (char)mySerial.read();
    
    if(com == "SUPRIMER"){
      file_nb = 0;
      sprintf(file_name, "/%s%d.%s",file_base,file_nb,file_ext);
      Serial.println("Supression des fichiers...");
      while(searchFile(SD, file_name)){
        Serial.println(file_name);
        file_nb++;
        deleteFile(SD, file_name);
      }
      file_nb = 0;
    }

    if(com == "FIN"){
      log_file.close();
    }


    Serial.print(com);
    if(!log_file){
      Serial.println("Failed to open file for appending");
      return;   
    }
    (log_file.print(com)) ? Serial.println("Message appended"):
                            Serial.println("Append failed");
  }


}