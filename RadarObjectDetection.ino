/*
  Project: Radar-Based Object Detection System
  Platform: Arduino (e.g. Uno)
  Functions:
    - Servo-based sensor scanning
    - Ultrasonic distance measurement
    - LCD status display
    - LED and buzzer alert
    - Bluetooth notification

  Hardware pin mapping:
    Bluetooth RX/TX: Arduino pins 2 and 3 (SoftwareSerial)
    Servo signal:    pin 8
    Ultrasonic TRIG: pin 9
    Ultrasonic ECHO: pin 10
    LED:             pin 7
    Buzzer:          pin 6

  Required libraries:
    - LiquidCrystal_I2C
    - Servo (normally included with Arduino IDE)
    - SoftwareSerial (normally included with Arduino IDE)
    - Wire (normally included with Arduino IDE)

  Detection rule retained from the original code:
    An object is detected after 4 consecutive valid readings
    between 2 cm and 10 cm.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// -------------------- Hardware configuration --------------------

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;
const byte LED_PIN = 7;
const byte BUZZER_PIN = 6;
const byte SERVO_PIN = 8;

const byte BLUETOOTH_RX_PIN = 2;
const byte BLUETOOTH_TX_PIN = 3;

const byte LCD_ADDRESS = 0x27;
const byte LCD_COLUMNS = 16;
const byte LCD_ROWS = 2;

// -------------------- Detection configuration --------------------

const int MIN_DETECTION_DISTANCE_CM = 2;
const int MAX_DETECTION_DISTANCE_CM = 10;
const byte REQUIRED_CONFIRMATIONS = 4;

const unsigned long ECHO_TIMEOUT_US = 30000UL;
const unsigned long ALERT_INTERVAL_MS = 3000UL;
const unsigned long BUZZER_DURATION_MS = 1000UL;
const unsigned long SERVO_SETTLE_MS = 15UL;

// -------------------- Objects and state --------------------

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
SoftwareSerial BT(BLUETOOTH_RX_PIN, BLUETOOTH_TX_PIN);
Servo radarServo;

int servoPosition = 0;
int servoDirection = 1;

byte confirmCount = 0;

unsigned long lastAlertTime = 0;
unsigned long buzzerStartTime = 0;

bool buzzerActive = false;
bool objectCurrentlyDetected = false;
bool firstAlertAllowed = true;

// -------------------- Setup --------------------

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Radar System");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  BT.begin(9600);

  radarServo.attach(SERVO_PIN);
  radarServo.write(servoPosition);

  delay(1500);
  showScanningMessage();
}

// -------------------- Main loop --------------------

void loop() {
  scanServo();

  int distanceCm = measureDistanceCm();

  updateDetection(distanceCm);
  updateBuzzer();
}

// -------------------- Servo scanning --------------------

void scanServo() {
  radarServo.write(servoPosition);
  delay(SERVO_SETTLE_MS);

  servoPosition += servoDirection;

  if (servoPosition >= 180) {
    servoPosition = 180;
    servoDirection = -1;
  } else if (servoPosition <= 0) {
    servoPosition = 0;
    servoDirection = 1;
  }
}

// -------------------- Ultrasonic measurement --------------------

int measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout prevents the program from waiting indefinitely
  // when the sensor does not receive an echo.
  unsigned long echoDuration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // A zero duration means no echo was received within the timeout.
  if (echoDuration == 0) {
    return -1;
  }

  int measuredDistance = (int)(echoDuration * 0.0343 / 2.0);
  return measuredDistance;
}

// -------------------- Object detection --------------------

void updateDetection(int distanceCm) {
  bool validDetectionReading =
      (distanceCm >= MIN_DETECTION_DISTANCE_CM &&
       distanceCm <= MAX_DETECTION_DISTANCE_CM);

  if (validDetectionReading) {
    if (confirmCount < REQUIRED_CONFIRMATIONS) {
      confirmCount++;
    }
  } else {
    confirmCount = 0;
  }

  bool objectDetected = (confirmCount >= REQUIRED_CONFIRMATIONS);

  if (objectDetected) {
    digitalWrite(LED_PIN, HIGH);

    if (!objectCurrentlyDetected) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Object Detected");
      objectCurrentlyDetected = true;
    }

    sendBluetoothAlertWhenDue();
  } else {
    digitalWrite(LED_PIN, LOW);

    if (objectCurrentlyDetected) {
      showScanningMessage();
      objectCurrentlyDetected = false;
    }
  }
}

// -------------------- Alerts --------------------

void sendBluetoothAlertWhenDue() {
  unsigned long currentTime = millis();

  if (firstAlertAllowed ||
      currentTime - lastAlertTime >= ALERT_INTERVAL_MS) {

    BT.println("Object Detected!");

    // Start the buzzer without blocking the scanning loop.
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerActive = true;
    buzzerStartTime = currentTime;

    lastAlertTime = currentTime;
    firstAlertAllowed = false;
  }
}

void updateBuzzer() {
  if (buzzerActive &&
      millis() - buzzerStartTime >= BUZZER_DURATION_MS) {

    digitalWrite(BUZZER_PIN, LOW);
    buzzerActive = false;
  }
}

// -------------------- LCD helper --------------------

void showScanningMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning...");
}
