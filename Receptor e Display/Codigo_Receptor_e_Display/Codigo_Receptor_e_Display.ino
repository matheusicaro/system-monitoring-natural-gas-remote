
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RCSwitch.h>
#include <SD.h>
#include <SPI.h>

//_____________________________RECEPTOR
RCSwitch mySwitch = RCSwitch();
int value = -1;
//_____________________________FIM

//_____________________________SD-CARD
int CS_PIN = 53;
File file;
//_____________________________FIM

String dado;

void setup()   
{
  Serial.begin(9600);
  //display.begin();

//_____________________________SD-card
  if (!SD.begin(CS_PIN)) {
      Serial.println("Inicialização FALHOU");
     }
//_____________________________FIM

  
  //seleciona o pino 0 que é na verdade o 2 no arduino, mas aqui tem que ser zero para ser o 2
  mySwitch.enableReceive(0);
}

void loop()
{
    
      
      Serial.println("START"); 
      delay(3000);

//__________Transmissor__________________Recebe os dados
  if(mySwitch.available()){

      value = mySwitch.getReceivedValue();
      dado = String (value);
      Serial.print("Recebido: ");
      Serial.println(dado); 
      delay(1000); 
    
      Serial.println("Encerrando...");  
      delay(1000); 
  }
  else{
     
      Serial.println("NADA...");  
      //display.display();
      delay(2000);
 }
//__________________________________________FIM


//__________________________________________Armazenar no Banco

 file = SD.open("BD.txt", FILE_WRITE);
 if (file) {
    Serial.print("gravando no DB...");
    file.println(dado);
    file.close();
    
    Serial.println("Sucesso!");
    delay(1000);

  } else {
    Serial.println("ERROR_SD: Ao gravar");
    delay(1000);
  }

//__________________________________________FIM


  mySwitch.resetAvailable();
 
}
