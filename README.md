# Industrial Automation Project - Natural Gas Monitoring

This is an industrial automation project that monitors the consumption of natural gas. Baiscamente this project consists of an automation to collect the reading in the gas meters. It consists of two devices, called "client" and "server", were developed. For the "client", it is connected to any gas measuring device (hydrometer) that has digital pulse outputs, such as those used by the reed-swtich sensor in 5V.

The "client" device collects the meter reading and sends it to the "server" device. For the "server" device, it is responsible for collecting data from all clients on the network and ultimately registers that data on a memory card and then sends it to any external application. In this project, the "server" was used the bluetooth module to send data to an application.

**[Watch the "video" that shows how the system and devices work.](https://youtu.be/eZkTDlT5P_Q)**.

# 
![ezgif com-gif-maker 5](https://user-images.githubusercontent.com/29001162/46265104-01734600-c4fa-11e8-9367-49d24ceb8077.gif)
# 
![ezgif com-gif-maker 3](https://user-images.githubusercontent.com/29001162/46264816-40ec6300-c4f7-11e8-9ad5-300a76c08e3c.gif)



# 
# "Client" Device:

- (1) Display 5110 Blue;
- (1) RF 433 MHz module;
- (1) Arduino Uno;
- (1) Integrated Circuit 555;
- (1) Capacitors 100 μF;
- (1) Capacitors 10 nF;
- (3) Resistor 10 kΩ;
- (1) Resistor 220 Ω;
- (1) Resistor 270 Ω;
- (1) Led;
- (1) Head Meter G10

![cliente esquematico](https://user-images.githubusercontent.com/29001162/46265306-8579fd80-c4fb-11e8-9511-3819de96114d.png)



# 
# "Server" Device:

- (1) Display 5110 Red;
- (1) RF Receiver Module 433MHz;
- (1) Arduino Mega 2560;
- (1) Shild SD-CARD;
- (1) 4GB Memory Card;
- (5) Resistor 10 kΩ;
- (2) Resistor 1 kΩ;
- (1) Resistor 220 Ω;
- (1) Resistor 3900 kΩ;
- (1) Bluetooth module HC-06;

![servidor esquematico](https://user-images.githubusercontent.com/29001162/46265286-5e233080-c4fb-11e8-928d-dcb31ce9b05e.png)


# 
# Folders Map:

`Display\*`  
  code for tests using the library LCD5110.  
  
`Module Bluetooth\*`  
  code for bluetooth module integration tests.  
  
`Receptor e Display\*`  
  code for test integration with the display.  
  
`Receptor\*`  
  **client device code.**  
  
`Transmitter\*`  
  **server device code.**	

