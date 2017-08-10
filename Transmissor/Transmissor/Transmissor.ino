#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  
  Serial.begin(9600);
  mySwitch.enableTransmit(10);
  delay(500);
}

void loop() {

  int val = 5;
  Serial.println("enviando");
  mySwitch.send(val,24);
  delay (1000);


}
