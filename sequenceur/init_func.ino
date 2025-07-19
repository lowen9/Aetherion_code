void init_servo(){
  Serial.println("init servo");
  servo.attach(pinServo);
  servo.write(90);
}

void init_pin(){
  Serial.println("init pin");
  pinMode(pinJack,INPUT);
}

void init_led(){
  Serial.println("init led");
  pinMode(pinDecollage,OUTPUT);
  pinMode(pinLEDServo,OUTPUT);

  digitalWrite(pinLEDServo, 1);
  digitalWrite(pinDecollage, 1);
}