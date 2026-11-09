#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Your LCD I2C address

// ADXL335 analog pins
#define X_PIN A0
#define Y_PIN A1
#define Z_PIN A2

// Outputs
#define BUZZER_PIN 9
#define LED_PIN 8

// Sensitivity threshold
#define THRESHOLD 12

// Baseline values
int BASE_X, BASE_Y, BASE_Z;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Earthquake Alarm");

  // Set pin modes
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Read stable baseline after a short delay
  delay(1000);
  BASE_X = analogRead(X_PIN);
  BASE_Y = analogRead(Y_PIN);
  BASE_Z = analogRead(Z_PIN);
}

void loop() {
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int z = analogRead(Z_PIN);

  int deltaX = abs(x - BASE_X);
  int deltaY = abs(y - BASE_Y);
  int deltaZ = abs(z - BASE_Z);

  Serial.print("ΔX: "); Serial.print(deltaX);
  Serial.print(" ΔY: "); Serial.print(deltaY);
  Serial.print(" ΔZ: "); Serial.println(deltaZ);

  // Check if any delta exceeds the threshold
  if (deltaX > THRESHOLD || deltaY > THRESHOLD || deltaZ > THRESHOLD) {
    // Earthquake detected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Earthquake!");

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);

    delay(1500);  // Keep buzzer/LED/LCD on for 1.5 seconds

    // Turn everything off
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Status: Monitoring...");
  }

  delay(100);  // Small delay before next reading
}
