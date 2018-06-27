#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <VirtualWire.h>    // you must download and install the VirtualWire.h to your hardware/libraries folder
#include <SD.h>
#include <SPI.h>

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

int pin = 49;
int CS_PIN = pin;                                // pino do arduino CS = 53, CS é a entrada do cartão de memoria
File file;                                      // variavel de armazenamento no cartão de memoria

String leitura = "";
String client = "";
String auxLeitura = "";
boolean auxSaveCard = "";

  void setup()
  {

       pinMode (pin, OUTPUT);
       digitalWrite (pin, HIGH);

       Serial.begin(9600);
       startReceiver();
       startDisplay();    

       //---------------------------------------SD-CARD---------------------------------------
            if (!SD.begin(CS_PIN)) {                                    // inicialização da conexão CS do cartão de memoria       
                  Serial.println("ERROR - Cartao de memoria FALHOU");   //
                  return;                                               //
             }                                                          // end
       //---------------------------------------FIM-------------------------------------------
       Serial.println("SETUP: Ok");
  }


  void loop() {
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];              // variaveis buf da biblioteca VM 433MHz
    uint8_t buflen = VW_MAX_MESSAGE_LEN;          // variaveis buf da biblioteca VM 433MHz
    
    client = "";                            
    
    if (vw_get_message(buf, &buflen)){            // se receber sinal do transmissor, entre
      if(buf[0] == 'c'){                              // analisa qual cliente esta enviando
         for (int i = 0; i < buflen; i++) {           // coleta todo dado do buffer recebido
           client += ((char)buf[i]);                  //
         }                                            // end if

         
         receiveData();
         printDisplay();                  
         saveDataBase();
         printSerial();                  
         
         delay(2000);
      }
     //Serial.println("");
    }                                             // end if
  }




/*_____ S E T U P()________________________________________ startReceiver() _____________________________________________*/
    void startReceiver (){
      
            vw_set_ptt_inverted(true);                    // Required for RX Link Module
            vw_setup(2000);                               // Bits per sec
            vw_set_rx_pin(2);                             // We will be receiving on pin 4 i.e the RX pin from the module connects to this pin.
            vw_rx_start();                                // Start the receiver
    }   
/*___________________________________________________________ end _____________________________________________________*/



/*_____ S E T U P()________________________________________ startDisplay() _____________________________________________*/
    void startDisplay (){
 
           display.begin();
           display.setContrast(60);        //Ajusta o contraste do display
           display.clearDisplay();         //Apaga o buffer e o display
           display.setTextSize(1);         //Seta o tamanho do texto
           display.setTextColor(BLACK);    //Seta a cor do texto
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
                    display.print("   RECEPTOR   ");

                    display.setTextColor(BLACK);
                    display.setCursor(0,15);
                    display.print("Cliente: ");
                    display.print(client);
                    
                    display.setTextSize(2);
                    display.setCursor(0,25);
                    display.print(leitura);
                    display.setTextSize(1);
                    display.setCursor(42,40);
                    display.print(" Kg/m^3");
                    display.display();
     }
/*=========================================================== end ========================================*/



/*====== L O O P ()====================================== receiveData() ===============================================*/
void receiveData(){                    // funcao para receber leitura do gas, enviada pelo transmissor 433MHz           

  boolean recebido = false;
  leitura = "";
  
  while(!recebido){                                  // loop para aguardar a leitura a ser enviada
    uint8_t buf[VW_MAX_MESSAGE_LEN];                        // variaveis buf da biblioteca VM 433MHz
    uint8_t buflen = VW_MAX_MESSAGE_LEN;                    // variaveis buf da biblioteca VM 433MHz
    if (vw_get_message(buf, &buflen)){
       if(buf[0] == '0'){                                   // verifica se o primeiro char é 0, codigo da leitura
           for (int i = 1; i < buflen; i++) {                     // coleta todo dado do buffer recebido
             leitura += ((char)buf[i]);                           //
           }                                                      // end for
       }                                                    //end if
    }
    if(leitura.length() > 0)                                 // defini loop até vir a leitura e não outro dado em sequencia
      recebido = true;
  }                                                  // end while
}
/*=========================================================== end ====================================================*/



/*====== L O O P ()====================================== saveDataBase() =================================*/
  void saveDataBase(){                        // funcao para gravar dados no cartao de memoria
    boolean end = true;
    
    while(end){                                   // enquanto não gravar os dados, continue
      String save = client;                               
      save += ".txt" ;                            // variavel para nome do arquivo txt conforme o cliente,
      file = SD.open(save, FILE_WRITE);             // abre arquivo com o nome do client

      if (file) {                                     // se o arquivo foi aberto, entre e grave os dados
          file.println(leitura);                           // escrevo no txt
          file.close();                                    // feche o arquivo
          end = false;
          auxSaveCard = true;                                
      } else {                                        
          end = false;
          auxSaveCard = false;
          delay(2000);
      }
    }                                             // end while
  }
/*=========================================================== end =================================================*/

   void printSerial(){                  
        
        if( auxLeitura != leitura){
             Serial.println("");
             Serial.println("Dados Recebido");
             Serial.print("Cliente...: ");
             Serial.println(client);
             Serial.print("Leitura...: ");
             Serial.println(leitura);
             auxLeitura = leitura;
            
            if (auxSaveCard){
               Serial.println("SD-Card: Success");
            }else{
                Serial.println("FAILED: Creat archive in data base");
            }
        }
   }
