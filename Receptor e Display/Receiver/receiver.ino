#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <VirtualWire.h>    
#include <SD.h>
#include <SPI.h>

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

//-----------------------------------CONFIG--SD-CARD-----------------------------------
    int pinSDCard = 53;
    int CS_PIN = pinSDCard;
    File file;  
//---------------------------------------FIM-------------------------------------------

//-----------------------------------VARIABLES OF PROJECT------------------------------
    String leitura = "";
    String client = "";
    String auxLeitura = "";
//---------------------------------------FIM-------------------------------------------





























  void setup()
  {
       pinMode (pinSDCard, OUTPUT);
       digitalWrite (pinSDCard, HIGH);

       Serial.begin(9600);
       startReceiver();                                                                       // function necessary to configure the 433MHz module
       startDisplay();                                                                        // necessary function to configure the display

       //---------------------------------------SD-CARD: Inicialization error---------------- // Check initialization of the SD-Card module
            if (!SD.begin(CS_PIN)) {                                                          //
                  Serial.println("ERROR - Cartao de memoria FALHOU");                         //
                  return;                                                                     //
             }                                                                                //
       //---------------------------------------FIM------------------------------------------ // end
       
       Serial.println("SETUP: Ok");
  }


  void loop() {

    
    uint8_t buf[VW_MAX_MESSAGE_LEN];                                                          // buf variables from the VM 433MHz library
    uint8_t buflen = VW_MAX_MESSAGE_LEN;                                                      // buf variables from the VM 433MHz library
    
    client = "";                            
    
    if (vw_get_message(buf, &buflen)){                                                        // if you receive any signal from the customers,
      if(buf[0] == '@'){                                                                      // check if the data received is the customer's identification, otherwise it is not an important data
         for (int i = 1; i < buflen; i++) {                                                   // scroll to the end of the buffer
           client += ((char)buf[i]);                                                          // stores the module buffer data 433MHz
         }                                                                                    // end if
         receiveData();                                                                       // function to receive customer readings
         saveDataBase();                                                                      // function to know the data received in SD-CARD, data in JSON: "client: read";
         printDisplay("");                                                                    // function to print the received data on the display
       }                                                                                      // 
    }                                                                                         // 

    if(Serial.read() == 'y'){                                                                 // if it receives the character "y" by serial (pin_0 and pin_1 of arduino)
        importDataBase();                                                                     // function required to import SD-CARD data
        delay(500);
    }
  }






















/*_____ S E T U P()________________________________________ startReceiver() _______________*/ 
    void startReceiver (){                                                                    // function necessary to configure the 433MHz module
            vw_set_ptt_inverted(true);                                                        // Required for RX Link Module
            vw_setup(2000);                                                                   // Bits per sec
            vw_set_rx_pin(2);                                                                 // We will be receiving on pin 4 i.e the RX pin from the module connects to this pin.
            vw_rx_start();                                                                    // Start the receiver
    }/*___________________________________________________________ end ____________________*/   




/*_____ S E T U P()________________________________________ startDisplay() ________________*/ // necessary function to configure the display
    void startDisplay (){
      for(int i=0; i < 2; i++){                                                               // it is necessary to set the initialization 2 times before established tests
           display.begin();                                                                   // initializes the display
           display.setContrast(60);                                                           // Adjust the display contrast
           display.clearDisplay();                                                            // Clears the buffer and display
           display.setTextSize(1);                                                            // Insert text size
           display.setTextColor(BLACK);                                                       // Insert text color

           display.clearDisplay();                                                            // Clears the buffer and display
           display.setTextSize(1);                                                            // Insert text size
           display.setTextColor(WHITE, BLACK);                                                // Insert the color of the text and background in the display
           display.setCursor(0,0);                                                            // Insert cursor position
           display.print("______________");                                                   // Print data
           display.setCursor(0,1);                                                            // Insert cursor position        
           display.print("   SERVIDOR   ");                                                   // Print data

           display.setTextColor(BLACK);                                                       // Insert black color in display text
           display.setCursor(0,15);                                                           // Insert cursor position
           display.println("Aguardando");                                                     // Print data
           display.print("Clientes...");                                                      // Print data
      }
    }/*___________________________________________________________ end _____________________*/   








