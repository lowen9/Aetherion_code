// uint8_t readStatus(){
//   Wire.requestFrom(MPRADDR, 1);
//   return Wire.read();
// }

// String new_com = "Rien";
// String com = "CA3";

// void test_pcb_v1(){

//   if(com == "CA2") Serial.print("Capteur A2 |");
//   if(com == "CA1") Serial.print("Capteur A1 |");
//   if(com == "CA4") Serial.print("Capteur A4 |");
//   if(com == "CA3") Serial.print("Capteur A3 |");
//   readPressure();
//   Serial.print(" Pression(hPa):");
//   Serial.print(pression);
//   Serial.println("");
//   if(Serial.available()){
//     new_com = Serial.readString();
//     if(new_com != com){
//       if(new_com == "CA2") pcaselect(0);
//       if(new_com == "CA1") pcaselect(1);
//       if(new_com == "CA4") pcaselect(2);
//       if(new_com == "CA3") pcaselect(3);
//       if(new_com ==     "RESET") {
//         reset_sensor();
//         pcaselect(3);
//         com = "CA3";
//       }
//       if(new_com !=     "RESET") com = new_com;
//     }
    
//   }
// }

