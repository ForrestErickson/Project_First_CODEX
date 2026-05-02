// Project_First_CODEX
// Based on the Arduino built-in "Blink" example.
// Customized for ESP32-S2 Dev K1: onboard/status LED on GPIO 2.

const uint8_t LED_PIN = 2;
const unsigned long LED_ON_MS = 1000;
const unsigned long LED_OFF_MS = 1000;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(LED_ON_MS);
  digitalWrite(LED_PIN, LOW);
  delay(LED_OFF_MS);
}
