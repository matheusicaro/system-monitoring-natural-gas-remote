
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RCSwitch.h>

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS/CE)
// pin 3 - LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

RCSwitch mySwitch = RCSwitch();
int value = -1;

void setup()   
{
  Serial.begin(9600);
  display.begin();

 //seleciona o pino 0 que é na verdade o 2 no arduino, mas aqui tem que ser zero para ser o 2
  mySwitch.enableReceive(0);
}

void loop()
{
 //---------------------------------------DISPLAY---------------------------------------
      display.setContrast(60); //Ajusta o contraste do display
      display.clearDisplay();   //Apaga o buffer e o display
      display.setTextSize(1);  //Seta o tamanho do texto
      display.setTextColor(BLACK); //Seta a cor do texto
      display.setCursor(0,0);  //Seta a posição do cursor
      display.setTextColor(BLACK, WHITE);//Texto invertido - Branco com fundo preto
 //---------------------------------------FIM-------------------------------------------


      display.println("Start");//Escreve no display  
      display.display();
      delay(3000);

  if(mySwitch.available()){

      value = mySwitch.getReceivedValue();
      String myString = String (value);
      display.println("Recebido:");//Escreve no display
      display.display();
      display.println(myString);//Escreve no display  
      display.display();
      delay(3000);
      display.println("encerrando...");//Escreve no display
      display.display();
      delay(1500);
 
 } else{
     
      display.println("NADA...");//Escreve no display  
      display.display();
      delay(3000);
 }

   mySwitch.resetAvailable();
 
}
