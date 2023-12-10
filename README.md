# Keyestudio Arduino Tank Robot V3

![a picture of Cora, the Arduino Tank Robot V3](https://github.com/fernandocido/arduino-robot-keyestudio-v3-simple-code/blob/main/cora.png)

This is a simple code for controlling the movements of the Keyestudio Arduino Tank Robot V3, as well as avoiding front collisions and displaying images on the dot matrix. It was created with the help of ChatGPT 4.0.

## Features

  - Move the robot forward, backward, left, and right using Bluetooth commands.
  - Avoid obstacles using an ultrasonic sensor.
  - Display various images on the dot matrix.

## Requirements

  Keyestudio Arduino Tank Robot V3

## Basic Usage

  1) Connect the Arduino Tank Robot V3 to your computer using a USB cable.
  2) Open the Arduino IDE and upload the code to the robot.
  3) Turn on Bluetooth on your phone and pair it with the Bluetooth module.
  4) Open a serial terminal on your phone and send commands to move the robot or stop it.
      Use the following commands to control the robot:
        - 'F': move forward
        - 'B': move backward
        - 'L': turn left
        - 'R': turn right
        - 'S': stop
  5) The ultrasonic sensor will detect obstacles and stop the robot if it gets too close.
  6) The dot matrix will display various images depending on the robot's movement or status.

## Base Information

   More information about the Robot can be found here: 
     
   https://wiki.keyestudio.com/KS0526_Keyestudio_Mini_Tank_Robot_V3

## Credits

This code was created by Fernando Cido and ChatGPT 4.0. It is based on the base code provided by Keyestudio.
