# BalanceCar
BalanceCar is a Self-balancing car Project 
# What does this Project contain?
A complete two-wheel self-balancing car project based on the STM32 microcontroller includes the schematics of the main control board (STM32F405RGT6) and the remote control board (STM32F103C8T6), PCB layout files, Gerber files and firmware.
# Development Environment
1. The firmware development environment uses MDK5.20
2. Schematic drawing using Orcad Capture CIS
3. The layout environment uses Cadence Allegro 16.6
4. Gerber file export type is RS274-X
# Communications
1. The remote control board communicates with the main control board using the 2.4G module NRF24L01
2. The phone communicates with the main control board through the Bluetooth serial port
3. The main control board is reserved OLED, USART and SWD interface
# Demo
![效果](https://github.com/eCodeing/BalanceCar/blob/master/demo.gif)
