#include <Wire.h>
#include <libaltimetreV2.h>  // Inclure la bibliothèque MPL3115A2

#define INTERRUPT_PIN 16 

bool test = true;
int cmpt = 0;
float baseAltitude = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("MPL3115A2 test!");

  Wire.begin();
  // Initialisation du capteur MPL3115A2
  MPL3115A2_Initialize();
  
  // Vérification du capteur
  Serial.println(MPL3115A2_ID());
  if (MPL3115A2_ID() != 0xC4) {  // L'ID attendu est 0xC4
    Serial.println("Could not find sensor. Check wiring.");
    while (1);
  }

  MPL3115A2_StandbyMode();    // Mettre en mode veille avant de configurer
  MPL3115A2_AltimeterMode();  // Mode altimètre pour mesurer l'altitude
  MPL3115A2_setSeaPressure(1028.0);
  MPL3115A2_ActiveMode();     // Passer en mode actif pour commencer les mesures

  // Définir la pression de la mer (1013.25 hPa par défaut)
  // MPL3115A2_SetPressureAlarmThreshold(1013); // optionnel
  // MPL3115A2_EnableEventFlags();

  // Configurer l'interruption pour les données prêtes (INT1 ou INT2 selon ton modèle)
  // MPL3115A2_ConfigureInterruptPin(INT2, INTERRUPT_PIN); // Utilisation du pin d'interruption 2
  
  // Configurer l'interruption pour la disponibilité des données
  // MPL3115A2_ConfigureAltitudeInterrupt();  // Optionnel, si tu veux une interruption pour l'altitude
  // MPL3115A2_ConfigurePressureInterrupt();  // Optionnel, si tu veux une interruption pour la pression

  // Initialisation de la pin d'interruption
  // pinMode(INTERRUPT_PIN, INPUT_PULLUP);  // Activer une résistance pull-up pour la pin d'interruption

  Serial.println("-----------------");
  Serial.println("-----------------");
  delay(1000);

  baseAltitude = MPL3115A2_ReadAltitude();
}

void loop() {
  // Vérifier si une interruption a eu lieu
  // Serial.println(digitalRead(INTERRUPT_PIN));
  // if (digitalRead(INTERRUPT_PIN) == HIGH) {  // Interruption détectée (niveau bas sur la pin)
    // Lecture des données quand l'interruption se déclenche
    // float pressure = MPL3115A2_ReadPressure(Pa); // MPL3115A2_ReadPressure(Pa);  // pression en Pa
    // float altitude =  MPL3115A2_ReadAltitude() - baseAltitude; // altitude relative
    float altitude =  MPL3115A2_ReadAltitude(); // altitude relative
    // float temperature = MPL3115A2_ReadTemperature(); // température en Celsius

    // Affichage des résultats
    // Serial.print("Pression (Pa): "); Serial.println(pressure);
    // Serial.print("Altitude (m): "); Serial.println(altitude);
    Serial.print("$altitude");
    Serial.println(altitude);
    // Serial.print("Température (°C): "); Serial.println(temperature);
    // Serial.println("-----------------");
    // Serial.println("-----------------");
    // Attendre la fin de l'événement pour éviter de multiples lectures simultanées
    delay(100);  // Attente après l'interruption pour permettre le traitement du prochain événement
  // }
}
