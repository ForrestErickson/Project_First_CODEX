// Project_First_CODEX
// Based on the Arduino built-in "Blink" example.
// Customized for ESP32-S2 Dev K1: onboard/status LED on GPIO 2.

const uint8_t LED_PIN = 2;
const unsigned long SERIAL_BAUD_RATE = 115200;
const unsigned long LED_ON_MS = 1000;
const unsigned long LED_OFF_MS = 1000;

bool ledIsOn = false;
unsigned long lastToggleAtMs = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("[SETUP] Starting setup...");

  // Placeholder for future setup steps (WiFi, OTA, MQTT, etc.)

  Serial.println("[SETUP] Setup complete.");
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const unsigned long nowMs = millis();
  const unsigned long intervalMs = ledIsOn ? LED_ON_MS : LED_OFF_MS;

  if (nowMs - lastToggleAtMs >= intervalMs) {
    ledIsOn = !ledIsOn;
    digitalWrite(LED_PIN, ledIsOn ? HIGH : LOW);
    lastToggleAtMs = nowMs;
  }
}
