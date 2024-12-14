#include <SPI.h>
#include <LoRa.h>

uint8_t LoRa_status = 0;

void init_led_lora(){
  pinMode(LED_BUILTIN, OUTPUT);
}

unsigned long lastTime_led = 0;

void loop_led_lora(){
  if ((millis() - lastTime_led) >= 500) //To stream at 1s without using additional timers
  {
    lastTime_led = millis();
    digitalWrite(LED_BUILTIN, 0);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  init_led_lora();
  init_lora();
}

void loop() {
  loop_lora();
  loop_led_lora();
}
