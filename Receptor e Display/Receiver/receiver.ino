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


int CS_PIN = 53;                                // pino do arduino CS = 53, CS é a entrada do cartão de memoria
File file;                                      // variavel de armazenamento no cartão de memoria

String leitura = "";
String client = "";


  void setup()
  {
       Serial.begin(9600);
       startReceiver();
       startDisplay();
    

       //---------------------------------------SD-CARD---------------------------------------
            if (!SD.begin(CS_PIN)) {                                    // inicialização da conexão CS do cartão de memoria       
                  Serial.println("ERROR - Cartao de memoria FALHOU");   //
                  return;                                               //
             }                                                          // end
       //---------------------------------------FIM-------------------------------------------

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
         saveDataBase();
         printDisplay();
         Serial.print("Cliente....: ");
         Serial.println(client);
         Serial.print("Leitura...: ");
         Serial.println(leitura);
      }
      Serial.println("");
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
                display.setContrast(50);        //Ajusta o contraste do display
                display.clearDisplay();         //Apaga o buffer e o display
                display.setTextSize(1);         //Seta o tamanho do texto
                display.setTextColor(BLACK);    //Seta a cor do texto
    }   
/*___________________________________________________________ end _____________________________________________________*/



/*====== L O O P ()====================================== printDisplay() =================================*/
    void printDisplay(){
                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(8,0);         //Seta a posição do cursor
                    display.print("RECEPTOR");
                    
                    display.setCursor(0,15);
                    display.print("Cliente: ");
                    display.print(client);
                    display.display();
                    display.setTextSize(2);
                    display.setCursor(0,25);
                    display.print(leitura);
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
           for (int i = 0; i < buflen; i++) {                     // coleta todo dado do buffer recebido
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
      
      client += ".txt";                               // variavel para nome do arquivo txt conforme o cliente,
      file = SD.open(client, FILE_WRITE);             // abre arquivo com o nome do client

      if (file) {                                     // se o arquivo foi aberto, entre e grave os dados
          file.println(leitura);                           // escrevo no txt
          file.close();                                    // feche o arquivo
          Serial.println("Gravado com sucesso.");
          end = false;                                
      } else {                                        
          Serial.println("ERROR - Ao criar arquivo no banco de dados");
          end = false;
          delay(2000);
      }
    }                                             // end while
  }
/*=========================================================== end =================================================*/



