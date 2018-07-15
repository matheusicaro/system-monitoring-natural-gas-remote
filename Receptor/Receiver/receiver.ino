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
                  String msgError = "Error-Setup-SD"; 
                  printDisplay("msgError");
                  return;
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
          if((char)buf[i] =='#'){
               for (int j = i+1; j < buflen; j++)                                             // scroll to the end of the buffer
                    leitura += ((char)buf[j]);
               i = buflen;
           }else{
               client += ((char)buf[i]);                                                       // stores the module buffer data 433MHz
           }
         }                                                                                    // end if
         //receiveData();                                                                       // function to receive customer readings
         saveDataBase();                                                                      // function to know the data received in SD-CARD, data in JSON: "client: read";
         printDisplay("");                                                                    // function to print the received data on the display
       }                                                                                      // 
        
        leitura = "";                                                                         // empty global variable;
        client = "";                                                                          // empty global variable;
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
                    display.clearDisplay();                                                    // Clears the buffer and display
                    display.setTextSize(1);                                                    
                    display.setTextColor(WHITE, BLACK);                                        // Insert the color of the text and background in the display
                    display.setCursor(0,0);                                                    // Insert cursor position             
                    display.print("______________");                                           // Print data
                    display.setCursor(0,1);                                                    // Insert cursor position   
                    display.print("   SERVIDOR   ");                                           // Print data
          
                    display.setTextColor(BLACK);                                               // Insert text color
                    display.setCursor(0,15);                                                   // Insert cursor position
        if(msgError != ""){                                                                    // if you do not have any error messages, enter
                    display.print(msgError);                                                   // Print data
        }else{
                    display.print("Cliente: ");                                                // Print data
                    display.print(client);                                                     // Print data  
        }                    
                    display.setTextSize(2);                                                    // Insert size in the display
                    display.setCursor(0,25);                                                   // Insert cursor position
                    display.print(leitura);                                                    // Print data
                    display.setTextSize(1);                                                    // Insert text size
                    display.setCursor(42,40);                                                  // Insert cursor position 
                    display.print(" Kg/m^3");                                                  // Print data
                    display.display();                                                         // Print data
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
            
            if(idClient[0] == '_'){                                                            // if file ID contains '_' and 'J', enter and read
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
            if((char)clientFile.read() == '\n'){                                               // if a filled line was found in the file, line ++
              linha ++;                                                                        // variable accountant
            }
        }

        clientFile.close();                                                                    // close file to empty library buffer
        
        if (linha == 1){                                                                       // if there is no line in the file, enter
            clientFile = SD.open(nameFileClient);                                              // open file again
            while(clientFile.available()){                                                     // while the file is available, continue
               clientLeitura += ((char)clientFile.read());                                     // receives the data read in the file
           }
        }else{
            clientFile = SD.open(nameFileClient);                                              // open file again
            int linha2 = 0;                                                                    // variable for control of last line
           
            while(clientFile.available()){                                                     // while file is available, enter
              
               if((char)clientFile.read() == '\n'){                                            // pass to the next line, if it is not the last one or cose exite next line
                  linha2 ++;                                                                   // counter to the next row
               }else if ((linha2 + 1) == linha){                                               // if it is the last line, enter
                  clientLeitura += ((char)clientFile.read());                                  // receives the data read in the file
               }
            }
        }
        Serial.print(nameFileClient);
        Serial.print(": ");
        Serial.println(clientLeitura);                                                         // print the read data on the serial
        
        clientFile.close();                                                                    // close the file
    }
/*=========================================================== end =================================================*/
