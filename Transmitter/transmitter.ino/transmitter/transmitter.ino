#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <VirtualWire.h>

#undef int
#undef abs
#undef double
#undef float
#undef round



//-----------------------------------CONFIG--DISPLAY-----------------------------------
  // pin 8 - Serial clock out (SCLK)
  // pin 9 - Serial data out (DIN)
  // pin 10 - Data/Command select (D/C)
  // pin 11 - LCD chip select (CS/CE)
  // pin 12 - LCD reset (RST)
  Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);  
//---------------------------------------FIM-------------------------------------------

String client = "@math";                                                                // global variable of customer identification
float volume = 188.00;                                                                  // global reading variable, adjusted with current meter
float volumeDisplay = 188.00;                                                           // global display variable, required to print only when reading varies
boolean INT = false;                                                                    // interrupt control variable













void setup() {
  
  Serial.begin(9600);
  
  startTransmitter();                                                                   // configure transmitter function
  startDisplay();                                                                       // function to configure the display
  printDisplay();                                                                       // function to print on the display

  attachInterrupt(0,readingVolume,RISING);                                              // programmed interrupt on arduino
}

void loop() {

    if (volumeDisplay != volume){                                                       // if there was a change in volume, between
          printDisplay();                                                               // print on the display the
          volumeDisplay = volume;                                                       // control variable equal to global variable
    }
    
    delay(1000);
    
    sendCliente();                                                                      // function to send client ID
    sendLeitura();                                                                      // function to send completed gas reading
    
    INT = false;                                                                        // insert interrupt to false;
}












/*_____ S E T U P()________________________________________ startTransmitter() ______*/ // configure transmitter function
    void startTransmitter (){
                vw_set_ptt_inverted(true);                                              // Required for RF Link module 
                vw_setup(2000);                                                         // Bits per sec
                vw_set_tx_pin(7);                                                       // pin 7 ARDUINO-UNO
    }/*___________________________________________________________ end ______________*/



/*_____ S E T U P()________________________________________ startDisplay() __________*/ // function to configure the display
    void startDisplay (){
                display.begin();                                                        // initializes the display      
                display.setContrast(50);                                                // Adjust the display contrast
    }/*___________________________________________________________ end ______________*/



/*====== L O O P ()====================================== readingVolume() ===========*/ // interrupt function for re-reading
    void readingVolume(){   
      if (!INT){                                                                        // if the interrupt was not called, enter
        INT = true;                                                                     // interrupt control arrow, true
        volume = 0.01 + volume;                                                         // volume is updated as interrupt
      }
    }/*=========================================================== end ==============*/



/*====== L O O P ()====================================== printDisplay() ============*/ // function to print on the display
    void printDisplay(){                                                                
                    display.clearDisplay();                                             // Clears the buffer and display
                    display.setTextSize(1);                                             // Insert text size
                    display.setTextColor(WHITE, BLACK);                                 // Insert the color of the text and background in the display
                    display.setCursor(0,0);                                             // Arrow cursor position
                    display.print("______________");                                    // Print data
                    display.setCursor(0,1);                                             // Insert cursor position
                    display.print(" TRANSMISSOR  ");                                    // Print data

                    display.setTextColor(BLACK);                                        // Insert black color in display text
                    display.setCursor(0,15);                                            // Insert cursor position
                    display.print("Volume: ");                                          // Print data
                    display.setTextSize(2);                                             // Insert text size
                    display.setCursor(0,25);                                            // Insert cursor position      
                    
                    display.print(volume);                                              // Print data
                    display.setTextSize(1);                                             // Insert text size
                    display.setCursor(42,40);                                           // Insert cursor position
                    display.print(" Kg/m^3");                                           // Print data
                    display.display();                                                  // Print data
     }/*=========================================================== end =============*/



/*====== L O O P ()===================================== sendCliente() ==============*/ // function to send client ID
    void sendCliente() {
        char tosend[client.length() + 1];                                               // array to receive data transmission
        client.toCharArray(tosend, sizeof(tosend));                                     // character array receives value for sending
        vw_send((uint8_t *)tosend, client.length() + 1);                                // sending characters
        vw_wait_tx();                                                                   // function to wait for transmission
   
        delay(200);
    }/*================================================= end =========================*/



/*======= L O O P ()=================================== sendLeitura() ================*/ // function to send completed gas reading
    void sendLeitura() {

        String leitura = "#";                                                           // identification to identify read at receiver
        leitura += String(volume);                                                      // converte int em string
                                                                                          
        char tosend[leitura.length() + 1];                                              // array to receive data transmission
        leitura.toCharArray(tosend, sizeof(tosend));                                    // character array receives value for sending
        vw_send((uint8_t *)tosend, leitura.length() + 1);                               // sending characters
        vw_wait_tx();                                                                   // function to wait for transmission
    
        delay(200);
    }/*================================================ end ==========================*/

