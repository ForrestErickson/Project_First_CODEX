// Project_First_CODEX
// Based on the Arduino built-in "Blink" example.
// Customized for ESP32-S2 Dev K1: onboard/status LED on GPIO 2.

void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}
