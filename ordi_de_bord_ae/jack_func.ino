void init_jack(){
  Serial.println("init jack!");
  pinMode(JACK_PIN, INPUT);
  pinMode(JACK_OUT, OUTPUT);
  digitalWrite(JACK_OUT, 0);
}

unsigned long timer_Interrupt = 0;

void check_Jack(){
  if ((millis() - timer_Interrupt ) >= 10) //Check jack pin at 1kHz
  {
    timer_Interrupt = millis();

    (!digitalRead(JACK_PIN)) ? led_update(LEDJ_PIN, 1) : led_update(LEDJ_PIN,0);

    if(digitalRead(JACK_PIN)){
      led_update(LEDJ_PIN, 0);
      sprintf(rocket_status, "LIFTOFF");
      digitalWrite(JACK_OUT, 1);
    }
    else{
      led_update(LEDJ_PIN, 1);
      sprintf(rocket_status, "GROUND");
      digitalWrite(JACK_OUT, 0);
    }
    // Serial.println(rocket_status);
  }
}