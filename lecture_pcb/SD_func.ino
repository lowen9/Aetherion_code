bool searchFile(fs::FS &fs, const char *path){
  Serial.printf("Searching file: %s\n", path);

  if(!fs.open(path)){
    Serial.println("No such file");
    return 0;
  }

  Serial.println("file found");
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

bool init_SD(){
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return 0;
  }
  Serial.printf("SD Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));

  sprintf(file_name, "/%s%d.%s",file_base,file_nb,file_ext);
  while(searchFile(SD, file_name)){
    file_nb++;
    // readFile(SD, file_name);
    // while(!Serial.available());  //A décommenté si on veux pas faire de lecture
    // Serial.readString();
    // deleteFile(SD, file_name); //A commenté si on ne veux pas effecer les fichiers
    sprintf(file_name, "/%s%d.%s",file_base,file_nb,file_ext);
    delay(1000);
  }

  appendFile(SD, file_name, "Time, C1, C2, C3, C4\n");

  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n\n", SD.usedBytes() / (1024 * 1024));

  log_file = SD.open(file_name, FILE_APPEND);
  
  return 1;
}





