# Industrial Automation Project Monitoring of Natural Gas

This is an industrial automation project that monitors the consumption of natural gas. Baiscamente this project consists of an automation to collect the reading in the gas meters. It consists of two devices, called "client" and "server", were developed. For the "client", it is connected to any gas measuring device (hydrometer) that has digital pulse outputs, such as those used by the reed-swtich sensor in 5V. The "client" device collects the meter reading and sends it to the "server" device. For the "server" device, it is responsible for collecting data from all clients on the network and ultimately registers that data on a memory card and then sends it to any external application. In this project, the "server" was used the bluetooth module to send data to an application.This repository contains the device communication code, such as:

- "Client" Device: Arduino Uno + RF Transmitter Module 433MHz + Display 5110 Blue

- "Server" Device: Arduino Mega + Shield-SD-Card Module + Bluetooth Module HC-06 + Receiver 433MHz + Display 5110 Red

Still, it contains code for tests of communication between the display 5110 and modulo RF 433Mhz.

Learn more about the project through the video at the link below.
