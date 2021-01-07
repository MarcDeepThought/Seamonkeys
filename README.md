# Seamonkeys (Urzeitkrebse)
 
This is a small project for growing Seamonkeys.

DE: Dies ist ein kleines Projekt zum Züchten von Urzeitkrebsen.

*** 

### 

![Picture](https://github.com/MarcDeepThought/Seamonkeys/blob/main/fotos/Seamonkeys2.jpg)

This project is based on an article of the magazine [Make-Ausgabe 5/2020](https://www.heise.de/select/make/2020/5/seite-78) . 
[The corresponding GitHub project can be found here.](https://github.com/MakeMagazinDE/Urzeitkrebse)

***

### Setup Raspbian Pi Zero with camera modul v2.1
The Raspbian Pi Zero is used for taking photos of the sea monkeys on a regular basis (e.g. every 15 minutes).
1. optional: Download the latest version of Rasbian Pi OS Lite.
2. Install Rasbian Pi OS Lite on a Micro SD card using Raspberry Pi Imager (v1.4).
3. Enable SSH in Rasbian Pi OS Lite, so that it is available right after the first start:
	- Create an empty file called "ssh" in the boot partition of the SD Card (e.g. parallel to "bootcode.bin").
4. Setup WLAN configuration:
	- Create a file wpa_supplicant.conf in the boot partition of the SD Card. An exmaple config file is here: [wpa_supplicant.conf](https://github.com/MarcDeepThought/Seamonkeys/blob/main/raspberry_pi_scripts/wpa_supplicant.conf) 
5. Insert the SD Card in the Raspberry Pi Zero and power it on.
6. Connect to Raspberry Pi Zero via SSH (user: pi // password: raspberry).
7. Create folders, one for the scripts and one for the pictures, via command line:
	mkdir -p pictures scripts
8. Update the Raspbian Pi OS:
	sudo apt update
	sudo apt upgrade
9. Install Python3 picamera module:
	sudo apt install python3-picamera
10. Navigate to scripts folder and either create a file called make_picture.py using nano
		nano ~/scripts/make_picture.py
	or copy the file [make_picture](https://github.com/MarcDeepThought/Seamonkeys/raspberry_pi_scripts/make_picture) into the scripts folder.
	This script takes a picture and afterwards it makes a short video (about 10 seconds).	
11. Create a cron trigger for the Python script: Add the following line
		0,15,30,45, * * * * pi /home/pi/scripts/make_picture.py
	with
		sudo nano /etc/crontab	
12. Activate the camera by typing the following into the terminal:
	sudo raspi-config
	
***

### Setting up the DIY 7segment Arduino UNO shield

A description how to create your own 7 segment shield for the Arduino UNO is described in a separate project.
[You can find the project here](https://github.com/MarcDeepThought/DIY_7segment_Arduino_UNO_shield) and it is considered to be a prerequisite for this project.

***

## Requirements

- [ ] Seamonkeys with food and tank
- [ ] Arduino Uno
- [ ] DS18B20 IC digital temperature sensor (waterproof)
- [ ] (optional) LM35 temperature sensor
- [ ] (optional) TMP36 temperature sensor
- [ ] MOSFET modul IRF520
- [ ] Heating pad (12V, ~11 Ohm -> run at 9V), water-repellent
- [ ] Raspberry Pi Zero W
- [ ] Pi-Zero-Gehäuse
- [ ] Raspberry Pi Camera modul V2.1
- [ ] Micro-SD card 16GB
- [ ] Wires
- [ ] Plug-in power supply (9V/1A, temperature protection, short-circuit proof)
- [ ] Adapter with screw terminals for barrel connector
- [ ] (optional) Microfuse (Feinsicherung/Schmelzsicherung), 0,6A flink


***

## Fotos

[Additional fotos](https://github.com/MarcDeepThought/Seamonkeys/blob/main/fotos/)