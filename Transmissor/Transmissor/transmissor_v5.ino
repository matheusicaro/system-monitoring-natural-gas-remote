#include <VirtualWire.h>


#undef int
#undef abs
#undef double
#undef float
#undef round
String client = "c-1";
int n = 0;

void setup() {
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(2000);                 // Bits per sec
  vw_set_tx_pin(10);         // pin 10 ARDUINO-UNO
}

void loop() {
    sendCliente();
    sendLeitura();
}



/*----------------------------------------------------------------------------------------*/
void sendCliente() {

/********* trecho de codigo para envio de dados transmissor -> recptor */
    delay(1000);
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
    leitura += String(n);           //converte int em string
    delay(1000);
    
/********* trecho de codigo para envio de dados transmissor -> recptor */
    char tosend[leitura.length() + 1]; 
    leitura.toCharArray(tosend, sizeof(tosend));
    vw_send((uint8_t *)tosend, leitura.length() + 1);
    vw_wait_tx();
/********* end */

    n++;
    delay(200);
}
/*----------------------------------------------------------------------------------------*/


