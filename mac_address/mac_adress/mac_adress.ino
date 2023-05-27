#include <WiFi.h>


void setup(){
  Serial.begin(115200);
  //PP:JJ:CC:NN:EE:OO
  //Passar formato acima para o formato abaixo na estacao
  //{0xPP, 0xJJ, 0xCC, 0xNN, 0xEE, 0xOO}
}
 
void loop(){
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.println();
  delay(100);
}
