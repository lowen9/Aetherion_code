#include <ESP32Servo.h>

#define TIMER1 18 //en second

#define pinJack D3
#define pinServo D10

Servo servo;

unsigned long timeDecollage = 0;

bool etatDecollage = false;

bool checkTimer(long T){
  return (millis() - timeDecollage > T) ? 1 : 0;
}

unsigned long timer_Interrupt = 0;
void check_Jack(){
  if ((millis() - timer_Interrupt ) >= 10) //Check jack pin at 1kHz
  {
    timer_Interrupt = millis();

    if(digitalRead(pinJack)){
        if(!etatDecollage){
        etatDecollage = 1;
        digitalWrite(LED_BUILTIN, 0);
        timeDecollage = millis();
        return;
      }
    }
    

    /*******************************************************************\
    | Que pour les test à enlever lors du vol simulé et du vrai vol !!! |
    \*******************************************************************/
    if(!digitalRead(pinJack)){
      if(etatDecollage){
        Serial.println("Reset de L'ESP32");
        esp_restart();
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  init_led();

  init_pin();

  while(digitalRead(pinJack));

  init_servo();
}

int val = 0;

void loop() {
  // DETECTION DECOLLAGE
  check_Jack(); 

  if(etatDecollage){
    Serial.print("Decollage : ON, attente T1\n");
    delay(TIMER1*1000);
    servo.write(140);
    delay(10000);
  }

  // if(Serial.available()){
  //   val = Serial.parseInt();
    
  // }
}
