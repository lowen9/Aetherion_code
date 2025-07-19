double pression01 = 0;
double pression02 = 0;
double pression03 = 0;
double pression04 = 0;

double pression11 = 0;
double pression12 = 0;
double pression13 = 0;
double pression14 = 0;

// void lecture_multi(){
//   pcaselect(0, PCAADDR0);
//   pression01 = readPressure();

//   pcaselect(1, PCAADDR0);
//   pression02 = readPressure();

//   pcaselect(2, PCAADDR0);
//   pression03 = readPressure();

//   pcaselect(3, PCAADDR0);
//   pression04 = readPressure();

//   pcaselect(0, PCAADDR1);
//   pression11 = readPressure();

//   pcaselect(1, PCAADDR1);
//   pression12 = readPressure();

//   pcaselect(2, PCAADDR1);
//   pression13 = readPressure();

//   pcaselect(3, PCAADDR1);
//   pression14 = readPressure();

//   Serial.print("/*");
//   Serial.print(pression01);
//   Serial.print(",");
//   Serial.print(pression02);
//   Serial.print(",");
//   Serial.print(pression03);
//   Serial.print(",");
//   Serial.print(pression04);
//   Serial.print(",");
//   Serial.print(pression11);
//   Serial.print(",");
//   Serial.print(pression12);
//   Serial.print(",");
//   Serial.print(pression13);
//   Serial.print(",");
//   Serial.print(pression14);
//   Serial.println("");
// }

unsigned long start = 0;
unsigned long delta = 0;
char msgBuffer[200];

void lecture_pipeline(){ //Lecture pipeline des capteurs (la 1er lecture est fausse!)
  start = millis();
  pcaselect(0,PCAADDR0);
  pression01 = readPressureP(MPRADDR0);
  start_cap(MPRADDR0);

  pcaselect(1,PCAADDR0);
  pression02 = readPressureP(MPRADDR0);
  start_cap(MPRADDR0);

  pcaselect(2,PCAADDR0);
  pression03 = readPressureP(MPRADDR0);
  start_cap(MPRADDR0);

  pcaselect(3,PCAADDR0);
  pression04 = readPressureP(MPRADDR0);
  start_cap(MPRADDR0);

  pcaselect(0,PCAADDR1);
  pression11 = readPressureP(MPRADDR1);
  start_cap(MPRADDR1);

  pcaselect(1,PCAADDR1);
  pression12 = readPressureP(MPRADDR1);
  start_cap(MPRADDR1);

  pcaselect(2,PCAADDR1);
  pression13 = readPressureP(MPRADDR1);
  start_cap(MPRADDR1);

  pcaselect(3,PCAADDR1);
  pression14 = readPressureP(MPRADDR1);
  start_cap(MPRADDR1);

  sprintf(msgBuffer, "/*%d,%f,%f,%f,%f,%f,%f,%f,%f\n", start, pression01, pression02, pression03, pression04, pression11, pression12, pression13, pression14);

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

