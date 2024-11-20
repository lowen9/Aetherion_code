void lecture_multi(){
  pcaselect(0);
  readPressure();
  Serial.print(" Pression A2(hPa):");
  Serial.print(pression);
  Serial.println("");
  
  pcaselect(3);
  readPressure();
  Serial.print(" Pression A3(hPa):");
  Serial.print(pression);
  Serial.println("");
}