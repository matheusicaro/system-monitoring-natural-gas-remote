//importa a biblioteca RCSwitch
#include <RCSwitch.h>

//Instacia a Biblioteca RF
RCSwitch mySwitch = RCSwitch();
 
 
void setup() {
  
  Serial.begin(9600);

  //Seta como habititado para enviar dados RF o pino 10
  mySwitch.enableTransmit(10);
  delay(50);
 
}
 
void loop(){
 
      mySwitch.send(5, 24);
      delay(1000);
}
