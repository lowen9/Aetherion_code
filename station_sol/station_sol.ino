#include <SPI.h>
#include <SX126x.h>

bool lora_status = 0;

void init_led_lora(){
  pinMode(LED_BUILTIN, OUTPUT);
}

unsigned long lastTime_led = 0;
bool led_value = 0;

void loop_led_lora(){
  if ((millis() - lastTime_led) >= 500) //To stream at 2Hz without using additional timers
  {
    lastTime_led = millis();
    (lora_status) ? led_value = !led_value : led_value = 0;
    digitalWrite(LED_BUILTIN, led_value);
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  init_led_lora();
  init_lora();
}

void loop() {
  lora_update();
  lora_print();
  loop_led_lora();
}
