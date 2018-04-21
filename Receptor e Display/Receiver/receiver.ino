#include <LCD5110_Basic.h>

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
  
  LCD5110 tela(8,9,10,12,11);
  
  extern uint8_t SmallFont[];
  extern uint8_t MediumNumbers[];
  extern uint8_t BigNumbers[];
//---------------------------------------FIM-------------------------------------------


int CS_PIN = 53;                                // pino do arduino CS = 53, CS é a entrada do cartão de memoria
File file;                                      // variavel de armazenamento no cartão de memoria

String leitura = "";
String client = "";


  void setup()
  {
  Serial.begin(9600);
       //---------------------------------------DISPLAY---------------------------------------
            tela.InitLCD(60);

            tela.setFont(SmallFont); //Definindo a fonte
            tela.clrScr(); //Apaga o contéudo que estiver na tela
      
            tela.setContrast(60);
            tela.print("RECEPTOR", CENTER, 0);
       //---------------------------------------FIM-------------------------------------------


       //---------------------------------------SD-CARD---------------------------------------
            if (!SD.begin(CS_PIN)) {                                    // inicialização da conexão CS do cartão de memoria       
                  Serial.println("ERROR - Cartao de memoria FALHOU");   //
                  return;                                               //
             }                                                          // end
       //---------------------------------------FIM-------------------------------------------



       //---------------------------------------RF-433--------------------------------------
            vw_set_ptt_inverted(true);                    // Required for RX Link Module
            vw_setup(2000);                               // Bits per sec
            vw_set_rx_pin(2);                             // We will be receiving on pin 4 i.e the RX pin from the module connects to this pin.
            vw_rx_start();                                // Start the receiver
         //---------------------------------------FIM-------------------------------------------
  }



/*----------------------------------------------------------------------------------------*/
  void loop() {
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];              // variaveis buf da biblioteca VM 433MHz
    uint8_t buflen = VW_MAX_MESSAGE_LEN;          // variaveis buf da biblioteca VM 433MHz
    
    client = "";                            
    
    if (vw_get_message(buf, &buflen)){            // se receber sinal do transmissor, entre
      if(buf[0] == 'c'){                              // analisa qual cliente esta enviando
         for (int i = 0; i < buflen; i++) {           // coleta todo dado do buffer recebido
           client += ((char)buf[i]);                  //
         }                                            // end if
         
         tela.print("Cliente: ", LEFT, 20);
         tela.print(client, RIGHT, 20);
         Serial.print("Cliente....: ");
         Serial.println(client);

         receberLeitura();                            // funcao receber leitura
         gravarBD();
      }
      Serial.println("");
    }                                             // end if
  }
/*----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------*/
void receberLeitura(){                    // funcao para receber leitura do gas, enviada pelo transmissor 433MHz           

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

         tela.setFont(MediumNumbers);
         tela.printNumI(leitura.toInt(), CENTER, 30);
         Serial.print("Leitura...: ");
         Serial.println(leitura);
}
/*----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------*/
  void gravarBD (){                        // funcao para gravar dados no cartao de memoria
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
/*----------------------------------------------------------------------------------------*/



