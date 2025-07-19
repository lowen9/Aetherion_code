/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32‑S2 | ESP32‑S3 | ESP32‑C3 | ESP32‑C6 | ESP32‑H2 |
 * +==============+=========+=======+==========+==========+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO34   | GPIO10   | GPIO7    | GPIO18   | GPIO0    |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO35   | GPIO11   | GPIO6    | GPIO19   | GPIO25   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO37   | GPIO13   | GPIO5    | GPIO20   | GPIO11   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO36   | GPIO12   | GPIO4    | GPIO21   | GPIO10   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"

bool init_SD(){
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return 0;
  }
  Serial.printf("SD Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  return 1;
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

bool searchFile(fs::FS &fs, const char *path){
  Serial.printf("Searching file: %s\n", path);

  if(!fs.open(path)){
    Serial.println("No such file");
    return 0;
  }

  Serial.println("file found");
  return 1;
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

char file_base[8] = "logcp1b";
int file_nb = 0;
char file_ext[4] = "txt";
char file_name[50];

File log_file = SD.open(file_name, FILE_APPEND);

void setup() {
  Serial.begin(115200);
  delay(1000);

  if(!init_SD()){
    while(1);
  }

  sprintf(file_name, "/%s%d.%s",file_base,file_nb,file_ext);

  while(searchFile(SD, file_name)){
    file_nb++;
    readFile(SD, file_name);
    // while(!Serial.available());  //A décommenté si on veux pas faire de lecture
    // Serial.readString();
    // deleteFile(SD, file_name); //A commenté si on ne veux pas effecer les fichiers
    sprintf(file_name, "/%s%d.%s",file_base,file_nb,file_ext);
  }

 
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n\n", SD.usedBytes() / (1024 * 1024));

  appendFile(SD, file_name, "Time, C1, C2, C3, C4\n");

  // log_file = SD.open(file_name, FILE_APPEND);
}

char msgBuffer[200];
float c1_val = 0.0;
float c2_val = 0.0;
float c3_val = 0.0;
float c4_val = 0.0;

String com = "n";

void loop() {  

  // sprintf(msgBuffer, "%d,%f,%f,%f\n", millis(), c1_val, c2_val, c3_val, c4_val);
  
  // if (!log_file) {
  //   Serial.println("Failed to open file for appending");
  //   return;
  // }
  // if (log_file.print(msgBuffer)) {
  //   Serial.println("Message appended");
  // } else {
  //   Serial.println("Append failed");
  // }

  // c1_val += 0.01;
  // c2_val += 0.02;
  // c3_val += 0.03;
  // c4_val += 0.04;

  // if(Serial.available()){
  //   com = Serial.readString();
  //   if(com == "FIN") {
  //     log_file.close();
  //     while(1);
  //   }
  // }
  // delay(10);
}