/*====== L O O P ()====================================== printDisplay() ===================*/ // function to print the received data on the display
    void printDisplay(String msgError){                    
        if(msgError == ""){                                                                    // if you do not have any error messages, enter
                    display.clearDisplay();                                                    // Clears the buffer and display
                    display.setTextSize(1);                                                    
                    display.setTextColor(WHITE, BLACK);                                        // Insert the color of the text and background in the display
                    display.setCursor(0,0);                                                    // Insert cursor position             
                    display.print("______________");                                           // Print data
                    display.setCursor(0,1);                                                    // Insert cursor position   
                    display.print("   SERVIDOR   ");                                           // Print data
          
                    display.setTextColor(BLACK);                                               // Insert text color
                    display.setCursor(0,15);                                                   // Insert cursor position
                    display.print("Cliente: ");                                                // Print data
                    display.print(client);                                                     // Print data
                    
                    display.setTextSize(2);                                                    // Insert size in the display
                    display.setCursor(0,25);                                                   // Insert cursor position
                    display.print(leitura);                                                    // Print data
                    display.setTextSize(1);                                                    // Insert text size
                    display.setCursor(42,40);                                                  // Insert cursor position 
                    display.print(" Kg/m^3");                                                  // Print data
                    display.display();                                                         // Print data
        }else{
                    Serial.println(msgError);                                                  // Print data, in this case there will be an error message in other server processes
                    delay(2000);
        }                 
     }/*=========================================================== end ===================*/



/*====== L O O P ()====================================== receiveData() ===================*/
void receiveData(){                                                                            // function to receive reading of the gas, sent by the transmitter 433MHz           

  boolean recebido = false;                                                                    // Variable needed to control data received or not;
  int tempDelay = 0;                                                                           // variable for control of communication between the transmitter and receiver
  leitura = "";                                                                                // clearing global variable data, reading
  
  while(!recebido && tempDelay < 2147483646){                                                  // loop to wait for reading to be sent
    uint8_t buf[VW_MAX_MESSAGE_LEN];                                                           // buf variables from the VM 433MHz library
    uint8_t buflen = VW_MAX_MESSAGE_LEN;                                                       // buf variables from the VM 433MHz library
    if (vw_get_message(buf, &buflen)){                                                         // if you have some data received in the module buffer, enter     
       if(buf[0] == '#'){                                                                      // if the data is identified by the id (#) of the reading, enter
           for (int i = 1; i < buflen; i++) {                                                  // collects all received buffer data
             leitura += ((char)buf[i]);                                                        // stores data received from the buffer
           }                                                                                   // end for
       }                                                                                       // end if
    }
    if(leitura.length() > 0)                                                                   // if the global reading variable contains data, enter
      recebido = true;                                                                         // data received
    else
      tempDelay++;                                                                             // data not received, so add a counter (timer to avoid infinite loop) 
  }                                                                                            // end while
  
  if(!recebido){                                                                               // if you have not received data, report the error
      String msgError = "RF433-Failed";                                                        // print the error
      printDisplay(msgError);
  }
}/*=========================================================== end =========================*/



