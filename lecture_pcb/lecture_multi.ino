double pression1 = 0;
double pression2 = 0;
double pression3 = 0;
double pression4 = 0;

void lecture_multi(){
  pcaselect(0);
  pression1 = readPressure();

  pcaselect(1);
  pression2 = readPressure();

  pcaselect(2);
  pression3 = readPressure();

  pcaselect(3);
  pression4 = readPressure();

  Serial.print("/*");
  Serial.print(pression1);
  Serial.print(",");
  Serial.print(pression2);
  Serial.print(",");
  Serial.print(pression3);
  Serial.print(",");
  Serial.print(pression4);
  Serial.println("");
}

unsigned long start = 0;
unsigned long delta = 0;
char msgBuffer[200];

void lecture_pipeline(){ //Lecture pipeline des capteurs (la 1er lecture est fausse!)
  start = millis();
  pcaselect(0);
  pression1 = readPressureP();
  start_cap();

  pcaselect(1);
  pression2 = readPressureP();
  start_cap();

  pcaselect(2);
  pression3 = readPressureP();
  start_cap();

  pcaselect(3);
  pression4 = readPressureP();
  start_cap();

  sprintf(msgBuffer, "/*%d,%f,%f,%f,%f\n", start, pression1, pression2, pression3, pression4);

  Serial.print(msgBuffer);
  
  if (!log_file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (log_file.print(msgBuffer)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }

  delta = millis()-start;
  if(delta > 16) delta = 16;
  Serial.println(delta);
  delay(16-delta);
}

