    
#include <SoftwareSerial.h>  
   
int bluetoothTx = 13;
int bluetoothRx = 14;
 
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

String command = ""; //response of the module to the inserted commands
   
void setup()   
{  
  // Open serial communications and wait for port to open:  
  bluetooth.begin(9600);  
  Serial.begin(115200);  
  Serial.println("Digite os comandos AT!");  

  /*
    COMMAND         RESPONSE         FUNCTION
    AT              OK               Communication test
    AT+VERSION      OKlinvorV1.8     Displays the firmware version
    AT+NAMExyz      OKsetname        Changes the module name
    AT+PIN1234      OKsetPIN         Change the module password
    AT+BAUD1        OK1200           Seta o baud rate in 1200
    AT+BAUD2        OK2400           Seta o baud rate in 2400
    AT+BAUD3        OK4800           Seta o baud rate in 4800
    AT+BAUD4        OK9600           Seta o baud rate in 9600
    AT+BAUD5        OK19200          Seta o baud rate in 19200
    AT+BAUD6        OK38400          Seta o baud rate in 38400
    AT+BAUD7        OK57600          Seta o baud rate in 57600
    AT+BAUD8        OK115200         Seta o baud rate in 115200
    AT+BAUD9        OK230400         Seta o baud rate in 230400
    AT+BAUDA        OK460800         Seta o baud rate in 460800
    AT+BAUDB        OK921600         Seta o baud rate in 921600
    AT+BAUDC        OK1382400        Seta o baud rate in 1382400
  */

}  
   
void loop()  
{  
  // Read device output if available.  
  if (bluetooth.available()) 
  {  
     while(bluetooth.available()){
      // While there is more to be read, keep reading.  
       command += (char)bluetooth.read();  
     }  
   Serial.print(command);  
   command = ""; // No repeats  
  }  
  
  // Read user input if available.  
  if (Serial.available() > 0)
  {  
    delay(10); // The DELAY!
    bluetooth.println("TEEEESTEE");
    bluetooth.write(Serial.read());
  }   
}

