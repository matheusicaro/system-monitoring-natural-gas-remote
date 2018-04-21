#include <LCD5110_Basic.h>
#include <VirtualWire.h>

#undef int
#undef abs
#undef double
#undef float
#undef round

//-----------------------------------CONFIG--DISPLAY------------------------------------
  // pin 8 - Serial clock out (SCLK)
  // pin 9 - Serial data out (DIN)
  // pin 10 - Data/Command select (D/C)
  // pin 11 - LCD chip select (CS/CE)
  // pin 12 - LCD reset (RST)
  
  LCD5110 tela(8,9,10,12,11);
  
  extern uint8_t SmallFont[];
  extern uint8_t MediumNumbers[];
  extern uint8_t BigNumbers[];
//---------------------------------------FIM-------------------------------------------



String client = "c-1";
int volume = 0;



void setup() {
  Serial.begin(9600);

   //---------------------------------------DISPLAY---------------------------------------
            tela.InitLCD(60);

            tela.setFont(SmallFont); //Definindo a fonte
            tela.clrScr(); //Apaga o contéudo que estiver na tela
      
            tela.setContrast(60);
            tela.print("RECEPTOR", CENTER, 0);
            tela.print("Volume:", LEFT, 20);
            tela.setFont(MediumNumbers);

   //---------------------------------------FIM-------------------------------------------

    attachInterrupt(0,readingVolume,RISING);        //interrupção programada no arduino

   //---------------------------------------RF-433--------------------------------------
            vw_set_ptt_inverted(true);      // Required for RF Link module
            vw_setup(2000);                 // Bits per sec
            vw_set_tx_pin(7);              // pin 7 ARDUINO-UNO
   //---------------------------------------FIM-------------------------------------------

}

void loop() {
    
    tela.setFont(MediumNumbers);
    tela.printNumI(0, CENTER, 30);
    delay(2000);

    //Serial.println(volume/2);   //divisão necessaria para relaçao da media
    
    sendCliente();
    sendLeitura();
    
}

/*----------------------------------------------------------------------------------------*/
void readingVolume(){
  delay(20000);               //ajuste delay para relação da medida coletada
  volume ++; 
}
/*----------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------*/
void sendCliente() {

/********* trecho de codigo para envio de dados transmissor -> recptor */
                                                                                      //delete delay(1000);
    char tosend[client.length() + 1];
    client.toCharArray(tosend, sizeof(tosend));
    vw_send((uint8_t *)tosend, client.length() + 1);
    vw_wait_tx();
/******** end */

    delay(200);
}
/*----------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------*/
void sendLeitura() {
    
    String leitura = "0";
    leitura += String(volume/2);           //converte int em string
                                                                                      //delete delay(1000);
    
/********* trecho de codigo para envio de dados transmissor -> recptor */
    char tosend[leitura.length() + 1]; 
    leitura.toCharArray(tosend, sizeof(tosend));
    vw_send((uint8_t *)tosend, leitura.length() + 1);
    vw_wait_tx();
/********* end */

    delay(200);
}
/*----------------------------------------------------------------------------------------*/
