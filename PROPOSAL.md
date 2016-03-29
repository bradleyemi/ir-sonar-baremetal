# IR Motion Detection Project

![alt text](https://imgs.xkcd.com/comics/actors.png "xkcd image")

## Team members
* Bradley Emi: interrupt handler
* Kat Phan: reading in sensor information

## Goal of the project
* Use passive IR sensors to detect motion
* Depict motion through different modes of visualization

## Technical details
* Use multiple IR sensors to interpolate the position of a moving, hot target in an enclosed space
* Set up triggers in a circle, do a visualization of which sensors have been triggered at approximate times, maybe do some math and figure out an approximate walking pattern of the subject?)

## Resources needed
* 10 Passive IR Sensors ($5 each)
* 10 transistors
* Really long jumper wires (can probably borrow from physics dept)
* Speakers (if pursuing music project)
* Monitor

## Milestones

### March 7th (Monday)

* Be able to read in sensors, do basic tests (light up LED)
* Have all sensors hooked up to breadboard and RPi

### March 9th

* Set final goal (piano vs. visualization based on sensitivity of sensors)

### March 11

* Complete project

### March 14th

* Present in Gates 3rd floor landing

## Possible Extensions
* Music (air piano/guitar)
* Piano stairs
* Visualize a person’s path around a room
* Graphics library

## Details and Documentation
* https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/using-a-pir
* http://diyhacking.com/raspberry-pi-gpio-control/