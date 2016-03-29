# Detecting Motion with Infrared and Sonar Sensors

- Katherine Phan: graphics, timestamp support, hardware setup and testing
- Bradley Emi: sensor, graphics, piano audio and visuals

# Description

Our project is a demonstration of what can be accomplished with simple, $2 passive infrared and sonar sensors, and a Raspberry Pi running on a bare metal C operating system.

## Security System / Intruder Detection Mode

In security system mode, the monitor displays the locations of the 8 passive infrared sensors around the room, and flashes them in red when the sensor detects motion. When the infrared sensors are activated, the Raspberry Pi sends a signal to all 8 sonar sensors, which all send out a short 40 kHz sound pulse. Each sonar sensor will then listen for its own signal to be reflected back, much like a bat, a dolphin, or other animals which use echolocation to detect physical obstacles. If an object is detected, each sonar sensor will send back a high pulse to the Raspberry Pi with duration proportional to the distance to the obstacle. This information is displayed on the screen with a green target that shows where in the room obstacles were detected. The screen also displays timestamps corresponding to the most recent obstacle detections.

## Air Piano Mode

Just for fun, we also implemented an air piano using the sonar sensors. If you wave your hand in front of the sonar sensor, the corresponding key of the piano will flash blue on the screen, and the Raspberry Pi will send the audio signal through its AUX connection!

# Instructions for Use

- Follow the instructions in wiring.txt to connect 8 HR-SR501 sensors and 8 HR-SR04 sensors to the corresponding GPIO pins on the Raspberry Pi
- Connect an HDMI display to the HDMI port on the Raspberry Pi
- Connect a speaker to the audio jack on the Raspberry Pi (piano mode)
- Fork/clone the repository to your local machine, cd into local directory

## Security System / Intruder Detection Mode

- Git checkout master
- Make
- Make install 

<img src="http://i.imgur.com/pGXOGpq.jpg" height="200"> <img src="http://i.imgur.com/SVQ3bjB.jpg" height="200"> <img src="http://i.imgur.com/4JBf3dF.jpg" height="200">

## Air Piano Mode

- Git checkout pianomode
- Make
- Make install

<img src="http://i.imgur.com/C5LTqVA.jpg" height="200">

# Works Cited
- [Drawing a circle](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm)