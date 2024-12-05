#include <LiquidCrystal.h>
#include <Encoder.h>

// LCD Setup
const int rs = 12, en = 11, d4 = 5, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Motor pins
const int motorEnableA = 4;
const int motorInput1 = 7;
const int motorInput2 = 6;

// Potentiometer pin
const int potPin = A0;

// Create Encoder object
Encoder myEnc(2, 3);

// Variables
int motorSpeed = 0;
long lastPosition = 0;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Starting...");
  
  // Initialize motor pins
  pinMode(motorEnableA, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  
  // Initialize potentiometer pin
  pinMode(potPin, INPUT);
}

void loop() {
  // Read potentiometer
  int potValue = analogRead(potPin);
  
  // Convert pot value to motor speed and direction
  // 0-512 = reverse, 512-1023 = forward
  if(potValue < 512) {
    motorSpeed = map(potValue, 511, 0, 0, 255);  // Reverse
  } else {
    motorSpeed = map(potValue, 512, 1023, 0, 255);  // Forward
  }
  
  // Read encoder for position feedback
  long currentPosition = myEnc.read();
  
  // Set motor speed
  setMotorSpeed(motorSpeed * (potValue < 512 ? -1 : 1));
  
  // Update displays
  if (currentPosition != lastPosition) {
    updateLCD(currentPosition, motorSpeed, potValue);
    updateSerial(currentPosition, motorSpeed, potValue);
    lastPosition = currentPosition;
  }
  
  delay(10);
}

void setMotorSpeed(int speed) {
  speed = constrain(speed, -255, 255);
  
  // Dead zone handling
  if(abs(speed) < 20) {
    speed = 0;
  }
  
  if (speed >= 0) {
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
  } else {
    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, HIGH);
  }
  
  analogWrite(motorEnableA, abs(speed));
}

void updateLCD(long pos, int speed, int pot) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pos:");
  lcd.print(pos);
  
  lcd.setCursor(0, 1);
  lcd.print("Spd:");
  lcd.print(speed);
  lcd.print(" P:");
  lcd.print(pot);
}

void updateSerial(long pos, int speed, int pot) {
  Serial.print("Position: ");
  Serial.print(pos);
  Serial.print(" | Speed: ");
  Serial.print(speed);
  Serial.print(" | Pot: ");
  Serial.print(pot);
  Serial.print(" | Dir: ");
  Serial.println(speed >= 0 ? "CW" : "CCW");
}