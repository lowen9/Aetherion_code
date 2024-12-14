unsigned long lastTime_printd = 0;
void print_data(){
  if ((millis() - lastTime_printd) >= 100) //To stream at 10Hz without using additional timers
  {
    lastTime_printd = millis();
    // Serial.print("Gps status:");
    Serial.print("/*");
    Serial.print(gps_status);
    Serial.print(",");
    // Serial.print("latitute:");
    Serial.print(latitude,6);
    Serial.print(",");
    // Serial.print("longitude:");
    Serial.print(longitude,6);
    Serial.print(",");
    // Serial.print("altitude:");
    Serial.print(alt);
    Serial.print(",");
    // Serial.print("speed:");
    Serial.print(speed);
    Serial.print(",");

    // // Serial.print("gx:");
    // Serial.print(g_x);
    // Serial.print(",");
    // // Serial.print("gy:");
    // Serial.print(g_y);
    // Serial.print(",");
    // // Serial.print("gz:");
    // Serial.print(g_z);
    // Serial.print(",");

    // // Serial.print("ax:");
    // Serial.print(a_x);
    // Serial.print(",");
    // // Serial.print("ay:");
    // Serial.print(a_y);
    // Serial.print(",");
    // // Serial.print("az:");
    // Serial.print(a_z);

    Serial.print("\n");
    
  }
}