/*====== L O O P ()====================================== saveDataBase() ===================*/
  void saveDataBase(){                                                                         // function to write data to the memory card
    boolean save = false;                                                                      // variable for process control
    int tempDelay = 0;                                                                         // variable to avoid infinite loop
    String date = "_";                                                                         // arrow identifying the read files in SD-CARD
    date += client;                                                                            // Identification + Client name
    
    while(!save && tempDelay < 20){                                                            // while not recording data && maximum time reached, continue
        file = SD.open(date, FILE_WRITE);                                                      // open file with the name of the client, or create the file if it does not exist
        if (file) {                                                                            // if the file was opened, enter and save the data
            file.println(leitura);                                                             // write in txt
            file.close();                                                                      // close the file
            save = true;                                                                       // data saved successfully
        } else {                                        
            save = false;                                                                      // data not saved
        }
        tempDelay++;                                                                           // contactor for infinite loop control
    }                                                                                          // end while

    if(!save){                                                                                 // if data is not saved, enter
         String msgError = "SDCard-Failed";                                                    // error message
         printDisplay(msgError);                                                               // print error message
    }
    
    client = "";                                                                               // empty global variable;
  }/*=========================================================== end =======================*/



/*====== L O O P ()====================================== importDataBase() =================*/
   void importDataBase(){                                                                      // function required to import SD-CARD data
    File root = SD.open("/");                                                                  // variable to open directories in the root of SD-CARD
    root.rewindDirectory();                                                                    // start from the first file in the root directory
    importData(root, 0);                                                                       // function to import data from open directory
    root.close();                                                                              // close the directories file
   }

   void importData (File dir, int numTabs){                                                    // function that receives the file and sequence pointer to the next file
        while(true){                                                                           // in between

            File entry = dir.openNextFile();                                                   // start a file from the next directory
            if (! entry){                                                                      // if the file was opened
                if (numTabs == 0)                                                              // if there is no proxio file
                  Serial.println();                                                            // 
                return;                                                                        // end while
            }                                                                                  
            
            String nameFileClient = entry.name();                                              // variable to receive filename
            char idClient [nameFileClient.length()];                                           // initialize a char array of file name size
            nameFileClient.toCharArray(idClient, nameFileClient.length());                     // char array gets client name by characters
            
            if(idClient[0] == '_' || idClient[0] == 'J'){                                      // if file ID contains '_' and 'J', enter and read
                sendClientDate(nameFileClient);                                                // send data from archives found by ID
            }
            if (entry.isDirectory()){                                                          // if next file is contained in sequence, enter
                importData(entry, numTabs+1);                                                  // function recursion
            }
            entry.close();                                                                     // close the file
        }
  }/*=========================================================== end =======================*/



/*====== L O O P ()====================================== sendClientDate() =================*/
   void sendClientDate(String nameFileClient){                                                 // function needed to send data obtained from open files
        String clientLeitura = "";                                                             // temporary variable for file data
        File clientFile = SD.open(nameFileClient);                                             // open file with the name of the client received
        int linha = 0;                                                                         // variable for control of open file lines
        while(clientFile.available()){                                                         // while the file is available, continue
            if(clientFile.read() == '\n'){                                                     // if a filled line was found in the file, line ++
              linha ++;                                                                        // variable accountant
            }             
        }
        clientFile.close();                                                                    // close file to empty library buffer
        clientFile = SD.open(nameFileClient);                                                  // open file again
        
        if (linha == 0){                                                                       // if there is no line in the file, enter
            while(clientFile.available()){                                                     // while the file is available, continue
               clientLeitura += ((char)clientFile.read());                                     // receives the data read in the file
           }
        }else{                                                                                 // if there are several lines inside the file, take the last line
            int linha2 = 0;                                                                    // variable for control of last line
            boolean stopReading = false;                                                       // variable for loop control
            while(clientFile.available()){                                                     // while file is available, enter
               if(stopReading == false && clientFile.read() == '\n'){                          // pass to the next line, if it is not the last one or cose exite next line
                  linha2 ++;                                                                   // counter to the next row
               }
               if((linha2 + 1) == linha){                                                      // if it is the last line, enter
                  stopReading = true;                                                          // stop while
                  clientLeitura += ((char)clientFile.read());                                  // receives the data read in the file
               }
            }
        }
        Serial.println(nameFileClient + ": " + clientLeitura);                                 // print the read data on the serial
        clientFile.close();                                                                    // close the file
   }
/*=========================================================== end =================================================*/
