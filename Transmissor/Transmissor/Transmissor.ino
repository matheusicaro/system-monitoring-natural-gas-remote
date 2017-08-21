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
  //cliente Identificação == 1000
  int cliente = 1000;
  int leitura = 3200;
  
  mySwitch.send(cliente, 24);
  delay(3000);
  mySwitch.send(leitura, 24);
  
  leitura = leitura + 20;
  delay(6000);

}
