#include <LCD5110_Basic.h>


// pin 8 - Serial clock out (SCLK)
// pin 9 - Serial data out (DIN)
// pin 10 - Data/Command select (D/C)
// pin 11 - LCD chip select (CS/CE)
// pin 12 - LCD reset (RST)

LCD5110 tela(8,9,10,12,11);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

int volume = 0;

void setup()   
{
  tela.InitLCD(50);

 //---------------------------------------DISPLAY---------------------------------------

      tela.setFont(SmallFont); //Definindo a fonte
      tela.clrScr(); //Apaga o contéudo que estiver na tela

      tela.setContrast(60);
      tela.print("TRANSMISSOR", CENTER, 0);
      tela.print("Volume:", LEFT, 20);

 //---------------------------------------FIM-------------------------------------------
}

void loop()
{

      tela.setFont(MediumNumbers);
      tela.printNumI(volume, CENTER, 30);
      
      delay(2000);
      volume++;
}
