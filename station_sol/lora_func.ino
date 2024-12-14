// #define SCLK_PIN 5
// #define MISO_PIN 19
// #define MOSI_PIN 27
#define CS_PIN 18
// #define VSPI FSPI

#define LORA_RST_PIN 14
#define LORA_DIO 26

#define FREQUENCY 433E6

SPIClass *vspi = NULL;


static void init_lora(){
  digitalWrite(LED_BUILTIN, 0);
  // vspi = new SPIClass(VSPI);
  // vspi->begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

  LoRa.setPins(CS_PIN, LORA_RST_PIN,LORA_DIO);
  // LoRa.setSPI(*vspi);

  LoRa.setGain(0);               //de base 0 pour le LNA
  LoRa.setTxPower(10);            //10dBm = 10mW, 14dBm ~= 25mW
  LoRa.setSpreadingFactor(10);    //de base 7
  LoRa.setSignalBandwidth(20.8E3); //de base 125E3
  LoRa.crc();                     //de nocrc()

  Serial.println("init LoRa Sensor");

  if(!LoRa.begin(FREQUENCY)){
    Serial.println("Starting LoRa Failed!");
    delay(1000);
  }

  LoRa.onReceive(irq_lora); //Appel de la routine d'interruption

  LoRa.receive();

  Serial.println("init LoRa succeeded");
}

static byte sender_addr = 0xAA;
static bool lora_flag = false;
static byte msg_lenght = 0;
static byte sender = 0;

static uint8_t gps_status = 0;

String incoming = "";
static int lora_SRRI = 0;

void irq_lora(int packetSize){
  if(!packetSize) return;

  lora_flag = true;
  sender = LoRa.read();
  msg_lenght = LoRa.read();

  incoming = "";

  while(LoRa.available()){
    incoming += (char)LoRa.read();
  }

  if(msg_lenght != incoming.length()){
    lora_flag = false;
    return;
  }

  if(sender != sender_addr){
    lora_flag = false;
    return;
  }

  lora_SRRI = LoRa.packetRssi();

  return;
}

void loop_lora(){
  if(!lora_flag) return;

  digitalWrite(LED_BUILTIN, 1);

  lora_flag = false;

  // Serial.print("lml: ");
  // Serial.print(msg_lenght);
  // Serial.print(",");
  // Serial.print("lmc: ");
  // Serial.print(incoming.length());
  // Serial.print(",");

  Serial.print("$Aetherion: ");
  Serial.print(incoming);
  Serial.print(",");
  Serial.print(lora_SRRI);
  Serial.println(" */");
  return;
}