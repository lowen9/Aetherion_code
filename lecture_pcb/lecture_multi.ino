void lecture_multi(){
  pcaselect(0);
  readPressure();
  // Serial.print(" Pression A1(hPa):");
  Serial.print("/*");
  Serial.print(pression);
  Serial.print(",");
  // Serial.println("");

  pcaselect(1);
  readPressure();
  // Serial.print(" Pression A2(hPa):");
  Serial.print(pression);
  Serial.print(",");
  // Serial.println("");

  pcaselect(2);
  readPressure();
  // Serial.print(" Pression A3(hPa):");
  Serial.print(pression);
  Serial.print(",");
  // Serial.println("");
  
  pcaselect(3);
  readPressure();
  // Serial.print(" Pression A4(hPa):");
  Serial.print(pression);
  Serial.println(",");
  // Serial.println("");
}