void init_led(){
  Serial.println("init led!");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);
  pinMode(LEDJ_PIN, OUTPUT);
  pinMode(LEDV_PIN, OUTPUT);
}

void led_update(uint8_t led, bool state){
  digitalWrite(led, state);
}