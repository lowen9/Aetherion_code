void init_led(){
  Serial.println("init servo");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void init_servo(){
  Serial.println("init servo");
  servo.attach(pinServo);
  servo.write(115);
}

void init_pin(){
  Serial.println("init pin");
  pinMode(pinJack,INPUT);
}