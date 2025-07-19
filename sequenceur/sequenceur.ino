#include <ESP32Servo.h>

#include <Adafruit_MPL3115A2.h>

#define TAU 200
#define timerT1 20000-TAU
#define timerT2 24000-TAU

#define pinReset D0
#define pinJack D1
#define pinDecollage D2
#define pinLEDServo D3
#define pinServo D10

Servo servo;

Adafruit_MPL3115A2 mpl;

unsigned long timeDecollage = 0;

bool etatDecollage = false;
bool etatTimer_T1 = false;
bool etat_alti = false;
bool etatTimer_T2 = false;
bool etatParachute = false;

float baseAltitude = 0.0;
float v_z = 0;

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
        timeDecollage = millis();
        digitalWrite(pinDecollage, LOW);
        return;
      }
    }
    
    /*******************************************************************\
    | Que pour les test à enlever lors du vol simulé et du vrai vol !!! |
    \*******************************************************************/
    if(!digitalRead(pinJack)){
      if(etatParachute){
        Serial.println("Reset de L'ESP32");
        esp_restart();
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  init_pin();

  while(digitalRead(pinJack));

  init_led();

  init_servo();

  init_altimetre();
}

void loop() {
  // DETECTION DECOLLAGE
  check_Jack();

  if(!etatParachute){ //Si le parachute n'est pas sortie
    if(etatDecollage && !etatTimer_T1){
      Serial.print("Decollage : ON, attente T1\n");
      delay(timerT1); //Avant T1 on ne fait rien !
      etatTimer_T1 = 1;
    }
    //FENETRE D'ACTIONNEMENT PAR CAPTEUR DE PRESSION
    else if(etatDecollage && etatTimer_T1 && !etatTimer_T2){
      if(!etat_alti){ //Condition pour afficher le message qu'une fois
        Serial.print("Decollage : ON, attente T2\n");
        etat_alti = 1;
      }

      read_altimetre();

      if(v_z<-1.5 || checkTimer(timerT2)){ //checkTimer(timerT2)
        etatTimer_T2 = 1;
      }
    }
    //DECLENCHEMENT FORCE PAR TIMER T2
    else if(etatDecollage && etatTimer_T1 && etatTimer_T2){
      etatParachute = 1;
      digitalWrite(pinLEDServo, LOW);
      Serial.print("Decollage : ON, Parachute : ON \n");
      servo.write(0);

      delay(100);

      delay(10000); //Attente par sécurité pour le debugage
    }
  }
}
