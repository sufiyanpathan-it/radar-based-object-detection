# Radar-Based Object Detection System

## Project Overview
The Radar-Based Object Detection System is an Arduino-based project that detects nearby objects using an ultrasonic sensor mounted on a servo motor. The system scans its surroundings and provides alerts through an LCD display, LED, buzzer, and Bluetooth communication.

## Features
- Automatic scanning using a servo motor.
- Object detection using an ultrasonic sensor.
- Real-time distance measurement.
- LCD display for detection status.
- LED and buzzer alerts when an object is detected.
- Bluetooth notification when an object is detected.

## Hardware Components
- Arduino board
- Ultrasonic sensor
- Servo motor
- 16x2 I2C LCD display
- Bluetooth module
- LED
- Buzzer
- Jumper wires
- Breadboard
- Power supply

## Technologies Used
- Arduino
- Embedded C/C++
- Arduino IDE
- Ultrasonic sensing
- Servo motor control
- Bluetooth serial communication

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| Bluetooth RX/TX | 2, 3 |
| Servo Motor | 8 |
| Ultrasonic Trigger | 9 |
| Ultrasonic Echo | 10 |
| LED | 7 |
| Buzzer | 6 |
| I2C LCD | SDA, SCL |

## Working Principle
1. The servo motor moves the ultrasonic sensor to scan different directions.
2. The ultrasonic sensor measures the distance to nearby objects.
3. The Arduino processes the distance readings.
4. When an object is detected within the programmed distance range, the LED and buzzer provide alerts.
5. The LCD displays the object detection status.
6. A Bluetooth notification is sent when an object is detected.

## Source Code
The complete Arduino source code is available in:

`RadarObjectDetection.ino`

## How to Run
1. Download or clone this repository.
2. Open `RadarObjectDetection.ino` using Arduino IDE.
3. Install the required libraries, including `Servo` and `LiquidCrystal_I2C`.
4. Connect the components according to the pin configuration.
5. Select the correct Arduino board and COM port.
6. Upload the code to the Arduino board.
7. Power on the system and observe the scanning and detection process.

## Future Improvements
- Improve object detection accuracy.
- Add graphical radar visualization.
- Enhance Bluetooth monitoring.
- Extend the detection range.

## Author
**Sufiyan Pathan**

GitHub: [@sufiyanpathan-it](https://github.com/sufiyanpathan-it)

---

If you find this project useful, consider giving the repository a ⭐.

