# STM_Wire(less)GameController

#Table of Contents:
1.	Overview
2.	Description
3.	Tools
4.	How to run
5.	How to compile
6.	Future improvements
7.	License
8.	Credits


<h1>#1. Overview</h1>

  This project is based on STM32F4Discovery MicroController. The project is a wireless game controller and simple 2D game SpaceInvaders to test and play. It uses accelerometer to steer the character and sends data via Bluetooth module.

Parts used:
- STM32F4Discovery MicroController,
- Bluetooth Module XM-15B SPP.

<h1>#2.	Description</h1>

The external power supply is PC USB port. Gamepad uses accelerometer to move the character and USER button to shoot. It sends data via Bluetooth  in form of chars ranged from -10 to 10 (for movement) or 5 (for shooting), which is read by PC application. 

Screenshoot from the game: https://i.imgur.com/ujIT2sh.png.

<h1>#3.	Tools</h1>

STM32 programme was made in C in System Workbench for STM32.
PC game was written in C# using Unity game engine. 

<h1>#4.	How to run</h1>

Upload program to STM32F4, connect pins to Bluetooth module (TXD - PC11, RXD - PC10, Ground - Ground, Power - 5V).

Pair Bluetooth module with PC (PIN: 1234).

Run game (SpaceInvaders2/Game/Game.exe).

<h1>#5.	How to compile</h1>

Use System Workbench to compile and send data to MicroController’s flash memory.

<h1>#6.	Future improvements</h1>

Adding battery to the controller, so it doesn’t have to be connected by USB cable to PC.

Increasing quality and more stages in game, so it becomes more user-friendly.

<h1>#7.	License</h1>

Distributed under MIT license.

<h1>#8.	Credits</h1>

Project created by Piotr Biskup, Michał Maciaszek.

Computer Science, Faculty of Electrical Engineering, Poznan University of Technology

Supervisor: Marek Kraft
	
