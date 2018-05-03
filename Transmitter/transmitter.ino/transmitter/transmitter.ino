#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
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
  
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);  
//---------------------------------------FIM-------------------------------------------



String client = "c-1";
int volume = 0;



void setup() {

  startTransmitter();
  startDisplay();
  attachInterrupt(0,readingVolume,RISING);        //interrupção programada no arduino
  
}

void loop() {
    printDisplay();
    delay(1000);
   
    sendCliente();
    sendLeitura();
}









/*_____ S E T U P()________________________________________ startTransmitter() ________________________________________*/
    void startTransmitter (){
      
                vw_set_ptt_inverted(true);      // Required for RF Link module
                vw_setup(2000);                 // Bits per sec
                vw_set_tx_pin(7);              // pin 7 ARDUINO-UNO
    }   
/*___________________________________________________________ end _____________________________________________________*/



/*_____ S E T U P()________________________________________ startDisplay() _____________________________________________*/
    void startDisplay (){
                
                display.begin();
                display.setContrast(50);        //Ajusta o contraste do display
    }   
/*___________________________________________________________ end _____________________________________________________*/







/*====== L O O P ()====================================== printDisplay() =================================*/
    void printDisplay(){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setTextColor(WHITE, BLACK); 
                    display.setCursor(0,0);         //Seta a posição do cursor
                    display.print("______________");
                    display.setCursor(0,1);        
                    display.print(" TRANSMISSOR  ");

                    display.setTextColor(BLACK);
                    display.setCursor(0,15);
                    display.print("Volume: ");
                    display.setTextSize(2);
                    display.setCursor(0,25);
                    
                    display.print(volume);
                    display.setTextSize(1);
                    display.setCursor(42,40);
                    display.print(" Kg/m^3");
                    display.display();
     }
/*=========================================================== end ========================================*/



/*====== L O O P ()====================================== readingVolume() =================================*/
    void readingVolume(){
      volume ++; 
    }
/*=========================================================== end ========================================*/



/*====== L O O P ()===================================== sendCliente() ==================================*/
    void sendCliente() {
    
    /* trecho de codigo para envio de dados transmissor -> recptor */
        char tosend[client.length() + 1];
        client.toCharArray(tosend, sizeof(tosend));
        vw_send((uint8_t *)tosend, client.length() + 1);
        vw_wait_tx();
    /* end */
    
        delay(200);
    }
/*================================================= end ================================================*/



/*======= L O O P ()=================================== sendLeitura() ==================================*/
    void sendLeitura() {
        
        String leitura = "0";
        leitura += String(volume);           //converte int em string
                                                                                          
    /* trecho de codigo para envio de dados transmissor -> recptor */
        char tosend[leitura.length() + 1]; 
        leitura.toCharArray(tosend, sizeof(tosend));
        vw_send((uint8_t *)tosend, leitura.length() + 1);
        vw_wait_tx();
    /* end */
    
        delay(200);
    }
/*================================================ end ================================================*/

